#include "KeyExpansion.h"

void KeyExpansion(ullong48 subkeys[16], bitset<64> key, bool printHex, bool printBin) {
	ullong56 cd = PermutedChoice1(key.to_ullong());
	for (int round = 0; round < 16; round++) {
		cd = Shift(cd, round);
		ullong subkey = PermutedChoice2(cd);
		subkeys[round] = subkey;
	}
	// Print subkeys
	for (int i = 0; i < 16; i++) {
		if (printHex)
			cout << "K" << dec << setfill(' ') << setw(2) << left << i + 1 << " = 0x"
			<< hex << uppercase << setw(12) << right << setfill('0') << subkeys[i] << endl;
	}
	for (int i = 0; i < 16; i++) {
		if (printBin)
			cout << "K" << dec << setfill(' ') << setw(2) << left << i + 1 << " = "
			<< bitset<48>(subkeys[i]) << endl;
	}
}


ullong56 PermutedChoice1(ullong64 key) {
	static constexpr int PC1[] = { 
		57, 49, 41, 33, 25, 17, 9, 
		1, 58, 50, 42, 34, 26, 18, 
		10, 2, 59, 51, 43, 35, 27, 
		19, 11, 3, 60, 52, 44, 36, 
		63, 55, 47, 39, 31, 23, 15, 
		7, 62, 54, 46, 38, 30, 22, 
		14, 6, 61, 53, 45, 37, 29, 
		21, 13, 5, 28, 20, 12, 4 
	};
	ullong56 output_c0d0 = 0;
	for (int i = 0; i < size(PC1); i++)
		if (key >> (64 - PC1[i]) & 1ULL)
			output_c0d0 |= (1ULL << (55 - i));
	return output_c0d0;
}


ullong48 PermutedChoice2(ullong56 cd) {
	static constexpr int PC2[] = { 
		14, 17, 11, 24, 1, 5, 3, 28, 
		15, 6, 21, 10, 23, 19, 12, 4, 
		26, 8, 16, 7, 27, 20, 13, 2, 
		41, 52, 31, 37, 47, 55, 30, 40, 
		51, 45, 33, 48, 44, 49, 39, 56, 
		34, 53, 46, 42, 50, 36, 29, 32 
	};
	ullong48 subkey = 0;
	for (int i = 0; i < size(PC2); i++)
		if ((cd >> (56 - PC2[i])) & 1ULL)
			subkey |= (1ULL << (47 - i));
	return subkey;
}

ullong56 Shift(ullong56 cd, int round) {
	static constexpr int ShiftSched[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	ullong28 c = (cd >> 28) & 0xFFFFFFF;
	ullong28 d = cd & 0xFFFFFFF;
	int shift = ShiftSched[round];
	c = RotateLeft(c, shift);
	d = RotateLeft(d, shift);
	return (c << 28) | d;
}

ullong28 RotateLeft(ullong28 input, int shift) {
	return ((input << shift) | (input >> (28 - shift))) & 0xFFFFFFF;
}


// string functions
void KeyExpansion(k64 key, bool doHex) {
	b56 c0d0 = PermutedChoice1(key);
	b28 c = c0d0.substr(0, 28);
	b28 d = c0d0.substr(28, 28);
	for (int round = 0; round < 16; round++) {
		c = Shift(c, round);
		d = Shift(d, round);
		b56 cd = c + d;
		string subkey = PermutedChoice2(cd);
		if (doHex) {
			subkey = BinToHex(subkey);
		}
		cout << "K" << setw(2) << left << round + 1 << " = " << subkey << endl;
	}
}

b56 PermutedChoice1(k64 key) {
	static constexpr int PC1[] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4
	};
	b56 output_c0d0 = "";
	for (int i = 0; i < size(PC1); i++)
		output_c0d0 += key[PC1[i] - 1];
	return output_c0d0;
}

k48 PermutedChoice2(b56 cd) {
	static constexpr int PC2[] = {
		14, 17, 11, 24, 1, 5, 3, 28,
		15, 6, 21, 10, 23, 19, 12, 4,
		26, 8, 16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55, 30, 40,
		51, 45, 33, 48, 44, 49, 39, 56,
		34, 53, 46, 42, 50, 36, 29, 32
	};
	k48 subkey = "";
	for (int i = 0; i < size(PC2); i++)
		subkey += cd[PC2[i] - 1];
	return subkey;
}

b28 Shift(b28 bin, int round) {
	static constexpr int ShiftSched[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
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