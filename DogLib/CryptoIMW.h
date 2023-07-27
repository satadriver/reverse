// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(RB_CRYPTO_H__CDE032E3_2008_0221_1000_FC5A76F1B9BF__INCLUDED_)
#define RB_CRYPTO_H__CDE032E3_2008_0221_1000_FC5A76F1B9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
extern "C" {
#endif
#define MAX_RSA_MODULUS_BITS 1024
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)

typedef struct {
  unsigned short int bits;                     /* length in bits of modulus */
  unsigned char modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
  unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* public exponent */
} R_RSA_PUBLIC_KEY;

typedef struct {
  unsigned short int bits;                     /* length in bits of modulus */
  unsigned char modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
  unsigned char publicExponent[MAX_RSA_MODULUS_LEN];     /* public exponent */
  unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* private exponent */
  unsigned char prime[2][MAX_RSA_PRIME_LEN];   /* prime factors */
  unsigned char primeExponent[2][MAX_RSA_PRIME_LEN];     /* exponents for CRT */
  unsigned char coefficient[MAX_RSA_PRIME_LEN];          /* CRT coefficient */
} R_RSA_PRIVATE_KEY;

// int GenKey(char* filename, R_RSA_PUBLIC_KEY * publicKey, R_RSA_PRIVATE_KEY * privateKey);
int LodeKeys(char* filename,R_RSA_PUBLIC_KEY * publicKey,R_RSA_PRIVATE_KEY * privateKey);
int LoadPublicKey(unsigned int useDefined, R_RSA_PUBLIC_KEY * publicKey);
int LoadPrivateKey(unsigned char *keyBuffer, R_RSA_PRIVATE_KEY * privateKey);

int PublicEncrypt (char * output, unsigned int * outputLen, const char * input, unsigned int inputLen,    R_RSA_PUBLIC_KEY *);
int PrivateEncrypt (char * output, unsigned int * outputLen, const char * input, unsigned int inputLen,    R_RSA_PRIVATE_KEY *);
int PublicDecrypt (char * output, unsigned int * outputLen, const char * input, unsigned int inputLen,    R_RSA_PUBLIC_KEY *);
int PrivateDecrypt (char * output, unsigned int * outputLen, const char * input, unsigned int inputLen,    R_RSA_PRIVATE_KEY *);

#ifdef __cplusplus
}
#endif

#endif // !defined(RB_CRYPTO_H__CDE032E3_2008_0221_1000_FC5A76F1B9BF__INCLUDED_)
