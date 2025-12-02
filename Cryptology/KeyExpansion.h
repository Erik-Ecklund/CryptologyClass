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

// string functions
void KeyExpansion(k64 key, bool hex);
b56 PermutedChoice1(k64 key);
k48 PermutedChoice2(b56 cd);
b28 Shift(b28 bin, int round);
h48 BinToHex(k48 subkey);

// binary functions
void KeyExpansion(bitset<64>, bool);
uint64_t PermutedChoice1(uint64_t key);
uint64_t PermutedChoice2(uint64_t cd);
uint64_t Shift(uint64_t cd, int round);
uint64_t RotateLeft(uint64_t input, int shift);