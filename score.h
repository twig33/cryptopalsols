#include <vector>
#include <math.h>
#include <ctype.h>

static const float frequencies[] = {0.081, 0.015, 0.022, 0.043, 0.127,
									0.022, 0.020, 0.061, 0.070, 0.002,
									0.013, 0.040, 0.024, 0.067, 0.075,
									0.019, 0.010, 0.060, 0.063, 0.094,
									0.028, 0.010, 0.026, 0.001, 0.020,
									0.008};

float get_freq(const std::string s, const char c){
	int ct = 0;
	for (int i = 0; i < s.length(); ++i){
		ct += (s[i] == c);	
	}
	return (float)ct / s.length();
}

std::string format (std::string s){
	std::string result = "";
	for (int i = 0; i < s.length(); ++i){
		if (s[i] >= 'a' && s[i] <= 'z'){
			result += s[i];	
		}
		else if (s[i] >= 'A' && s[i] <= 'Z'){
			result += tolower(s[i]);
		}
	}
	return result;
}

//this english plaintext scoring function could be better
unsigned int score (std::string s) { // from 0 to 100
	s = format(s);
	float result = 0;
	float max_letter = (float)100/26;
	for (int i = 0; i < 26; ++i){
		float freq = get_freq(s, 'a' + i);
		float diff = frequencies[i] - freq;
		diff = abs(diff);
		if (diff > frequencies[i]) // completely out of range
			continue;
		result += ((frequencies[i]-diff) / frequencies[i]) * max_letter;
	}
	if (result > 100){
		result = 100;	
	}
	return floor(result);
}