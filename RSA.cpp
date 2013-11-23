/*
 * RSA.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Maxx Boehme
 */
#include <iostream>
#include <ctime>
#include <cstring>

#include "rsaencryption.h"

using namespace std;

int main(int argc, char *args[]){
	if(argc > 1){
		if(!strcmp(args[1], "k")){
			unsigned long long *keys = new unsigned long long[3];
			int result = generateKeys(keys);
			if(result){
				cerr << "Problem generating keys" << endl;
			} else {
				for(int i = 0; i < 3; i++){
					cout << keys[i] << " ";
				}
				cout << endl;
			}
			delete[] keys;
		} else if(!strcmp(args[1], "e")){
			cout << "encrypting" << endl;
			encryption(args[2], args[3], args[4]);
		} else if(!strcmp(args[1], "d")){
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

