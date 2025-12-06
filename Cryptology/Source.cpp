// Erik Ecklund, Tyler Frailey & Jaryd Rockman
// Dr. Eun-Joo Lee CMSC 370 MW 2:20-4:00 
// eecklund@live.esu.edu, tfrailey1@live.esu.edu, jrockman@live.esu.edu
// Presenting December 8th 2025
// Purpose: 1. Key Schedule Implementation: 
// Program the DES Key Schedule, taking a 64-bit key (including parity bits) and correctly performing:
// Parity Drop/Initial Permutation (PC-1) to get the 56-bit key.
// Splitting into C_0 and D_0 28-bit halves.
// Left Circular Shifts for each of the 16 rounds.
// Compression Permutation (PC-2) to generate the 48-bit subkey K_i for each round i=1 to 16. 
// 2. 
// Output: Print all 16 subkeys in binary and hexadecimal format. 

#include "KeyExpansion.h"
#include "DESEncrypt.h"
#include <iostream>
#include <bitset>
#include <string>
#include <chrono>
using namespace std;

ullong64 paritybits = 0x0101010101010101ULL;

int main() {
	ullong testkey = 0x0123456789ABCDEFULL;
	ullong subkeys[16];
	cout << "Hex Keys" << endl;
	KeyExpansion(subkeys, bitset<64>(testkey), true);
	cout << "Binary Keys" << endl;
	KeyExpansion(subkeys, bitset<64>(testkey), false, true);

	ullong64 secretkey = 1ULL << 20;
	// if secretkey includes parity bits, increment them.
	while (secretkey & paritybits) secretkey += secretkey & paritybits;
	cout << "Secret key = 0x" << hex << uppercase << setw(16) << right << setfill('0') << secretkey << endl;
	ullong64 plaintext = 0;
	ullong64 ciphertext = DESEncrypt(plaintext, bitset<64>(secretkey));
	ullong64 foundkey = 0;
	ullong64 searchcount = 0;
	auto start = chrono::high_resolution_clock::now();
	while (true) {
		// skip keys with parity bits set to reduce search space
		while(foundkey & paritybits) foundkey += foundkey & paritybits;
		ullong64 encryptedtext = DESEncrypt(plaintext, bitset<64>(foundkey));
		searchcount++;
		if (encryptedtext == ciphertext) break;
		foundkey++;
	}
	auto end = chrono::high_resolution_clock::now();
	cout << "Found Key: 0x" << hex << uppercase << setw(16) << right << setfill('0') << foundkey << endl;
	cout << "Time spent: " << chrono::duration<double>(end - start).count() << " seconds." << endl;
	cout << "Keys checked: " << dec << searchcount << endl;
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

	uint64_t subkeys[16] = {};
	//KeyExpansion(subkeys, keybits, false);
	cout << endl;
	KeyExpansion(subkeys, keybits, true);
	*/