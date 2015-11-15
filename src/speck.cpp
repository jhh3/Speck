// John Holliman
#include "speck.h"

SpeckCipher::SpeckCipher(uint64_t _key) {
	// Parse key and truncate
	key = _key & KEY_MASK;

	// Pre-compile key schedule
	key_schedule[0] = key & MOD_MASK;
	uint64_t l_schedule[L_SCHEDULE_SIZE];
	for (uint64_t i = 1; i < KEY_SIZE / WORD_SIZE; ++i) {
		l_schedule[i - 1] = ((key >> (i * WORD_SIZE)) & MOD_MASK);
	}

	for (uint64_t i = 0; i < ROUNDS - 1; ++i) {
		RoundResult new_l_k = encrypt_round(l_schedule[i], key_schedule[i], i);
		l_schedule[(KEY_SIZE / WORD_SIZE) + i - 1] = new_l_k.a;
		key_schedule[i + 1] = new_l_k.b;
	}
};

void SpeckCipher::encrypt_str(const char* in, int len, char* out) {
	int n_blocks = N_BLOCKS(len);
	encrypt_blocks((block32_t*) in, n_blocks, (block32_t*) out);
}

void SpeckCipher::decrypt_str(const char* in, int len, char* out) {
	int n_blocks = N_BLOCKS(len);
	decrypt_blocks((block32_t*) in, n_blocks, (block32_t*) out);
}

void SpeckCipher::encrypt_blocks(block32_t* in_blocks, int n_blocks, block32_t* out_blocks) {
	for (int i = 0; i < n_blocks; ++i) {
		out_blocks[i].block = encrypt(in_blocks[i].block);
	}
}

void SpeckCipher::decrypt_blocks(block32_t* in_blocks, int n_blocks, block32_t* out_blocks) {
	for (int i = 0; i < n_blocks; ++i) {
		out_blocks[i].block = decrypt(in_blocks[i].block);
	}
}

uint32_t SpeckCipher::encrypt(uint32_t plaintext) {
	RoundResult rs;
	rs.a = (((uint64_t) plaintext) >> WORD_SIZE) & MOD_MASK;
	rs.b = ((uint64_t) plaintext) & MOD_MASK;

// Reduce binary size
// Decide MODE at compile time
#if ECB
	for (int i = 0; i < ROUNDS; ++i) {
		rs = encrypt_round(rs.a, rs.b, key_schedule[i]);
	}
	return (uint32_t) ((rs.a << WORD_SIZE) + rs.b);
#elif CTR
	//TODO
#elif CTR
	//TODO
#elif CBC
	//TODO
#elif PCBC
	//TODO
#elif CFB
	//TODO
#elif OFB
	//TODO
#else
	exit();
#endif
}

uint32_t SpeckCipher::decrypt(uint32_t ciphertext) {
	RoundResult rs;
	rs.a = (((uint64_t) ciphertext) >> WORD_SIZE) & MOD_MASK;
	rs.b = ((uint64_t) ciphertext) & MOD_MASK;

// Reduce binary size
// Decide MODE at compile time
#if ECB
	for (int i = ROUNDS - 1; i >= 0; --i) {
		rs = decrypt_round(rs.a, rs.b, key_schedule[i]);
	}
	return (uint32_t) ((rs.a << WORD_SIZE) + rs.b);
#elif CTR
	//TODO
#elif CTR
	//TODO
#elif CBC
	//TODO
#elif PCBC
	//TODO
#elif CFB
	//TODO
#elif OFB
	//TODO
#else
	exit();
#endif
}

RoundResult SpeckCipher::encrypt_round(uint64_t x, uint64_t y, uint64_t k) {
	uint64_t rs_x = ((x << (WORD_SIZE - ALPHA_SHIFT)) + (x >> ALPHA_SHIFT)) & MOD_MASK;
	uint64_t ls_y = ((y >> (WORD_SIZE - BETA_SHIFT)) + (y << BETA_SHIFT)) & MOD_MASK;
	uint64_t add_sxy = (rs_x + y) & MOD_MASK;
	RoundResult rs;
	rs.a = k ^ add_sxy;
	rs.b = rs.a ^ ls_y;
	return rs;
}

RoundResult SpeckCipher::decrypt_round(uint64_t x, uint64_t y, uint64_t k) {
	uint64_t xor_xy = x ^ y;
	uint64_t xor_xk = x ^ k;
	uint64_t new_y = ((xor_xy << (WORD_SIZE - BETA_SHIFT)) + (xor_xy >> BETA_SHIFT)) & MOD_MASK;
	uint64_t msub = xor_xk >= new_y ? xor_xk - new_y : ((xor_xk - new_y) % MOD_MASK);
	uint64_t new_x = ((msub >> (WORD_SIZE - ALPHA_SHIFT)) + (msub << ALPHA_SHIFT)) & MOD_MASK;
	return (RoundResult) { new_x, new_y };
}

