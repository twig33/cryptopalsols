#include <vector>
#include <iostream>

static const unsigned int base64_decoding[] = {62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,
								8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,
								30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51}; //range from '+' to 'z'

static const char* base64_encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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

int main(){
	std::string hex;
	std::cin >> hex;
	std::cout << encode_base64(decode_hex(hex));
}