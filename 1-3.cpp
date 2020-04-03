#include <vector>
#include <iostream>
#include <algorithm>
#include "score.h"

static const unsigned int base64_decoding[] = {62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,
								8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,
								30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51}; //range from '+' to 'z'

static const char* base64_encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char* hex_encoding = "0123456789abcdef";

void print_byte(unsigned int a){
	for (int i = 0; i < 8; ++i){
		std::cout << (bool)(a & (0x80 >> i));	
	}
	std::cout << '\n';
}

std::vector<unsigned int> sextets_to_octets(const std::vector<unsigned int> in){
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < in.size(); ++i){
		switch(i%4){
			case 0:
				result.push_back(in[i] << 2);
				break;
			case 1:
				result[result.size()-1] |= (in[i] >> 6);
				result.push_back(in[i] << 4);
				break;
			case 2:
				result[result.size()-1] |= (in[i] >> 2);
				result.push_back(in[i] << 6);
				break;
			case 3:
				result[result.size()-1] |= in[i];
				break;
		}
	}
	return result;
}

std::vector<unsigned int> octets_to_sextets(const std::vector<unsigned int> in){
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < in.size(); ++i){
		switch (i%3){
			case 0:
				result.push_back((in[i] & 0xFC) >> 2);
				result.push_back((in[i] & 0x03) << 4);
				break;
			case 1:
				result[result.size()-1] |= ((in[i] & 0xF0) >> 4);
				result.push_back((in[i] & 0x0F) << 2);
				break;
			case 2:
				result[result.size()-1] |= ((in[i] & 0xC0) >> 6);
				result.push_back(in[i] & 0x3F);
		}
	}
	return result;
}

std::vector<unsigned int> nibbles_to_octets(const std::vector<unsigned int> in){
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < in.size(); ++i){
		switch(i%2){
			case 0:
				result.push_back(in[i] << 4);
				break;
			case 1:
				result[result.size()-1] |= in[i];
				break;
		}
	}
	return result;
}

std::vector<unsigned int> octets_to_nibbles(const std::vector<unsigned int> in){
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < in.size(); ++i){
		result.push_back((in[i] & 0xF0) >> 4);
		result.push_back(in[i] & 0x0F);
	}
	return result;
}

unsigned int decode_base64(const char c){
	if (c < '+' || c > 'z'){
		return -1;	
	}
	return base64_decoding[c - '+'];
}

std::vector<unsigned int> decode_base64(const std::string s){
	std::vector<unsigned int> sextets;
	for (unsigned int i = 0; i < s.length(); ++i){
		sextets.push_back(decode_base64(s[i]));
	}
	return sextets_to_octets(sextets);
}

char encode_base64(unsigned int in){
	return base64_encoding[in];	
}

std::string encode_base64(const std::vector<unsigned int> in){
	std::vector<unsigned int> sextets = octets_to_sextets(in);
	std::string result = "";
	for (unsigned int i = 0; i < sextets.size(); ++i){
		result += encode_base64(sextets[i]);	
	}
	return result;
}

unsigned int decode_hex(const char c){
	if (c >= '0' && c <= '9'){
		return c - '0';	
	}
	if (c >= 'a' && c <= 'f'){
		return c - 'a' + 10;	
	}
	return -1;
}

std::vector<unsigned int> decode_hex(const std::string s){
	std::vector<unsigned int> result;
	for (unsigned int i = 0; i < s.length(); ++i){
		result.push_back(decode_hex(s[i]));	
	}
	return nibbles_to_octets(result);
}

char encode_hex(unsigned int in){
	return hex_encoding[in];	
}

std::string encode_hex(const std::vector<unsigned int> in){
	std::string result = "";
	std::vector<unsigned int> nibbles = octets_to_nibbles(in);
	for (int i = 0; i < nibbles.size(); ++i){
		result += encode_hex(nibbles[i]);	
	}
	return result;
}

std::vector<unsigned int> xor_buf(const std::vector<unsigned int> a, const std::vector<unsigned int> b){
	if (a.size() < b.size()){
		//std::swap(a, b);	
	}
	if (a.size() % b.size() != 0)
		return {};
	std::vector<unsigned int> result;
	for (int i = 0; i < a.size()/b.size(); ++i){
		for (int j = 0; j < b.size(); ++j){
			result.push_back(a[i*b.size()+j] ^ b[j]);	
		}
	}
	return result;
}

std::string octets_to_string(const std::vector<unsigned int> a){
	std::string result = "";
	for (int i = 0; i < a.size(); ++i){
		result += (char)a[i];		
	}
	return result;
}

struct decoding {
	std::string s;
	unsigned int score;
	unsigned int key;
};

bool compare(decoding a, decoding b){
	return a.score > b.score;	
}

int main(){
	std::vector<decoding> decodings;
	std::string hex;
	std::cin >> hex;
	std::vector<unsigned int> decoded = decode_hex(hex);
	for (unsigned int i = 0; i <= 0xFF; ++i){
		std::string a = octets_to_string(xor_buf(decoded, {i}));
		decodings.push_back({a, score(a), i});
	}
	std::sort(decodings.begin(), decodings.end(), compare);
	std::cout << "Best decodings based on score:\n";
	for (int i = 0; i < 5; ++i){
		std::cout << "Score: " << decodings[i].score << " Key:" << decodings[i].key << '\n';
		std::cout << "Decoding: " << decodings[i].s << '\n';
	}
}