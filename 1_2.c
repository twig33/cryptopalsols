#include <stdio.h>
#include <cstring>
#include <stdlib.h>

extern char const base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
extern char const hex[] = "0123456789abcdef";

void hex3_to_base64(unsigned short* input, unsigned short* output){
	unsigned short result1 = 0;
	result1 = input[1] >> 2;
	result1 |= (input[0] << 2);
	result1 &= 0x3f;
	unsigned short result2 = 0;
	result2 = ((input[1] << 6) >> 2);
	result2 |= input[2];
	result2 &= 0x3f; //0x3f is 00111111
	output[0] = result1;
	output[1] = result2;
}
unsigned short* hex_to_base64(unsigned short* input, int size, int* outputSize){
	*outputSize = (size+size%3)/3*2;
	unsigned short* result = (unsigned short*)calloc(*outputSize, sizeof(unsigned short));
	unsigned short* inputf = (unsigned short*)calloc(3, sizeof(unsigned short));
	for (int i = 0; i < size + size%3; i += 3){

		if ((size%3) && i + 3 >= size + size%3){
			inputf[0] = input[i];
			if (size%3 == 1){
				inputf[1] = input[i + 1];
				inputf[2] = 0;
			}else{
				inputf[1] = 0;
				inputf[2] = 0;
			}
		}
		else
		{
			inputf[0] = input[i];
			inputf[1] = input[i+1];
			inputf[2] = input[i+2];
		}
		hex3_to_base64(&inputf[0],&result[i/3*2]);
	}
	free(inputf);
	return result;
}

unsigned short* string_to_hex(const char* input){
	int size = (unsigned)strlen(input);
	unsigned short* result = (unsigned short*)calloc(size, sizeof(unsigned short));
	for (int i = 0; i < size; ++i){
		if (input[i] > 57){
			result[i] = input[i] - 87;
		}
		else
		{
			result[i] = input[i] - 48;	
		}
	}
	return result;
}

char* hex_to_string(unsigned short* input, int size){
	char* result = (char*)calloc(size+1,sizeof(char));
	result[size] = '\0';
	for (int i = 0; i < size; ++i){
		result[i] = hex[input[i]];	
	}
	return result;
}

char* base64_to_string(unsigned short* input, int size){
	char* result = (char*)calloc(size+1, sizeof(char));
	result[size] = '\0';
	for (int i = 0; i < size; ++i){
		result[i] = base64[input[i]];
	}
	return result;
}


unsigned short* short_xor_short(unsigned short* input1, unsigned short* input2, int size){
	unsigned short* result = (unsigned short*)calloc(size,sizeof(unsigned short));
	for (int i = 0; i < size; ++i){
		result[i] = (input1[i])^(input2[i]);
	}
	return result;
}
char* hexstr_to_base64str(char* input){
	int size = (unsigned)strlen(input);
	unsigned short* hex = string_to_hex(&input[0]);
	int outputSize;
	unsigned short* base64 = hex_to_base64(&hex[0], size, &outputSize);
	char* result = base64_to_string(&base64[0], outputSize);
	free(hex);
	free(base64);
	return result;
}


int main (){
    char s1[100];
	char s2[100];
	scanf("%s",s1);
	scanf("%s",s2);
	int size = (unsigned)strlen(s1);
	unsigned short* hex1 = string_to_hex(s1);
	unsigned short* hex2 = string_to_hex(s2);
	unsigned short* hexxored = short_xor_short(hex1, hex2, size);
	char* hexXoredStr = hex_to_string(hexxored, size);
	free(hex1);
	free(hex2);
	free(hexxored);
	printf("%s\n", hexXoredStr);
	free(hexXoredStr);
	return 0;
}