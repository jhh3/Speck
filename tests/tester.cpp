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

TEST(Speck, Encrypt) {
	uint64_t plaintext = 0xdeadbeef;
	CHECK(plaintext == sc->decrypt(sc->encrypt(plaintext)));
}

int main(int ac, char** av) {
	return CommandLineTestRunner::RunAllTests(ac, av);
}
