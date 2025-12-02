// Erik Ecklund
// CMSC 370
// eecklund@live.esu.edu
// 30 Nov 2025
// Purpose: Implement DES Key Expansion

#include "KeyExpansion.h"
#include <iostream>
#include <bitset>
#include <string>
using namespace std;

int main() {
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
