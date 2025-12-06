// Erik Ecklund
// CMSC 370
// eecklund@live.esu.edu
// 30 Nov 2025
// Purpose: Implement DES Key Expansion

#include "KeyExpansion.h"
#include <iostream>
#include <bitset>
#include <string>
#include <random>
#include <chrono>
using namespace std;

int main() {
	ullong64 secretkey = 1ULL << 20;
	// if secretkey includes parity bits, increment them.
	while (secretkey & paritybits) secretkey += secretkey & paritybits;
	cout << "0x" << hex << uppercase << setw(16) << right << setfill('0') << secretkey << endl;
	ullong64 plaintext = 0;
	ullong64 vplaintext = 255;
	ullong64 ciphertext = DESEncrypt(plaintext, bitset<64>(secretkey));
	ullong64 verification = DESEncrypt(vplaintext, bitset<64>(secretkey));
	ullong64 foundkey = 0;
	auto start = chrono::high_resolution_clock::now();
	while (true) {
		// skip keys with parity bits set to reduce search space
		while(foundkey & paritybits) foundkey += foundkey & paritybits;
		ullong64 encryptedtext = DESEncrypt(plaintext, bitset<64>(foundkey));
		if (encryptedtext == ciphertext) break;
		foundkey++;
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "Found Key: 0x" << hex << uppercase << setw(16) << right << setfill('0') << foundkey;
	cout << "Time Taken: " << chrono::duration<double>(end - start).count() << " seconds.";
	return 0;
}


/*
	string keyString = "";
	cout << "Enter Key? [y/n] ";
	char answer;
	cin >> answer;
	if (toupper(answer) != 'Y')
		keyString = "0000000100100011010001010110011110001001101010111100110111101111";
	else
	{
		cout << "Enter a 64 bit key in binary: ";
		cin >> keyString;
	}
	//KeyExpansion(keyString, false);
	cout << endl;
	KeyExpansion(keyString, true);

	bitset<64> keybits(keyString);

	//KeyExpansion(keybits, false);
	cout << endl;
	KeyExpansion(keybits, true);

	return 0;
}
