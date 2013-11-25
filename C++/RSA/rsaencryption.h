/*
 * rsaencryption.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Maxx Boehme
 */

#ifndef RSAENCRYPTION_H_
#define RSAENCRYPTION_H_

int encryption(std::string, std::string, std::string);
int decryption(std::string, std::string, std::string);

int key(unsigned long long , unsigned long long , unsigned long long *);
int generateKeys(unsigned long long *);

#endif /* RSAENCRYPTION_H_ */
