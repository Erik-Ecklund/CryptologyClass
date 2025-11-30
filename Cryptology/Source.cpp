// Erik Ecklund
// CMSC 370
// eecklund@live.esu.edu
// 30 Nov 2025
// Purpose: Implement DES Key Expansion

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;
typedef string k64, k48, b56, b28, h64, h48;

int PC1[] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };
int PC2[] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
int ShiftSched[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

b56 PermutedChoice1(k64 key);
k48 PermutedChoice2(b56 cd);
b28 Shift(b28 bin, int round);
h48 BinToHex(k48 subkey);
void KeyExpansion(k64 key, bool hex);

int main() {
	k64 key = "";
	cout << "Enter Key? [y/N] ";
	char answer;
	cin >> answer;
	if (toupper(answer) != 'Y')
		key = "0000000100100011010001010110011110001001101010111100110111101111";
	else
	{
		cout << "Enter a 64 bit key in binary: ";
		cin >> key;
	}
	KeyExpansion(key, false);
	cout << endl;
	KeyExpansion(key, true);

	return 0;
}

void KeyExpansion(k64 key, bool hex) {
	b56 c0d0 = PermutedChoice1(key);
	b28 c = c0d0.substr(0, 28);
	b28 d = c0d0.substr(28, 28);
	for (int round = 0; round < 16; round++) {
		c = Shift(c, round);
		d = Shift(d, round);
		b56 cd = c + d;
		string subkey = PermutedChoice2(cd);
		if (hex) {
			subkey = BinToHex(subkey);
		}
		cout << "K" << setw(2) << left << round + 1 << " = " << subkey << endl;
	}
}

b56 PermutedChoice1(k64 key) {
	b56 output_c0d0 = "";
	for (int i = 0; i < size(PC1); i++)
		output_c0d0 += key[PC1[i]-1];
	return output_c0d0;
}

k48 PermutedChoice2(b56 cd) {
	k48 subkey = "";
	for (int i = 0; i < size(PC2); i++)
		subkey += cd[PC2[i] - 1];
	return subkey ;
}

b28 Shift(b28 bin, int round) {
	bin += bin[0];
	if (ShiftSched[round] == 2) {
		bin += bin[1];
	}
	bin.erase(0, ShiftSched[round]);
	return bin;
}

h48 BinToHex(k48 subkey) {
	stringstream ss;
	for (int i = 0; i < subkey.size(); i += 4) {
		string nibble = subkey.substr(i, 4);
		int value = stoi(nibble, nullptr, 2);
		ss << uppercase << hex << value;
	}
	return ss.str();
}