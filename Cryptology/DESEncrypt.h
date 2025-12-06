#pragma once

#include "DESEncrypt.h"
#include "KeyExpansion.h"
#include <bitset>

using namespace std;
typedef uint64_t ullong32;

ullong64 DESEncrypt(ullong64 block, bitset<64> key);
ullong48 expand(ullong32 half);
ullong32 f_function(ullong32 half, ullong48 subkey);
ullong64 Scramble(ullong64 block);
ullong64 Unscramble(ullong64 block);
ullong32 Substitution(ullong48 input);
ullong32 Permutation(ullong32 half);
