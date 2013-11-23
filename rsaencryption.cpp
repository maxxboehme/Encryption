/*
 * rsaencryption.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: Maxx Boehme
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;


typedef unsigned long long ULong;
typedef char byte;

ULong findHighestSetBit(ULong);
unsigned int fastModularExp(ULong, ULong, ULong);
byte* intToByteArray(int);
string intToBinary(int);
string byteToBinary(byte);
string byteAC(byte *, int);
string byteAS(byte *, int);

void encryption(string nameInputFile, string nameKeyFile, string nameOutputFile) {
	ifstream keyStream;
	keyStream.open(nameKeyFile.c_str());
	ULong keys[3];
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
	ULong keys[3];
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

unsigned int fastModularExp(ULong a, ULong b, ULong c){
	ULong result = 1;
	ULong leadingbit = findHighestSetBit(b); // Heighest set bit
	while(leadingbit > 0){ //while there are bits left
		result = ((result*result)%c); //case 1: bit is a 0
		if((b & leadingbit) > 0){
			result = ((result*a)%c); //case 2: if bit is a 1
		}
		leadingbit = leadingbit>>1;
	}
	return (unsigned int)result;
}

ULong findHighestSetBit(ULong num){
	ULong result = 0;
	for(int i = 63; i >= 0; i--){
		if(num & (1ULL << i)){
			result = 1ULL << i;
			return result;
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

/**
 * Generates a large unsignend random number
 */
ULong large_Random(){
	int times = (rand() % 5) + 1;
	ULong random_integer = 1;
	bool done = false;
	for(int i = 0; i < times && !done; i++){
		ULong r = rand();
		if((random_integer * r) != 0){
			random_integer = random_integer * r;
		} else {
			done = true;
		}
	}
	return random_integer;
}

/**
 * Finds the modular inverse using Extended Euclidean Algorithm
 */
long long mod_inv(long long a, long long b){
	long long b0 = b, t, q;
	long long x0 = 0, x1 = 1;

	if(b == 1){
		return 1;
	}
	while(a > 1) {
		q = a / b;
		t = b;
		b = a % b;
		a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}
	if(x1 < 0){
		x1 += b0;
	}
	return x1;
}

/**
 * Notes:
 * 		Words up to 999999999999999989 the highest tested 18 digit prime that it can calculate correctly
 *  	Though it does take ULonger since using the brute force method
 */
bool isPrime(ULong x) {
	if(x == 0){
		return false;
	}
	double root = sqrt(x);

	//quick check to see if root is an int, therefore not prime
	if((ULong)root == root){
		return false;
	}

	//checks all possible factors from 2 to the square root of x
	for(double i = 2; i < root; i++) {
		double check = x / i;
		if((ULong)check == check){
			return false;
		}
	}
	return true;
}

/**
 *  Implementation of the Euclidean Algorithm to find the
 *  Greatest Common Divisor.
 *  Ex.
 *
 *  Note:
 *  	Uses multiplication and division operations but since the Euclidean Algorithm
 *  	diverges quickly does not make much of a difference
 */
ULong gcd(ULong e, ULong theta){
	if(e== 0){
		return theta;
	}
	if(theta == 0){
		return e;
	}

	ULong a1[3];
	a1[0] = e;
	a1[1] = theta;
	a1[3] = 1;
	ULong q = 0;
	ULong result = e;
	while(a1[2]!=0){
		q = a1[0]/a1[1];
		a1[2] = a1[0]-q*a1[1];
		result = a1[1];
		a1[0] = a1[1];
		a1[1] = a1[2];
	}
	return result;
}

/**
 * Interesting way to find the Greatest Common Divisor
 * using just binary operations.
 */
ULong gcd_binary(ULong a, ULong b)
{
	while(b){
		a %= b;
		b ^= a;
		a ^= b;
		b ^= a;
	}
	return a;
}

/**
 * Finds e using RSA techniques.
 *
 * returns an e tha is smaller than n and is supposed to be
 * a random pseudoprime of theta (meaning that gcd(e, theta)=1)
 */
ULong finde(ULong n, ULong theta){
	ULong e = large_Random() % n;
	while(gcd_binary(e, theta) != 1){
		e = large_Random() % n;
		while(e == 0){
			e = large_Random() % n;
		}
	}
	return e;
}

/**
 * Returns a large random prime with then number of digits asked for
 */
ULong generateRandomPrime(int digits){
	if(digits < 1){
		digits = 1;
	}
	ULong lowerLimit = 1 * pow(10, digits-1);
	ULong upperLimit = 1 * pow(10, digits);
	ULong p = rand() % (upperLimit - lowerLimit) + lowerLimit;
	while(!isPrime(p)){
		ULong r = large_Random();
		p = r % (upperLimit - lowerLimit) + lowerLimit;
	}
	return p;
}

ULong generateRandomPrimeRange(int top){
	if(top < 2){
		return 2;
	}
	ULong p = large_Random() % top;
	while(!isPrime(p)){
		ULong r = large_Random();
		p = r % top;
	}
	return p;
}

/**
 * Generates the public and private keys n, e, d given two primes p and q.
 *
 * The array result needs to be of length 3 for this function to work properly.
 *
 *
 */
int key(ULong p, ULong q, ULong *result){   //finds n e and d, then you can use d and e for encryption
	//makes sure both arguments are prime
	if(isPrime(p) && isPrime(q)) {
		ULong n = p*q;
		ULong theta = (p-1)*(q-1);
		ULong e = finde(n, theta);
		ULong d = mod_inv(e, theta);
		result[0] = n;
		result[1] = e;
		result[2] = d;
		return 0;
	}
	else{
		return 1;
	}
}

/**
 * Places the public and private keys n, d, e into the array given.
 */
int generateKeys(ULong *result){
	srand(time(0));
	ULong p = generateRandomPrime((rand() % 4) + 3);
	unsigned int maxq= 4294967295U/p;
	ULong q = generateRandomPrimeRange(maxq);
	return key(p, q, result);
}
