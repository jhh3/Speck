// John Holliman
#ifndef _SPECK_H_
#define _SPECK_H_

#include <stdint.h>
#include <vector>

struct RoundResult {
	uint64_t a;
	uint64_t b;
};

class SpeckCipher {
	public:
		SpeckCipher(uint64_t _key);

		uint64_t encrypt(uint64_t plaintext);
		uint64_t decrypt(uint64_t ciphertext);
	private:
		RoundResult encrypt_round(uint64_t x, uint64_t y, uint64_t k);
		RoundResult decrypt_round(uint64_t x, uint64_t y, uint64_t k);
		void generate_key_schedule();

		uint64_t block_size;
		uint64_t word_size;

		uint64_t rounds;

		uint64_t key;
		uint64_t key_size;
		std::vector<uint64_t> key_schedule;

		uint64_t beta_shift;
		uint64_t alpha_shift;
		uint64_t mod_mask;

		std::vector<std::string> _valid_modes;
};

#endif
