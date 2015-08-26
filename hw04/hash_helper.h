/**
 * File: hash_helper.h
 * Author: Milan Fabian <milan.fabian@mail.muni.cz>
 * Date: 2.1.2015
 */

#ifndef HASH_HELPER_H
#define HASH_HELPER_H


#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

/**
 * Struct for holding current CRC-16 result. After finishing, value of
 * crc16_context.crc is valid CRC-16 hash.
 */
typedef struct {
	unsigned short crc;
} crc16_context;

/**
 * Initialize crc16_context, must be called before calling crc16_update.
 */
void crc16_init(crc16_context *context);

/**
 * Updates crc16_context with one byte of data.
 */
void crc16_update(crc16_context *context, unsigned char data);

/**
 * Function conducts xor operation on the file supplied
 */
unsigned char xor_hash(char* file, unsigned size);

/**
 * Additional function for crc16 that uses other function to create hash from file supplied
 */
unsigned short crc_16(char* file, unsigned size);

/**
 * Function that reverses number
 */
unsigned reverse(unsigned x);

/**
 * Creates crc32 hash from the file
 */
unsigned crc_32(char* file, unsigned size);

typedef unsigned int MD5_u32plus;

/**
 * Struct for holding current MD5 context.
 */
typedef struct {
	MD5_u32plus lo, hi;
	MD5_u32plus a, b, c, d;
	unsigned char buffer[64];
	MD5_u32plus block[16];
} MD5_CTX;
 
/**
 * Initialize MD5_CTX, must be called before calling MD5_Update.
 */
void MD5_Init(MD5_CTX *ctx);

/**
 *  Updates MD5_CTX with one data of length size.
 */
void MD5_Update(MD5_CTX *ctx, const void *data, unsigned long size);

/**
 * Calculates result of MD5 and stores it into result array of length 16.
 */
void MD5_Final(unsigned char *result, MD5_CTX *ctx);
 
void printMD5(char* string, int length);

#endif