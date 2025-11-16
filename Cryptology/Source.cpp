// Erik Ecklund
// CMSC 370
// eecklund@live.esu.edu
// 16 Nov 2025
// Purpose: Implement S-DES stream modes ECB and CBC, and to demonstrate the avalanche effect with CBC

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <unordered_map>
using namespace std;

const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // ciphertext
const string alphabet_lower = "abcdefghijklmnopqrstuvwxyz"; // plaintext
unordered_map<int, int> inverses = {
		{1, 1}, {3, 9}, {5, 21}, {7, 15}, {9, 3}, {11, 19},
		{15, 7}, {17, 23}, {19, 11}, {21, 5}, {23, 17}, {25, 25}
};
int from_letter(char letter)
{
	return toupper(letter) - 'A';
}
// Row Transposition Functions
// key is 0 terminated array of integers, no repeats, no skipped numbers
string encrypt_row_transposition(string plaintext, int* key);
string decrypt_row_transposition(string ciphertext, int* key);

// Hill Cipher Functions
string encrypt_hill_cipher(string plaintext, int[2][2]);
string decrypt_hill_cipher(string ciphertext, int[2][2]);
void derive_hill_key(string plaintext, string ciphertext, int key[2][2]);
void matrix_mult(int a[2][2], int b[2][2], int result[2][2]);



int main()
{
	// Affine Ciphers
	cout << "Affine Ciphers" << "-------------------------" << endl;
	{
		// 1. Encrypt with 7x + 8
		string plaintext = "cleopatra";
		string ciphertext;
		for (char c : plaintext)
		{
			int p = from_letter(c);
			int C = (7 * p + 8) % 26;
			ciphertext += alphabet[C];
		}
		cout << "1.) Encrypted message: " << ciphertext << endl;
	}
	{
		// 2. Decrypt C = 5p + 12
		// p = (C - 12) * 1/5 (mod 26) = 21 * (C + 14) % 26
		string ciphertext = "MZDVEZC";
		string plaintext;
		for (char c : ciphertext)
		{
			int C = from_letter(c);
			int p = (21 * (C + 14)) % 26;
			plaintext += alphabet_lower[p];
		}
		cout << "2.) Decrypted message: " << plaintext << endl;
		// 3. Brute Force C = 3x + b
	}
	{
		// p = (C - b) * 1/3 mod 26 = 9 * (C - b + 26) % 26 
		// Note: do not % negatives in C++
		cout << "3.) " << endl;
		string ciphertext = "TCABTIQMFHEQQMRMVMTMAQ";
		for (int b = 0; b < 26; b++) {
			string plaintext;
			for (char c : ciphertext)
			{
				int C = from_letter(c);
				int p = 9 * (C + 26 - b) % 26;
				plaintext += alphabet_lower[p];
			}
			cout << "For b = " << b << ", decrypted message: " << plaintext << endl;
		}
		cout << endl;
	}
	{
		// 4. Known Plaintext Attack
		string ciphertext = "EDSGICKXHUKLZVEQZVKXWKZUKCVUH";
		string plaintext = "if";
		// C = alpha * p + beta
		// 'E' = alpha * 'i' + beta
		// 'D' = alpha * 'f' + beta
		// 4 = alpha * 8 + beta
		// 3 = alpha * 5 + beta
		// 1 = alpha * 3 => alpha = 9
		int alpha;
		{
			// Find alpha inverse
			int c1 = from_letter(ciphertext[0]);
			int c2 = from_letter(ciphertext[1]);
			int left = (c1 - c2 + 26) % 26;
			int p1 = from_letter(plaintext[0]);
			int p2 = from_letter(plaintext[1]);
			int coef = (p1 - p2 + 26) % 26;
			alpha = left * inverses[coef] % 26;
		}
		// 4 = 9 * 8 + beta => beta = 4 - 72 = -68 = 10 mod 26
		int beta;
		{
			// Find beta
			int c = from_letter(ciphertext[0]);
			int p = from_letter(plaintext[0]);
			beta = (c - alpha * p + 26) % 26;
		}
		// p = (C - b) / a

		for (int i = 2; i < ciphertext.size(); i++) {
			int p = (from_letter(ciphertext[i]) - beta + 26) * inverses[alpha] % 26;
			plaintext += alphabet_lower[p];
		}
		cout << "4.) Decrypted message: " << plaintext << endl << endl;
	}

	// Vigenere and Transposition Ciphers
	cout << "Vigenere & Transposition Ciphers" << "-------------------------" << endl;
	{
		// Shift cipher
		string ciphertext = "LCLLEWLJAZLNNZMVYIYLHRMHZA";
		int counts[26] = { 0 };
		for (char c : ciphertext)
		{
			int C = from_letter(c);
			counts[C]++;
		}
		int max_count = 0;
		int max_index = 0;
		for (int i = 0; i < 26; i++)
		{
			if (counts[i] > max_count)
			{
				max_count = counts[i];
				max_index = i;
			}
		}
		int shift = (max_index - from_letter('E') + 26) % 26;
		cout << "1. Using a frequency count the key was found to be " << shift << endl;
		cout << "The message was decrypted to: ";
		string plaintext = "";
		for (char c : ciphertext) {
			int C = from_letter(c);
			int p = (C - shift + 26) % 26;
			plaintext += alphabet_lower[p];
		}
		cout << plaintext << endl;
	}
	{
		// Row Transposition
		cout << endl << "2.a" << endl;
		string plaintext = "attackpostponeduntiltwoam";
		int key[] = { 4, 3, 1, 2, 5, 6, 7, 0 };
		string ciphertext_1 = encrypt_row_transposition(plaintext, key);
		string ciphertext = encrypt_row_transposition(ciphertext_1, key);
		cout << "The message was encrypted to: " << ciphertext << endl;
	}
	{
		cout << endl << "2.b" << endl;
		string ciphertext = "AMRTMOEPEAEGRTFYTZTYXAWE";
		int key[] = { 3, 5, 1, 6, 2, 4, 0 };
		string plaintext_1 = decrypt_row_transposition(ciphertext, key);
		string plaintext = decrypt_row_transposition(plaintext_1, key);
		cout << "The message was decrypted to: " << plaintext << endl;
	}
	
	// Hill Ciphers
	cout << endl << "Hill Ciphers" << "-------------------" << endl;
	{
		cout << "3.a" << endl;
		int key[2][2] = { {9, 4}, {5, 7} };
		string plaintext = "meetmeattheusualplaceattenratherthaneightoclock";
		string ciphertext = encrypt_hill_cipher(plaintext, key);
		cout << "Ciphertext: " << ciphertext << endl;
	}
	{
		cout << endl << "3.b" << endl;
		int key[2][2] = { {9, 13}, {2, 3} };
		string ciphertext = "YIFZMA";
		string plaintext = decrypt_hill_cipher(ciphertext, key);
		cout << "Plaintext: " << plaintext << endl;
	}
	{
		cout << endl << "4" << endl;
		string plaintext = "howareyoutoday";
		string ciphertext = "ZWSENIUSPLJVEU";
		int key[2][2];
		derive_hill_key(plaintext, ciphertext, key);
		cout << "Key: " << endl;
		cout << key[0][0] << " " << key[0][1] << endl;
		cout << key[1][0] << " " << key[1][1] << endl;
	}

	return 0;
}

