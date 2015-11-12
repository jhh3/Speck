#include "speck.h"
#include <iostream>

int main(void) {
	std::cout << "Starting Speck\n";

	uint64_t key = 0x1918111009080100;
	uint64_t plaintext = 0x6574694c;

	SpeckCipher sc(key);

	std::cout << "Plaintext " << std::hex << plaintext << "\n";
	uint64_t ciphertext = sc.encrypt(plaintext);
	std::cout << "Ciphertext " << std::hex << ciphertext << "\n";
	uint64_t de = sc.decrypt(ciphertext);
	std::cout << "Decrypted plaintext " << std::hex << de << "\n";

	std::cout << "Ending Speck\n";
	return 0;
}


