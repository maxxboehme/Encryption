/*
 * RSA.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Maxx Boehme
 */
#include <iostream>
#include <ctime>
#include <cstring>
#include <fstream>

#include "rsaencryption.h"

using namespace std;

int main(int argc, char *args[]){
	if(argc > 1 && argc <= 4){
		if(!strcmp(args[1], "k")){
			unsigned long long *keys = new unsigned long long[3];
			int result = generateKeys(keys);
			if(result){
				cerr << "Problem generating keys" << endl;
			} else {
				if(argc > 2){
					ofstream keyStream;
					keyStream.open(args[2]);
					for(int i = 0; i < 3; i++){
						keyStream << keys[i] << " ";
					}
					keyStream.close();
				} else {
					for(int i = 0; i < 3; i++){
						cout << keys[i] << " ";
					}
					cout << endl;
				}
			}
			delete[] keys;
		}
	} else if(argc > 4){
		if(!strcmp(args[1], "e")){
			cout << "encrypting" << endl;
			if(encryption(args[2], args[3], args[4])){
				cerr << "Error Encrypting File" << endl;
			}
		} else if(!strcmp(args[1], "d")){
			cout << "decrypting" << endl;
			if(decryption(args[2], args[3], args[4])){
				cerr << "Error Decrypting File" << endl;
			}
		} else {
			cerr << "Do not know command entered: " << args[1] << endl;
		}
	} else {
		cerr << "Did not enter correct number of argumensts: 5"<< endl;
	}
	return 0;
}