// Row Transposition Functions
// key is 0 terminated array of integers, no repeats, no skipped numbers
string encrypt_row_transposition(string plaintext, int* key) {
	string ciphertext;
	int cols = 0;
	while (key[cols] != 0) cols++;
	string* columns = new string[cols];

	// Padding
	int padding = cols - (plaintext.size() % cols);
	if (padding != cols) {
		plaintext += string(padding, 'x');
	}
	int rows = plaintext.size() / cols;

	// Fill rows
	for (int i = 0; i < plaintext.size(); i++) {
		columns[i % cols] += plaintext[i];
	}
	
	// Rearrange columns in key order
	string* rearranged = new string[cols];
	for (int i = 0; i < cols; i++) {
		rearranged[key[i] - 1] = columns[i];
	}

	// Read columns
	for (int i = 0; i < cols; i++) {
		ciphertext += rearranged[i];
	}
	
	// Uppercase for convention
	for (char& c : ciphertext) c = toupper(c);

	delete[] columns;
	delete[] rearranged;
	return ciphertext;
}

string decrypt_row_transposition(string ciphertext, int* key) {
	string plaintext;
	int cols = 0;
	while (key[cols] != 0) cols++;

	// Reverse key
	// for assignment key, this part is not needed
	int* new_key = new int[cols + 1];
	for (int i = 0; i < cols; i++) {
		new_key[key[i] - 1] = i + 1;
	}
	new_key[cols] = 0;

	// Fill columns
	string* columns = new string[cols];
	int col_length = ciphertext.size() / cols;
	for (int i = 0; i < cols; i++) {
		columns[i] = ciphertext.substr(i * col_length, col_length);
	}
	
	// Rearrange columns in new key order
	string* rearranged = new string[cols];
	for (int i = 0; i < cols; i++) {
		rearranged[new_key[i] - 1] = columns[i];
	}

	// Read columns in new key order
	for (int row = 0; row < col_length; row++) {
		for (int col = 0; col < cols; col++) {
			plaintext += rearranged[col][row];
		}
	}

	// lowercase for convention
	for (char& c : plaintext) c = tolower(c);

	delete[] columns;
	delete[] new_key;
	delete[] rearranged;
	return plaintext;
}

