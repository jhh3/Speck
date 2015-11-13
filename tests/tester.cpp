#include <iostream>
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
	uint32_t plaintext = 0x6574694c;
	uint32_t ciphertext = 0xa86842f2;
	CHECK(ciphertext == sc->encrypt(plaintext));
}

TEST(Speck, Decrypt_Speck32_64) {
	uint32_t plaintext = 0x6574694c;
	uint32_t ciphertext = 0xa86842f2;
	CHECK(plaintext == sc->decrypt(ciphertext));
}

TEST(Speck, STR_Speck32_64) {
	const char* plaintext = "This is a test. This is a big test.";
	int len = strlen(plaintext);
	char encrypted_str[60];
	char decrypted_str[60];

	sc->encrypt_str(plaintext, len, encrypted_str);
	sc->decrypt_str(encrypted_str, len, decrypted_str);

	STRCMP_EQUAL(plaintext, decrypted_str);
}

int main(int ac, char** av) {
	return CommandLineTestRunner::RunAllTests(ac, av);
}
