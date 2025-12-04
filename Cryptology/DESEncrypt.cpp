#include "DESEncrypt.h"
#include "KeyExpansion.h"
#include "DESTables.h"
#include <bitset>

using namespace std;
using namespace DESTables;
typedef uint64_t ullong32;

ullong64 DESEncrypt(ullong64 block, bitset<64> key) {
	block = Scramble(block);
	ullong32 left = block >> 32;
	ullong32 right = block & 0xFFFFFFFF;
	ullong48 subkeys[16] = {};
	KeyExpansion(subkeys, key, false, false);
	for (int round = 0; round < 16; round++) {
		ullong48 subkey = subkeys[round];
		ullong32 newleft = right;
		ullong32 newright = left ^ f_function(right, subkey);
		left = newleft;
		right = newright;
	}
	ullong64 outblock = (right << 32) | left;
	outblock = Unscramble(outblock);
	return outblock;
}

ullong48 expand(ullong32 half) {
	bitset<32> input(half);
	bitset<48> output;
	for (int i = 0; i < size(E); i++)
		output[47 - i] = input[32 - E[i]];
	return output.to_ullong();
}

ullong32 f_function(ullong32 half, ullong48 subkey) {
	ullong48 unsubbed = expand(half) ^ subkey;
	ullong32 subbed = Substitution(unsubbed);
	ullong32 permuted = Permutation(subbed);
	return permuted;
}

ullong64 Scramble(ullong64 block) {
	bitset<64> input(block);
	bitset<64> output;
	for (int i = 0; i < size(IP); i++)
		output[63 - i] = input[64 - IP[i]];
	return output.to_ullong();
}

ullong64 Unscramble(ullong64 block) {
	bitset<64> input(block);
	bitset<64> output;
	for (int i = 0; i < size(FP); i++)
		output[63 - i] = input[64 - FP[i]];
	return output.to_ullong();
}

ullong32 Substitution(ullong48 input) {
	ullong32 output = 0;
	for (int i = 0; i < 8; i++) {
		int shift = 48 - 6 * (i + 1);
		bitset<6>inbox((input >> shift) & 0x3F);
		// note: bitsets are indexed from the right 
		int row = inbox[5] << 1 | inbox[0];
		int col = inbox[4] << 3 | inbox[3] << 2 | inbox[2] << 1 | inbox[1];
		
		int outshift = 32 - 4 * (i + 1);
		output |= SBoxes[i][row][col] << outshift;
	}
	return output;
}

ullong32 Permutation(ullong32 half) {
	bitset<32> input(half);
	bitset<32> output;
	for (int i = 0; i < size(P); i++)
		output[31 - i] = input[32 - P[i]];
	return output.to_ullong();
}