// Hill Cipher Functions
string encrypt_hill_cipher(string plaintext, int key[2][2]) {
	string ciphertext;

	// Padding
	int padding =  4 - plaintext.size() % 4;
	if (padding != 4) {
		plaintext += string(padding, 'x');
	}

	// encrypt 4 letters at a time
	for (int i = 0; i < plaintext.length(); i += 4) {
		int p1 = from_letter(plaintext[i]);
		int p2 = from_letter(plaintext[i + 1]);
		int p3 = from_letter(plaintext[i + 2]);
		int p4 = from_letter(plaintext[i + 3]);

		// Matrix multiplication
		// [[p1 p2]   [[k00 k01]   [[C1 C2]
		//  [p3 p4]] x [k10 k11]] = [C3 C4]]
		int C1 = (key[0][0] * p1 + key[1][0] * p2) % 26;
		int C2 = (key[0][1] * p1 + key[1][1] * p2) % 26;
		int C3 = (key[0][0] * p3 + key[1][0] * p4) % 26;
		int C4 = (key[0][1] * p3 + key[1][1] * p4) % 26;

		ciphertext += alphabet[C1];
		ciphertext += alphabet[C2];
		ciphertext += alphabet[C3];
		ciphertext += alphabet[C4];
	}

	return ciphertext;
}
string decrypt_hill_cipher(string ciphertext, int key[2][2]) {
	string plaintext;

	// Find inverse of key matrix
	int det = (key[0][0] * key[1][1] - key[0][1] * key[1][0]) % 26;
	if (det < 0) det += 26; // det % 26 can be negative in C++
	int det_inv = inverses[det];
	int inv_key[2][2] = {
		{ det_inv * key[1][1] % 26, (-det_inv * key[0][1] + 26) % 26 },
		{ (-det_inv * key[1][0] + 26) % 26, det_inv * key[0][0] % 26 }
	};

	// Padding
	int padding = 4 - ciphertext.size() % 4;
	if (padding != 4) {
		ciphertext += string(padding, 'X');
	}

	// decrypt 4 letters at a time
	for (int i = 0; i < ciphertext.length(); i += 4) {
		int C1 = from_letter(ciphertext[i]);
		int C2 = from_letter(ciphertext[i + 1]);
		int C3 = from_letter(ciphertext[i + 2]);
		int C4 = from_letter(ciphertext[i + 3]);

		// Matrix multiplication
		// [[C1 C2]   [[k00 k01]   [[p1 p2]
		//  [C3 C4]] x [k10 k11]] = [p3 p4]]
		int p1 = (inv_key[0][0] * C1 + inv_key[1][0] * C2) % 26;
		int p2 = (inv_key[0][1] * C1 + inv_key[1][1] * C2) % 26;
		int p3 = (inv_key[0][0] * C3 + inv_key[1][0] * C4) % 26;
		int p4 = (inv_key[0][1] * C3 + inv_key[1][1] * C4) % 26;
		
		plaintext += alphabet_lower[p1];
		plaintext += alphabet_lower[p2];
		plaintext += alphabet_lower[p3];
		plaintext += alphabet_lower[p4];
	}

	// Remove padding
	plaintext = plaintext.substr(0, plaintext.size() - padding);
	// lowercase for convention
	for (char& c : plaintext) c = tolower(c);

	return plaintext;
}

void derive_hill_key(string plaintext, string ciphertext, int key[2][2]) {
	int p1 = from_letter(plaintext[0]);
	int p2 = from_letter(plaintext[1]);
	int C1 = from_letter(ciphertext[0]);
	int C2 = from_letter(ciphertext[1]);

	int pair2idx = 0;
	int det = 0, det_inv;
	int p3, p4, C3, C4;
	do{
		pair2idx += 2;
		p3 = from_letter(plaintext[pair2idx]);
		p4 = from_letter(plaintext[pair2idx + 1]);
		C3 = from_letter(ciphertext[pair2idx]);
		C4 = from_letter(ciphertext[pair2idx + 1]);

		// Inverse of plaintext matrix
		det_inv = (p1 * p4 - p2 * p3) % 26;
		if (det_inv < 0) det_inv += 26;
		if (inverses.find(det_inv) == inverses.end()) continue; // not invertible
		det = inverses[det_inv];
	} while (det == 0);
	int inv_p[2][2] = {
		{ det * p4 % 26, (-det * p2 + 26) % 26 },
		{ (-det * p3 + 26) % 26, det * p1 % 26 }
	};

	// [[C1 C2]   [[inv_p00 inv_p01]    [[k00 k01]
	//  [C3 C4]] x [inv_p10 inv_p11]] =  [k10 k11]]
	// Multiply by ciphertext matrix
		int C_mat[2][2] = { {C1, C2}, {C3, C4} };
		matrix_mult(inv_p, C_mat, key);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			while (key[i][j] < 0) key[i][j] += 26;
			key[i][j] %= 26;
		}
	}
}

void matrix_mult(int a[2][2], int b[2][2], int result[2][2]) {
	result[0][0] = (a[0][0] * b[0][0] + a[0][1] * b[1][0]);
	result[0][1] = (a[0][0] * b[0][1] + a[0][1] * b[1][1]);
	result[1][0] = (a[1][0] * b[0][0] + a[1][1] * b[1][0]);
	result[1][1] = (a[1][0] * b[0][1] + a[1][1] * b[1][1]);
}