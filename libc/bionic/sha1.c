/*	$NetBSD: sha1.c,v 1.1 2005/12/20 20:29:40 christos Exp $	*/
/*	$OpenBSD: sha1.c,v 1.9 1997/07/23 21:12:32 kstailey Exp $	*/

/*
 * SHA-1 in C
 * By Steve Reid <steve@edmweb.com>
 * 100% Public Domain
 *
 * Test Vectors (from FIPS PUB 180-1)
 * "abc"
 *   A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 * "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 *   84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 * A million repetitions of "a"
 *   34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
 */

#define SHA1HANDSOFF		/* Copies data before messing with it. */

#include <sys/cdefs.h>
#include <sys/types.h>
#include <assert.h>
#include <sha1.h>
#include <string.h>
#include <endian.h>

#if !HAVE_SHA1_H

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/*
 * blk0() and blk() perform the initial expand.
 * I got the idea of expanding during the round function from SSLeay
 */
#if BYTE_ORDER == LITTLE_ENDIAN
# define blk0(i) swap32(block->l[i])
#else
# define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/*
 * (R0+R1), R2, R3, R4 are the different operations (rounds) used in SHA1
 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef union {
    uint8_t c[SHA1_BLOCK_SIZE];
    uint32_t l[SHA1_BLOCK_SIZE/4];
} CHAR64LONG16;

/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */
void SHA1Transform(uint32_t state[SHA1_DIGEST_LENGTH/4],
                   const uint8_t buffer[SHA1_BLOCK_SIZE])
{
    uint32_t a, b, c, d, e;
    CHAR64LONG16 *block;

#ifdef SHA1HANDSOFF
    CHAR64LONG16 workspace;
#endif

    assert(buffer != 0);
    assert(state != 0);

#ifdef SHA1HANDSOFF
    block = &workspace;
    (void)memcpy(block, buffer, SHA1_BLOCK_SIZE);
#else
    block = (CHAR64LONG16 *)(void *)buffer;
#endif

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    /* Wipe variables */
    a = b = c = d = e = 0;
}


/*
 * SHA1Init - Initialize new context
 */
void SHA1Init(SHA1_CTX *context)
{
    assert(context != 0);

    /* SHA1 initialization constants */
    *context = (SHA1_CTX) {
        .state = {
            0x67452301,
            0xEFCDAB89,
            0x98BADCFE,
            0x10325476,
            0xC3D2E1F0,
        },
        .count = 0,
    };
}


/*
 * Run your data through this.
 */
void SHA1Update(SHA1_CTX *context, const uint8_t *data, unsigned int len)
{
    unsigned int i, j;
    unsigned int partial, done;
    const uint8_t *src;

    assert(context != 0);
    assert(data != 0);

    partial = context->count % SHA1_BLOCK_SIZE;
    context->count += len;
    done = 0;
    src = data;

    if ((partial + len) >= SHA1_BLOCK_SIZE) {
        if (partial) {
            done = -partial;
            memcpy(context->buffer + partial, data, done + SHA1_BLOCK_SIZE);
            src = context->buffer;
        }
        do {
            SHA1Transform(context->state, src);
            done += SHA1_BLOCK_SIZE;
            src = data + done;
        } while (done + SHA1_BLOCK_SIZE <= len);
        partial = 0;
    }
    memcpy(context->buffer + partial, src, len - done);
}


/*
 * Add padding and return the message digest.
 */
void SHA1Final(uint8_t digest[SHA1_DIGEST_LENGTH], SHA1_CTX *context)
{
    uint32_t i, index, pad_len;
    uint64_t bits;
    static const uint8_t padding[SHA1_BLOCK_SIZE] = { 0x80, };

    assert(digest != 0);
    assert(context != 0);

#if BYTE_ORDER == LITTLE_ENDIAN
    bits = swap64(context->count << 3);
#else
    bits = context->count << 3;
#endif

    /* Pad out to 56 mod 64 */
    index = context->count & 0x3f;
    pad_len = (index < 56) ? (56 - index) : ((64 + 56) - index);
    SHA1Update(context, padding, pad_len);

    /* Append length */
    SHA1Update(context, (const uint8_t *)&bits, sizeof(bits));

    if (digest) {
        for (i = 0; i < SHA1_DIGEST_LENGTH/4; i++)
#if BYTE_ORDER == LITTLE_ENDIAN
            ((uint32_t *)digest)[i] = swap32(context->state[i]);
#else
            ((uint32_t *)digest)[i] = context->state[i];
#endif
    }
}

#endif /* HAVE_SHA1_H */
