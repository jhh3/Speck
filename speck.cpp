// John Holliman
#include "speck.h"
#include <iostream>

SpeckCipher::SpeckCipher(uint64_t _key) {
	// Setup block/word size
	block_size = 32;
	word_size = block_size >> 1;

	// Setup number of rounds and key size
	key_size = 64;
	rounds = 22;

	// Create mask to truncate addition and left shift outputs
	mod_mask = (1ull << word_size) - 1;

	// Setup circular shift parameters
	beta_shift = 2;
	alpha_shift = 7;

	// Parse key and truncate
	uint64_t key_mask = 0xFFFFFFFFFFFFFFFF;
	key = _key & key_mask;

	// Pre-compile key schedule
	generate_key_schedule();
};

void SpeckCipher::generate_key_schedule() {
	key_schedule.push_back(key & mod_mask);
	std::vector<uint64_t> l_schedule;
	for (uint64_t i = 1; i < key_size / word_size; ++i) {
		l_schedule.push_back((key >> (i * word_size)) & mod_mask);
	}

	for (uint64_t i = 0; i < rounds - 1; ++i) {
		RoundResult new_l_k = encrypt_round(l_schedule[i], key_schedule[i], i);
		l_schedule.push_back(new_l_k.a);
		key_schedule.push_back(new_l_k.b);
	}
};

uint64_t SpeckCipher::encrypt(uint64_t plaintext) {
	RoundResult rs;
	rs.a = (plaintext >> word_size) & mod_mask;
	rs.b = plaintext & mod_mask;
	for (size_t i = 0; i < key_schedule.size(); ++i) {
		rs = encrypt_round(rs.a, rs.b, key_schedule[i]);
	}
	return (rs.a << word_size) + rs.b;
}

uint64_t SpeckCipher::decrypt(uint64_t ciphertext) {
	RoundResult rs;
	rs.a = (ciphertext >> word_size) & mod_mask;
	rs.b = ciphertext & mod_mask;
	for (int i = key_schedule.size() - 1; i >= 0; --i) {
		rs = decrypt_round(rs.a, rs.b, key_schedule[i]);
	}
	return (rs.a << word_size) + rs.b;
}

RoundResult SpeckCipher::encrypt_round(uint64_t x, uint64_t y, uint64_t k) {
	uint64_t rs_x = ((x << (word_size - alpha_shift)) + (x >> alpha_shift)) & mod_mask;
	uint64_t ls_y = ((y >> (word_size - beta_shift)) + (y << beta_shift)) & mod_mask;
	uint64_t add_sxy = (rs_x + y) & mod_mask;
	RoundResult rs;
	rs.a = k ^ add_sxy;
	rs.b = rs.a ^ ls_y;
	return rs;
}

RoundResult SpeckCipher::decrypt_round(uint64_t x, uint64_t y, uint64_t k) {
	uint64_t xor_xy = x ^ y;
	uint64_t xor_xk = x ^ k;
	uint64_t new_y = ((xor_xy << (word_size - beta_shift)) + (xor_xy >> beta_shift)) & mod_mask;
	uint64_t msub = xor_xk >= new_y ? xor_xk - new_y : ((xor_xk - new_y) % mod_mask);
	uint64_t new_x = ((msub >> (word_size - alpha_shift)) + (msub << alpha_shift)) & mod_mask;
	return (RoundResult) { new_x, new_y };
}

int main(void) {
	std::cout << "Starting Speck\n";

	uint64_t key = 0x1918111009080100;
	uint64_t plaintext = 0x6574694c;
	plaintext = 0xdeadbeef;

	SpeckCipher sc(key);

	std::cout << "Plaintext " << std::hex << plaintext << "\n";
	uint64_t en = sc.encrypt(plaintext);
	std::cout << "Ciphertext " << std::hex << en << "\n";
	uint64_t de = sc.decrypt(en);
	std::cout << "Decrypted plaintext " << std::hex << de << "\n";

	std::cout << "Ending Speck\n";
	return 0;
}

