// John Holliman
#ifndef _SPECK_H_
#define _SPECK_H_

#include <stdint.h>

#define BLOCK_SIZE 32
#define WORD_SIZE (BLOCK_SIZE >> 1)
#define KEY_SIZE 64
#define KEY_MASK (0xFFFFFFFFFFFFFFFF)
#define ROUNDS 22
#define L_SCHEDULE_SIZE (ROUNDS + (KEY_SIZE / WORD_SIZE))
#define BETA_SHIFT 2
#define ALPHA_SHIFT 7

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
		void generate_key_schedule();
		RoundResult encrypt_round(uint64_t x, uint64_t y, uint64_t k);
		RoundResult decrypt_round(uint64_t x, uint64_t y, uint64_t k);

		uint64_t block_size;
		uint64_t word_size;
		uint64_t rounds;
		uint64_t key;
		uint64_t key_size;
		uint64_t key_schedule[ROUNDS];
		uint64_t beta_shift;
		uint64_t alpha_shift;
		uint64_t mod_mask;
};

#endif
