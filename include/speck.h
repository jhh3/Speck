// Author: John Holliman
//
// Speck is a software optmized lightweight block cipher designed by the NSA.
//
// NSA Simon and Speck paper --> http://eprint.iacr.org/2013/404.pdf
//
// In order to maximize flexibility, speck was designed to support five
// different block sizes and up to threed different key sizes.
//
// block size | key sizes
// ----------------------
//    32      |  64
//    48      |  72, 96
//    64      |  96, 128
//    96      |  96, 144
//    128     |  128, 192, 256
//
// This implementation currently only support a block size of 32 and a key size
// of 64.
//
// In the future, I plan to make it configurable to any desired combination
// give in the above table.

#ifndef _SPECK_H_
#define _SPECK_H_

#define BLOCK_SIZE 32
#define WORD_SIZE (BLOCK_SIZE >> 1)
#define KEY_SIZE 64
#define KEY_MASK (0xFFFFFFFFFFFFFFFF)
#define MOD_MASK ((1ull << WORD_SIZE) - 1)
#define ROUNDS 22
#define L_SCHEDULE_SIZE (ROUNDS + (KEY_SIZE / WORD_SIZE) - 2)
#define BETA_SHIFT 2
#define ALPHA_SHIFT 7

// Modes of operation:
//
// Mode                                    | Value
// ------------------------------------------------
// Electronic codebook (ECB)               |  1
// Counter (CTR)                           |  2
// Cipher block chaining (CBC)             |  3
// Propagting cipher block chaining (PCBC) |  4
// Cipher feedback (CFB)                   |  5
// Output feedbac(OFB)                     |  6
//
// Currently only ECB is supported. In the future, all modes will be supported.

#ifndef MODE
#define MODE (1)
#endif

#define ECB (MODE == 1)
#define CTR (MODE == 2)
#define CBC (MODE == 3)
#define PCBC (MODE == 4)
#define CFB (MODE == 5)
#define OFB (MODE == 6)

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;

#define N_BLOCKS(LEN) (LEN % 4 == 0 ? LEN / 4 : (LEN / 4) + 1);

union block32_t {
	uint32_t block;
	char data[sizeof(uint32_t)];
};

struct RoundResult {
	uint64_t a;
	uint64_t b;
};

class SpeckCipher {
	public:
		SpeckCipher(uint64_t _key);

		uint32_t encrypt(uint32_t plaintext);
		uint32_t decrypt(uint32_t ciphertext);

		void encrypt_str(const char* in, int len, char* out);
		void decrypt_str(const char* in, int len, char* out);
	private:
		// Single rounds of Feistel operation.
		RoundResult encrypt_round(uint64_t x, uint64_t y, uint64_t k);
		RoundResult decrypt_round(uint64_t x, uint64_t y, uint64_t k);

		void encrypt_blocks(block32_t* in_blocks, int n_blocks, block32_t* out_blocks);
		void decrypt_blocks(block32_t* in_blocks, int n_blocks, block32_t* out_blocks);

		uint64_t key;
		uint64_t key_schedule[ROUNDS];
};

#endif
