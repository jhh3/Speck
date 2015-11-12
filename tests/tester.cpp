#include <iostream>
#include <stdint.h>
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "speck.h"

TEST_GROUP(Speck) {
	SpeckCipher* sc;

	void setup() {
		sc = new SpeckCipher(0x1918111009080100);
	}

	void teardown() {
		delete sc;
	}
};

TEST(Speck, Encrypt_Speck32_64) {
	uint64_t plaintext = 0x6574694c;
	uint64_t ciphertext = 0xa86842f2;
	CHECK(ciphertext == sc->encrypt(plaintext));
}

TEST(Speck, Decrypt_Speck32_64) {
	uint64_t plaintext = 0x6574694c;
	uint64_t ciphertext = 0xa86842f2;
	CHECK(plaintext == sc->decrypt(ciphertext));
}

int main(int ac, char** av) {
	return CommandLineTestRunner::RunAllTests(ac, av);
}
