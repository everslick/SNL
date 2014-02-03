#ifndef _BLOWFISH_H_
#define _BLOWFISH_H_

#define MAXKEYBYTES 56 // 448 bits

typedef struct blowfish_t {
	unsigned long P[16 + 2];
	unsigned long S[4][256];
} blowfish_t;

int bf_init(blowfish_t *bf, void *key, int len);
int bf_encrypt(blowfish_t *bf, void *data, int len);
int bf_decrypt(blowfish_t *bf, void *data, int len);

#endif
