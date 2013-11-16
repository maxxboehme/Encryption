/*
 * RSA.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Maxx Boehme
 */
#include <iostream>
#include <fstream>
#include <ctime>
#include <streambuf>
#include <cstring>

using namespace std;

typedef unsigned long long Long;
typedef char byte;

Long findHighestSetBit(Long);
int fastModularExp(Long, Long, Long);
void encryption(string, string, string);
void decryption(string, string, string);
byte* intToByteArray(int);
string intToBinary(int);
string byteToBinary(byte);
string byteAC(byte *, int);
string byteAS(byte *, int);

int main(int argc, char *args[]){
	if(argc == 5){
		if(!strcmp(args[1], "e")){
			cout << "encrypting" << endl;
			encryption(args[2], args[3], args[4]);
		}else if(!strcmp(args[1], "d")){
			cout << "decrypting" << endl;
			decryption(args[2], args[3], args[4]);
		} else {
			cerr << "Do not know command entered: " << args[1] << endl;
		}
	} else {
		cerr << "Did not enter correct number of argumensts: 5"<< endl;
	}
	return 0;
}

void encryption(string nameInputFile, string nameKeyFile, string nameOutputFile) {
	ifstream keyStream;
	keyStream.open(nameKeyFile.c_str());
	Long keys[3];
	int keyIndex = 0;
	while(keyStream.good() && keyIndex < 3){
		keyStream >> keys[keyIndex++];
	}
	keyStream.close();

	//creates file streams for the files
	ifstream istream;
	//	char bigbufferR[BUFF_SIZE];
	//	char *bigbufferR = new char[BUFF_SIZE];
	//	istream.rdbuf()->pubsetbuf(bigbufferR, BUFF_SIZE);
	istream.open(nameInputFile.c_str(), ios::binary);

	ofstream ostream;
	//	char bigbufferW[BUFF_SIZE];
	//	char *bigbufferW = new char[BUFF_SIZE];
	//	ostream.rdbuf()->pubsetbuf(bigbufferW, BUFF_SIZE);
	ostream.open(nameOutputFile.c_str(), ios::binary);

	int message = 0;
	byte bytes[3];

	//checks to make sure there are still bytes left to read
	int length = 0;
	while(istream.good()) {
		istream.read(bytes, 3);
		length = istream.gcount();
		if(length > 0){
			message = 0;
			//concatenates 3 bytes together in message
			for(int k = 0; k < length; k++){
				message = message | ((bytes[k] & 0xFF)<<(8*(2-k)));
			}
			//encrypts the message using fast modular exponentiation
			message = fastModularExp(message, keys[1], keys[0]);
			//writes all 4 bytes to the output file
			ostream.write(intToByteArray(message), 4);
		}
	}
	//	delete[] bigbufferR;
	//	delete[] bigbufferW;
	ostream.close();
	istream.close();
}

void decryption(string nameInputFile, string nameKeyFile, string nameOutputFile) {
	ifstream keyStream;
	keyStream.open(nameKeyFile.c_str());
	Long keys[3];
	int keyIndex = 0;
	while(keyStream.good() && keyIndex < 3){
		keyStream >> keys[keyIndex++];
	}
	keyStream.close();

	//creates file streams for the files
	ifstream istream;
	//	char bigbufferR[BUFF_SIZE];
	//	char *bigbufferR = new char[BUFF_SIZE];
	//	istream.rdbuf()->pubsetbuf(bigbufferR, BUFF_SIZE);
	istream.open(nameInputFile.c_str(), ios::binary);
	ofstream ostream;
	//	char bigbufferW[BUFF_SIZE];
	//	char *bigbufferW = new char[BUFF_SIZE];
	//	ostream.rdbuf()->pubsetbuf(bigbufferW, BUFF_SIZE);
	ostream.open(nameOutputFile.c_str(), ios::binary);

	int message = 0;
	byte bytes[4];

	istream.read(bytes, 4);
	int length = istream.gcount();
	while(length > 0) {
		message = 0;
		//concatenates 3 bytes together in message
		for(int k = 0; k < length; k++){
			message = message | ((bytes[k] & 0xFF)<<(8*(3-k)));
		}

		//encrypts the message using fast modular exponentiation
		message = fastModularExp(message, keys[2], keys[0]);

		//writes all 4 bytes to the output file
		byte *messageArray = intToByteArray(message);

		istream.read(bytes, 4);
		length = istream.gcount();

		if(length == 0){
			ostream.write(&messageArray[1], 1);
			if(messageArray[2]){
				ostream.write(&messageArray[2], 1);
			}
			if(messageArray[3]){
				ostream.write(&messageArray[3], 1);
			}
		} else {
			ostream.write(&messageArray[1], 1);
			ostream.write(&messageArray[2], 1);
			ostream.write(&messageArray[3], 1);
		}
		delete[] messageArray;
	}
	//	delete[] bigbufferR;
	//	delete[] bigbufferW;
	ostream.close();
	istream.close();
}

int fastModularExp(Long a, Long b, Long c){
	Long result = 1;
	Long leadingbit = findHighestSetBit(b); // Heighest set bit
	while(leadingbit > 0){ //while there are bits left
		result = ((result*result)%c); //case 1: bit is a 0
		if((b & leadingbit) > 0){
			result = ((result*a)%c); //case 2: if bit is a 1
		}
		leadingbit = leadingbit>>1;
	}
	return (int)result;
}

Long findHighestSetBit(Long num){
	Long result = 0;
	for(int i = 0; i < 64; i++){
		if(num & (1ULL << i)){
			result = 1ULL << i;
		}
	}
	return result;
}

byte* intToByteArray(int num){
	char *result = new char[4];
	for(int i = 0; i < 4; i++){
		result[i] = (num & (0xFF << (8 *(3-i)))) >> (8 *(3-i));
	}
	return result;
}

string intToBinary(int num){
	string result = "";
	for(int i = 31; i >=0; i--){
		if(num &(1 << i)){
			result += '1';
		} else {
			result += '0';
		}
	}
	return result;
}

string byteAC(byte *n, int length){
	string result = "";
	for(int i = 0; i < length; i++){
		result += n[i];
		result += " ";
	}
	return result;
}

string byteAS(byte *n, int length){
	string result = "";
	for(int i = 0; i < length; i++){
		result += byteToBinary(n[i])+" ";
	}
	return result;
}

string byteToBinary(byte num){
	string result = "";
	for(int i = 7; i >=0; i--){
		if(num & (1 << i)){
			result += '1';
		} else {
			result += '0';
		}
	}
	return result;
}

