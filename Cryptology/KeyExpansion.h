#pragma once
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;
typedef string k64, k48, b56, b28, h64, h48;
typedef uint64_t ullong, ullong64, ullong56, ullong48, ullong28;

// string functions
void KeyExpansion(k64 key, bool hex);
b56 PermutedChoice1(k64 key);
k48 PermutedChoice2(b56 cd);
b28 Shift(b28 bin, int round);
h48 BinToHex(k48 subkey);

// binary functions
void KeyExpansion(ullong48 subkeys[16], bitset<64> key, bool printHex = false, bool printBin = false);
ullong56 PermutedChoice1(ullong64 key);
ullong48 PermutedChoice2(ullong56 cd);
ullong48 Shift(ullong48 cd, int round);
ullong28 RotateLeft(ullong28 input, int shift);