// FreeGateDec.cpp: implementation of the FreeGateDec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
#include "httpsession.h"
#include "FreeGateDec.h"
#include "PostStruct.h"
#include "PostSplit.h"
#include "PublicFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
unsigned char g_fixKey[]=
{
	0x40, 0x9B, 0x40, 0x00, 0x70, 0x94, 0x40, 0x00, 0x53, 0x4F, 0x46, 0x54, 0x57, 0x41, 0x52, 0x45,
	0x5C, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x5C, 0x57, 0x69, 0x6E, 0x64, 0x6F,
	0x77, 0x73, 0x5C, 0x43, 0x75, 0x72, 0x72, 0x65, 0x6E, 0x74, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F,
	0x6E, 0x5C, 0x52, 0x75, 0x6E, 0x00, 0x00, 0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
	0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x13, 0x28, 0x99, 0xE8, 0x57, 0x42, 0x1A, 0x3B, 0x98, 0x3E, 0x23, 0xBD, 0xE8, 0xD7, 0xE4, 0x36,
	0xCC, 0x1A, 0x8C, 0xE6, 0x30, 0x3C, 0x5B, 0x24, 0x78, 0x11, 0xCC, 0x9E, 0x7F, 0x91, 0x31, 0x12,
	0xDA, 0x78, 0x5D, 0x0B, 0xD2, 0x96, 0x06, 0xE7, 0x0E, 0xBE, 0xD7, 0x77, 0x66, 0x4A, 0xAB, 0xBE,
	0xA9, 0xE6, 0x22, 0x85, 0x62, 0x6B, 0x18, 0x08, 0x98, 0x7F, 0x07, 0xAD, 0x47, 0x61, 0xC8, 0xA1,
	0xEB, 0x33, 0x31, 0x92, 0x3F, 0x58, 0xC6, 0x2D, 0x6A, 0x48, 0x28, 0x41, 0x7C, 0xDC, 0x64, 0xE8,
	0x3A, 0xFB, 0xDE, 0xFA, 0xD4, 0xF9, 0xB3, 0x3B, 0x75, 0x32, 0x68, 0xD2, 0xE8, 0xE0, 0x60, 0xA2,
	0xCD, 0xC8, 0x2E, 0xC2, 0x4A, 0xFA, 0x10, 0x8D, 0x7E, 0x6F, 0x46, 0x42, 0x32, 0x57, 0xB9, 0x9D,
	0xB5, 0x48, 0x3F, 0x89, 0xD4, 0x68, 0xA3, 0x8F, 0x21, 0xB5, 0x70, 0xE3, 0x5B, 0x4B, 0xF1, 0xA4,
	0x6F, 0xFD, 0xCD, 0xEB, 0xB2, 0x9B, 0xF5, 0x10, 0x02, 0x9D, 0x27, 0xB0, 0x2D, 0x5D, 0x46, 0x3F,
	0x9E, 0xBB, 0x6D, 0xD5, 0xB8, 0xAF, 0x51, 0xBE, 0x58, 0x61, 0xBC, 0x1B, 0xFB, 0x96, 0x2C, 0x1D,
	0xF5, 0x64, 0x78, 0x5B, 0x81, 0x88, 0x2C, 0xA2, 0x04, 0xC7, 0x52, 0x89, 0x66, 0x3F, 0x2B, 0x79,
	0xEA, 0x9D, 0xB6, 0x06, 0x38, 0x33, 0x91, 0xF8, 0xB8, 0x53, 0x93, 0xB5, 0xF9, 0x5C, 0x47, 0xF4,
	0x9E, 0x84, 0xAF, 0xE2, 0xF8, 0x42, 0x73, 0x86, 0x4A, 0x56, 0x45, 0xE8, 0x51, 0x3A, 0x39, 0x1D,
	0x1B, 0x0A, 0x6C, 0x29, 0xF2, 0xBE, 0x4F, 0xC6, 0xFF, 0x91, 0x6B, 0x91, 0x06, 0xAC, 0x7D, 0x5A,
	0xA2, 0x70, 0x09, 0xB1, 0x03, 0x1B, 0x69, 0x96, 0x69, 0x65, 0x85, 0xF5, 0x85, 0x5D, 0xE9, 0x56,
	0x59, 0x10, 0xF9, 0xBD, 0xDA, 0x02, 0x15, 0xAA, 0x41, 0xBB, 0x48, 0x23, 0xDF, 0x7B, 0x83, 0x45,
	0x64, 0xB9, 0x1C, 0x40, 0x9D, 0x98, 0xEA, 0x8D, 0x8A, 0x82, 0xEA, 0xB3, 0xF1, 0x8C, 0x9F, 0xDB,
	0xA8, 0x5F, 0xCB, 0xB1, 0x0F, 0xCE, 0x02, 0x58, 0xCE, 0xD5, 0x7B, 0x8C, 0x4A, 0xF7, 0x51, 0x01,
	0x55, 0xEE, 0x53, 0xFD, 0x8C, 0x14, 0x34, 0xBD, 0x8E, 0x1C, 0x92, 0x1F, 0xDA, 0xFB, 0xA0, 0x48,
	0xF9, 0x7B, 0x8A, 0x17, 0x70, 0x5A, 0x87, 0x81, 0xA7, 0x78, 0x40, 0x59, 0x4B, 0x7E, 0x36, 0x2B,
	0x27, 0xDD, 0x6F, 0xEC, 0xD1, 0xA9, 0x67, 0x97, 0xFA, 0xD1, 0xA1, 0xD1, 0x5C, 0xF3, 0x28, 0xA2,
	0xFB, 0x4F, 0xF6, 0xA6, 0xC1, 0x9A, 0x20, 0xF1, 0x23, 0x94, 0x53, 0xF0, 0x9D, 0x1D, 0xEE, 0x55,
	0x9B, 0xF5, 0x52, 0x58, 0x7B, 0x23, 0x37, 0xC9, 0x55, 0xD7, 0xF9, 0x8D, 0xAF, 0x32, 0x63, 0xFB,
	0xDE, 0x30, 0xF2, 0x5D, 0x4F, 0xA9, 0x3B, 0xC9, 0x90, 0xAA, 0x1B, 0x78, 0x1D, 0x38, 0xAD, 0x40,
	0x36, 0x22, 0x01, 0x35, 0x79, 0xBA, 0x04, 0x59, 0xFE, 0x68, 0x9F, 0x84, 0xB7, 0xBD, 0xA3, 0xF3,
	0x0D, 0x86, 0x06, 0x5D, 0xCE, 0xD7, 0xB5, 0xF1, 0x87, 0x5D, 0x33, 0x8D, 0xA0, 0xC2, 0x49, 0x57,
	0x97, 0xE5, 0x67, 0xC8, 0x39, 0xA4, 0x9A, 0x3D, 0x8B, 0xE4, 0x9D, 0xB2, 0x94, 0x62, 0x67, 0x4E,
	0x2F, 0x4E, 0x30, 0x1B, 0x30, 0xF9, 0xC4, 0x85, 0xD1, 0xBF, 0x93, 0xE8, 0x6B, 0xFB, 0x94, 0x39,
	0xD7, 0x85, 0xE7, 0x1F, 0x84, 0xDF, 0x61, 0x0A, 0x58, 0x23, 0xA5, 0x12, 0xB1, 0x72, 0x03, 0x92,
	0xE6, 0x95, 0x96, 0xF8, 0xD9, 0x34, 0x7B, 0xEC, 0xF7, 0xC9, 0x8B, 0x59, 0x69, 0x45, 0xEB, 0x64,
	0x0C, 0x2F, 0x6E, 0x65, 0xCD, 0xC6, 0xDD, 0x56, 0x99, 0xFF, 0x6D, 0xB6, 0x6C, 0x72, 0x31, 0x8A,
	0x31, 0x68, 0x50, 0x51, 0x81, 0xAE, 0xC1, 0xCB, 0x01, 0x0D, 0xAB, 0x4E, 0xC8, 0x7F, 0x64, 0x2A,
	0xA4, 0x39, 0x0C, 0xA3, 0x8C, 0xED, 0xE4, 0x2E, 0x69, 0xE2, 0x0E, 0x1E, 0x66, 0xC2, 0xDE, 0x86,
	0x56, 0xB1, 0xBA, 0x3C, 0x9E, 0x48, 0xA4, 0xE2, 0x76, 0x0B, 0x7F, 0x2C, 0x47, 0x43, 0x29, 0x50,
	0xF8, 0x97, 0x21, 0x27, 0x51, 0x26, 0x3D, 0x4C, 0xD5, 0x99, 0x46, 0x0E, 0x38, 0xB2, 0x49, 0x7E,
	0xFF, 0xBF, 0x90, 0x5D, 0x62, 0xB5, 0x50, 0xBE, 0x3C, 0x51, 0x53, 0xBD, 0x9F, 0xE9, 0x5A, 0xC8,
	0xB5, 0xD9, 0x41, 0xC5, 0x36, 0x2D, 0xD6, 0xC9, 0x68, 0x59, 0x2E, 0x9F, 0x69, 0xF7, 0x13, 0x1C,
	0xA9, 0x3F, 0xFF, 0x45, 0xFA, 0x0A, 0x96, 0x99, 0x99, 0x90, 0xE7, 0x16, 0x0F, 0x8D, 0x8E, 0x73,
	0x1E, 0x48, 0xD8, 0xAA, 0x38, 0x02, 0x53, 0x8E, 0x7A, 0xAA, 0x25, 0xEE, 0x21, 0xFB, 0x32, 0x27,
	0x40, 0x89, 0xA1, 0x7E, 0x79, 0xFC, 0x53, 0xAB, 0x37, 0x0C, 0xDB, 0x3B, 0x6F, 0xC9, 0x3E, 0x48,
	0x48, 0xFB, 0xE1, 0x09, 0x9C, 0x88, 0xE8, 0xA6, 0x85, 0xF7, 0xA8, 0xF8, 0xD4, 0xBF, 0x11, 0x03,
	0x4D, 0x2B, 0x6D, 0x00, 0xA9, 0xC8, 0xB7, 0xDB, 0xFC, 0x3F, 0xCF, 0xFE, 0x26, 0xA3, 0x68, 0x66,
	0x3B, 0x39, 0x5C, 0x87, 0x6C, 0xB6, 0x6B, 0x04, 0x40, 0xA3, 0x3A, 0x2A, 0xCD, 0x37, 0xD2, 0x93,
	0x3F, 0x5E, 0x24, 0x63, 0x9D, 0xB8, 0x1B, 0xA8, 0xAA, 0x0A, 0x87, 0x13, 0x48, 0x86, 0xEB, 0x86,
	0xF9, 0x7D, 0x17, 0xB7, 0x65, 0x0D, 0x98, 0xE7, 0xC0, 0x90, 0x49, 0x92, 0x9C, 0x94, 0x66, 0x16,
	0x41, 0x93, 0x63, 0x99, 0xB7, 0x04, 0xC6, 0x73, 0x8A, 0x19, 0x82, 0x30, 0x55, 0x97, 0x25, 0xDB,
	0xDF, 0x8A, 0x9D, 0x19, 0xF2, 0xCA, 0x58, 0xF1, 0x78, 0xE5, 0x8B, 0xBB, 0x2F, 0xCC, 0xBF, 0x3A,
	0x57, 0xD2, 0x71, 0xE6, 0xC0, 0x42, 0x9D, 0xC8, 0x6D, 0x13, 0x71, 0xA4, 0xCA, 0x1F, 0xF4, 0x5B,
	0x95, 0x66, 0x4E, 0x59, 0xFB, 0x43, 0xDF, 0xAD, 0x94, 0xC4, 0xEE, 0x1C, 0x76, 0x91, 0x0C, 0x21,
	0x7D, 0x55, 0xF5, 0xA1, 0xCF, 0x37, 0x59, 0xC4, 0x15, 0xF2, 0x06, 0xE7, 0xCB, 0xC0, 0x29, 0x55,
	0x42, 0x57, 0x0A, 0x7A, 0x4D, 0xAB, 0x3A, 0xF9, 0x4A, 0x9D, 0xDE, 0x12, 0x31, 0x13, 0xAD, 0x55,
	0x29, 0x59, 0x12, 0x35, 0xBB, 0x77, 0x8C, 0x9B, 0x29, 0xED, 0x84, 0x2A, 0x55, 0xCA, 0xC0, 0x0A,
	0x05, 0x29, 0xDC, 0x3B, 0x0A, 0x14, 0x30, 0x88, 0xF4, 0xCA, 0xA9, 0xD6, 0x58, 0x55, 0x50, 0xB4,
	0x4B, 0x2F, 0xEC, 0x08, 0xF4, 0x95, 0x84, 0x5A, 0x1C, 0xFD, 0x5C, 0x8A, 0x74, 0x46, 0x4C, 0x09,
	0x5F, 0x29, 0x96, 0x2C, 0x71, 0x3C, 0xE6, 0x65, 0x4A, 0xAE, 0x03, 0x68, 0xB1, 0x80, 0x9C, 0x96,
	0x90, 0x04, 0x39, 0x52, 0x96, 0xE8, 0x3A, 0x96, 0xFE, 0x05, 0xA5, 0x2F, 0x28, 0xE6, 0x51, 0x77,
	0xB8, 0x85, 0xFE, 0x72, 0x67, 0x5F, 0x5B, 0x16, 0x7A, 0x06, 0x4A, 0xAC, 0xC3, 0x15, 0xC3, 0x74,
	0xDB, 0xED, 0xBE, 0x3A, 0xB5, 0xC4, 0xAF, 0xA5, 0xF1, 0x83, 0xB0, 0xBB, 0x35, 0x33, 0x57, 0xE9,
	0x3D, 0xF0, 0x92, 0x66, 0x76, 0x65, 0xD1, 0xBB, 0x00, 0x6E, 0x4A, 0x82, 0xF9, 0x89, 0x0C, 0xD8,
	0x3F, 0x36, 0x99, 0xBD, 0x49, 0x9E, 0xAA, 0x2D, 0x19, 0x4A, 0xBC, 0x5A, 0x75, 0x56, 0xEA, 0x6D,
	0x45, 0xAD, 0xBF, 0xA1, 0xBA, 0xE3, 0x13, 0xDB, 0xAC, 0x66, 0x1F, 0x43, 0xF5, 0x5E, 0xCC, 0x05,
	0xB1, 0x8B, 0xF8, 0x89, 0xE4, 0xFE, 0x1B, 0x87, 0x17, 0x9A, 0xC0, 0xFB, 0x09, 0x88, 0x24, 0xF0,
	0x08, 0x42, 0x0C, 0x64, 0xA9, 0x88, 0xC0, 0x89, 0x1C, 0x06, 0x63, 0xB0, 0x47, 0x3E, 0x53, 0x14,
	0x8E, 0xF6, 0x4F, 0x10, 0xCE, 0x19, 0x8A, 0x50, 0xC8, 0xD4, 0x16, 0x11, 0x81, 0x46, 0xC2, 0x6C,
	0xED, 0x5D, 0x1F, 0x01, 0xCD, 0x1C, 0x6D, 0xB2, 0x1D, 0xED, 0x1D, 0x31, 0xB2, 0xE5, 0x7C, 0xB0,
	0x2F, 0x22, 0x3E, 0xCA, 0xED, 0x3E, 0x2D, 0x34, 0x05, 0xF9, 0xB8, 0xC1, 0xEB, 0x5C, 0x1C, 0x10,
	0xE0, 0xD4, 0x21, 0x90, 0xF7, 0xFB, 0x05, 0x22, 0xF1, 0x1C, 0xE0, 0x71, 0x40, 0x4D, 0x9D, 0x67,
	0x7E, 0x7B, 0x88, 0x87, 0xD9, 0x08, 0x51, 0x32, 0x56, 0xE3, 0xE0, 0xD8, 0xE9, 0x29, 0x31, 0xB4,
	0xA1, 0xCF, 0xC9, 0xCF, 0xA2, 0xE6, 0x10, 0x19, 0x35, 0x6F, 0xCA, 0xAE, 0xCC, 0xED, 0x30, 0x31,
	0xAB, 0x35, 0x8E, 0xF8, 0xFD, 0x14, 0x2E, 0x68, 0xD0, 0x84, 0x76, 0xE3, 0x47, 0x39, 0x3B, 0x09,
	0x00, 0x86, 0x44, 0xD2, 0x44, 0x89, 0xA6, 0x89, 0x9F, 0xFD, 0x18, 0x3A, 0x23, 0x04, 0x2E, 0xD0,
	0xCA, 0xEE, 0x8A, 0x9C, 0x53, 0x37, 0x5E, 0x64, 0xBF, 0x33, 0xC5, 0xC7, 0xB7, 0xD1, 0xB4, 0x9B,
	0xBA, 0x76, 0xB4, 0x76, 0x09, 0x67, 0xA7, 0xAF, 0x22, 0x8A, 0xFB, 0x51, 0xF1, 0x18, 0xB7, 0x4A,
	0xFE, 0x28, 0xBF, 0xFB, 0x2B, 0xD3, 0x06, 0x0D, 0x8B, 0x1A, 0xE6, 0xA6, 0xA3, 0x7C, 0xCE, 0xAA,
	0xBC, 0x10, 0x64, 0xEA, 0x7C, 0x2C, 0xED, 0xF9, 0x94, 0xF1, 0x5F, 0x9E, 0xDE, 0xCE, 0x0D, 0x45,
	0x2E, 0x3C, 0x99, 0x32, 0x89, 0x76, 0x42, 0x37, 0xDC, 0xAB, 0x17, 0x2D, 0xA1, 0x1D, 0x20, 0x53,
	0x11, 0xBC, 0xA3, 0x73, 0xB2, 0xBC, 0x86, 0x19, 0x3E, 0xBE, 0xA7, 0x5F, 0x65, 0xEF, 0x46, 0x02,
	0x4E, 0x55, 0x39, 0x92, 0x3C, 0xD5, 0x4F, 0x7A, 0x66, 0x77, 0x53, 0x52, 0x7A, 0x7D, 0xB3, 0x6F,
	0x04, 0xE0, 0x17, 0x95, 0x9B, 0x00, 0x52, 0x51, 0xDC, 0x13, 0xAB, 0x40, 0x8E, 0x6B, 0xE6, 0x82,
	0x3F, 0x55, 0xE3, 0xBB, 0x6C, 0x52, 0xE6, 0xE7, 0xFC, 0x5A, 0x4D, 0x83, 0xFB, 0xCF, 0xB9, 0x06,
	0x31, 0x33, 0xDD, 0x99, 0x2C, 0xD2, 0xD4, 0xB2, 0xB4, 0xC4, 0xCE, 0x17, 0x9F, 0xAC, 0x14, 0x5E,
	0xB5, 0xDE, 0x5D, 0x9F, 0x43, 0x39, 0xE3, 0xFD, 0xBB, 0x6A, 0x21, 0x6D, 0x61, 0x82, 0xFD, 0x3E,
	0x93, 0x06, 0xFC, 0xF6, 0x27, 0x23, 0xED, 0x2B, 0xE8, 0xA7, 0x84, 0x01, 0x44, 0x41, 0xA8, 0x88,
	0x1C, 0x58, 0xE4, 0x01, 0x29, 0x6B, 0x1C, 0xFC, 0xBA, 0x90, 0x19, 0x2E, 0x65, 0x36, 0x32, 0x78,
	0x0F, 0x0E, 0xA1, 0xA5, 0xB9, 0x37, 0x2A, 0x1C, 0x04, 0x4A, 0x7D, 0x56, 0x66, 0x14, 0x36, 0xFD,
	0x64, 0x83, 0x82, 0x9D, 0xB8, 0x1A, 0xD3, 0xEB, 0x04, 0x87, 0x3B, 0xAF, 0x8F, 0x3E, 0x35, 0x6F,
	0x35, 0xE6, 0x09, 0xFE, 0xBB, 0x1A, 0x4D, 0xEF, 0x0E, 0xCD, 0x65, 0x91, 0xC3, 0x53, 0xA9, 0x5A,
	0xA5, 0xCF, 0x6D, 0xC4, 0xCC, 0x1C, 0x79, 0x56, 0x76, 0x9D, 0xD3, 0x7F, 0x9D, 0x43, 0xDF, 0xBE,
	0x94, 0xD8, 0x74, 0x50, 0xAA, 0xF9, 0x6F, 0xB3, 0x9D, 0xC0, 0x3A, 0x65, 0xF7, 0xF8, 0xD9, 0xAB,
	0x62, 0xF2, 0xCD, 0xE7, 0xFA, 0x50, 0x9D, 0x1D, 0x4D, 0x38, 0xE8, 0x03, 0x5D, 0xD8, 0x8D, 0xD4,
	0x69, 0x55, 0x21, 0x11, 0x4F, 0x4F, 0x3E, 0x08, 0x2A, 0xC5, 0x6A, 0xCC, 0x8D, 0x8D, 0x66, 0x6B,
	0x0C, 0xC9, 0x84, 0xC5, 0xBD, 0xF8, 0x2A, 0x1B, 0xE2, 0x8D, 0x77, 0xB2, 0x12, 0x4D, 0x4B, 0x86,
	0x76, 0xA3, 0xFD, 0xC8, 0x56, 0x63, 0x33, 0x21, 0xDD, 0xB7, 0x1E, 0xF5, 0x53, 0x3E, 0x54, 0xEB,
	0xF4, 0xF2, 0xF1, 0xCC, 0x53, 0x47, 0x6D, 0xDA, 0x0A, 0x61, 0xA7, 0x3A, 0xC1, 0xD2, 0xD0, 0xA4,
	0x35, 0x43, 0x3D, 0xAC, 0x05, 0x3E, 0xB2, 0xE2, 0x15, 0x33, 0x96, 0xDB, 0x16, 0x00, 0x93, 0x58,
	0x86, 0x89, 0x95, 0x06, 0x3A, 0x40, 0xAE, 0xE6, 0x8C, 0x5D, 0x25, 0xBF, 0xD7, 0x17, 0xFA, 0x9D,
	0x6E, 0x80, 0x1B, 0x11, 0xAD, 0xEE, 0x81, 0x68, 0x4B, 0xB6, 0xF5, 0x8C, 0x6D, 0xFF, 0xFA, 0xD8,
	0x37, 0x5C, 0x34, 0x46, 0xBB, 0x7C, 0xAD, 0xE6, 0x25, 0x3F, 0x8B, 0x37, 0x90, 0xCA, 0xA4, 0x53,
	0x30, 0xEE, 0xF9, 0xCE, 0xEF, 0x77, 0x22, 0xF5, 0x33, 0x38, 0xA4, 0x9A, 0x3D, 0xD8, 0x6C, 0xDB,
	0x7A, 0x85, 0x6F, 0x3F, 0x44, 0xD2, 0x5D, 0x75, 0x7B, 0x9C, 0x5B, 0xCB, 0xEF, 0xAD, 0xE2, 0xA0,
	0x49, 0xD6, 0x5D, 0x6E, 0xB9, 0x1A, 0x8F, 0x54, 0x25, 0x4F, 0x17, 0x5B, 0xDB, 0xAD, 0x89, 0x55,
	0xAD, 0x07, 0x16, 0xC8, 0xFE, 0xD9, 0x05, 0xDB, 0xFD, 0x3C, 0x46, 0xF4, 0x79, 0xEA, 0x87, 0x8B,
	0xB8, 0x94, 0x7D, 0x90, 0x82, 0x34, 0x6C, 0x07, 0x17, 0x6F, 0xC5, 0xCE, 0x32, 0x3A, 0xFF, 0xA4,
	0xF3, 0x60, 0x8B, 0x52, 0x68, 0xB1, 0x20, 0x2B, 0x27, 0x08, 0xC1, 0xA3, 0xF7, 0x52, 0xF0, 0xAA,
	0x9C, 0xEA, 0x9E, 0x5E, 0xF7, 0x74, 0x5B, 0x6B, 0x67, 0xAD, 0x49, 0x05, 0x4D, 0xD3, 0xAC, 0x4D,
	0x18, 0x1B, 0x87, 0x18, 0x81, 0x86, 0xFD, 0x5E, 0x8A, 0xE0, 0xD7, 0x1F, 0x84, 0xD4, 0x1C, 0xB6,
	0xF6, 0xC0, 0x03, 0xB0, 0xFF, 0xA9, 0x6D, 0x74, 0x67, 0x02, 0x12, 0xE3, 0x29, 0x19, 0x94, 0x6E,
	0x17, 0x04, 0x5C, 0x63, 0xDE, 0x19, 0x83, 0x25, 0x2E, 0x3E, 0x9C, 0x1F, 0x90, 0xB1, 0xD9, 0xFA,
	0xB0, 0x02, 0x65, 0x41, 0x74, 0xC0, 0x44, 0x61, 0x55, 0x83, 0x33, 0x52, 0x2F, 0xC4, 0x51, 0x84,
	0x96, 0x63, 0x65, 0x36, 0x24, 0x5B, 0x61, 0xCA, 0x6B, 0x8A, 0x2A, 0xEB, 0x27, 0x48, 0x4E, 0x7E,
	0x1B, 0x1B, 0xCF, 0x7B, 0x45, 0x96, 0xF6, 0x35, 0x70, 0x77, 0x89, 0xB6, 0x18, 0xCE, 0x88, 0x54,
	0x02, 0x51, 0x50, 0x5A, 0xDF, 0xF9, 0x81, 0x5E, 0x21, 0x9A, 0xBE, 0x3D, 0xCE, 0x51, 0x92, 0xAC,
	0xBE, 0xF1, 0xFC, 0x6F, 0xCA, 0x7C, 0x29, 0xC1, 0x63, 0xB2, 0x2D, 0xB2, 0xCE, 0x4B, 0x45, 0xC2,
	0x72, 0x8B, 0x9C, 0x86, 0xDF, 0x73, 0x79, 0x4B, 0xCC, 0x84, 0xAE, 0xEB, 0x33, 0x67, 0x7B, 0xA5,
	0x8D, 0xEE, 0xCF, 0x3D, 0x9F, 0xDF, 0x34, 0xE1, 0x2F, 0xEC, 0x27, 0xF4, 0x12, 0xBD, 0xBD, 0x09,
	0x1A, 0x9A, 0x39, 0x6B, 0x5F, 0xF7, 0x0F, 0xDB, 0xAC, 0x89, 0x27, 0x72, 0xDE, 0xA8, 0xE1, 0xA3,
	0x7B, 0x87, 0xC5, 0x12, 0x74, 0x6B, 0xD2, 0xEC, 0xC8, 0xFC, 0x29, 0x80, 0x19, 0xF0, 0xB5, 0xDB,
	0x12, 0x08, 0x8D, 0x71, 0xE3, 0x59, 0x82, 0x87, 0x9A, 0xAA, 0xA9, 0x57, 0xA7, 0xDC, 0xCE, 0x12,
	0x98, 0xD5, 0xBA, 0x6F, 0x09, 0x8D, 0xFF, 0x32, 0x02, 0xBC, 0x58, 0x12, 0x09, 0x06, 0x06, 0xF0,
	0x5B, 0x68, 0x28, 0xC2, 0xEC, 0x5A, 0x9E, 0xA0, 0x08, 0x8B, 0xC8, 0xD8, 0x4A, 0x2B, 0x4B, 0xC2,
	0xF1, 0xDB, 0x59, 0xC6, 0x56, 0x1E, 0xEE, 0x97, 0xB2, 0x5C, 0xAF, 0xE2, 0xBE, 0x12, 0x00, 0xE6,
	0xA9, 0xA0, 0x74, 0xA5, 0xAF, 0x81, 0xA8, 0xAB, 0x97, 0xA0, 0x76, 0xA0, 0xF7, 0xB0, 0x94, 0xC8,
	0x9F, 0xE5, 0x45, 0x11, 0x15, 0x03, 0xED, 0x75, 0xAF, 0x7E, 0xE3, 0xAD, 0x85, 0x66, 0x1A, 0x10,
	0x26, 0x24, 0xD6, 0x64, 0x89, 0xBB, 0x4B, 0xFF, 0x14, 0x82, 0xFE, 0xC9, 0xF4, 0x4B, 0x1D, 0xAD,
	0x55, 0x8F, 0x7A, 0xE6, 0x9A, 0xB6, 0x66, 0xE9, 0x0B, 0x4B, 0xEE, 0x00, 0xCB, 0x79, 0x12, 0x2B,
	0x84, 0x46, 0xCC, 0xEC, 0xB3, 0xEB, 0x96, 0xB5, 0x25, 0x67, 0x28, 0x97, 0xE5, 0xA8, 0x5C, 0xAA,
	0x33, 0xD4, 0xA9, 0xC1, 0xB3, 0x89, 0x5A, 0x23, 0x27, 0x0B, 0xF4, 0xCE, 0x6E, 0xD7, 0x01, 0x95,
	0xE4, 0xF1, 0x85, 0xEA, 0xBB, 0xC5, 0x7D, 0xFF, 0x1E, 0x39, 0x2E, 0xCA, 0xF5, 0xC7, 0xCC, 0xBB,
	0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
	0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x01, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
	0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x03, 0x80, 0xDE, 0x3C, 0x62,
	0xBE, 0xE0, 0x02, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
	0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x07,
	0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x06, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
	0x65, 0x3B, 0xD9, 0x87, 0x04, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
	0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x05, 0xE7, 0xB9,
	0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0x0F, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
	0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0x0E, 0x50,
	0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0x0C, 0x52, 0xB0, 0xEE,
	0x32, 0x6C, 0x8E, 0xD0, 0x53, 0x0D, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
	0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x08, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
	0x57, 0x09, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
	0xE9, 0xB7, 0x55, 0x0B, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
	0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0x0A, 0x54, 0xD7, 0x89, 0x6B, 0x35
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//固定密钥 (第二层加密密钥)
char* fixkey="\xa5\x4c\xb4\x75\xb2\x34\x54\x7\x55\x92\x30\x10\x34\x69\xf2\x48\x70\x75\xac\xef\xee"
"\x45\x96\x5e\xbf\xc3\xd4\x84\xe5\x4c\xb4\x75";
//对换表
char* fixtable="abcdefghijklmnopqrstuvwxyz-=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

FreeGateDec::FreeGateDec()
{
	memset(m_decbuf,0,16);
	m_randkeylen=0;
	memset(m_randkey,0,10);
}

FreeGateDec::~FreeGateDec()
{

}

BOOL FreeGateDec::CheckCookieFor63(CString strCookie)
{
	int pos;
	CString strData;

	pos = strCookie.Find(";");  //只有一项
    if(pos != -1)
		return FALSE;
	
	pos = strCookie.Find("\x3d");		//发现"="号
    if(pos == -1)
		return FALSE;
	
    strData = strCookie.Mid(pos+1);//memcpy(databuf,pos+1,strlen(pos)-1);
	if(strData.GetLength() == 86)
	   return TRUE;
   else
	   return FALSE;
}

BOOL FreeGateDec::CheckUrlFor63(CString str_url)
{
	
	int i;
	CString str_left;
	CString str_right;
	
	CString str_tmp;
	str_url.TrimLeft();
	str_url.TrimRight();
	i = str_url.Find(".",0);
	if (i > 0 )  // 有. 不是
		return FALSE;
	
	
	i = str_url.Find("/",0);	
	if (i < 0 )   //没有 / 不是
		return FALSE;
	
	i = str_url.Find("/",i+1);
	
	str_tmp = str_url.GetAt(0);
	if (str_tmp.CompareNoCase("/") !=0 ) //第一个字节不是 / 不是
		return FALSE;	
	
	str_left = str_url.Left(i);
	str_right = str_url.Right(str_url.GetLength()-i);	
	
	str_left.Replace("/","");
	str_right.Replace("/","");
	
// 	str_tmp = str_right.GetAt(0);
// 	if (str_tmp.CompareNoCase("u") != 0 )  //第一个字节不是 u  则不是
// 		return FALSE;
	
	
	if (str_left.GetLength()<5 || str_left.GetLength()>8) //随机数长度小于5或大于8 不是
		return FALSE;
	
	int j;
	for(j = 0 ;j < str_left.GetLength();j++)
	{		
		i = (int)str_left.GetAt(j);
		if (i < 97 || i > 122)   //前面的随机数有不是小写的 不是
			return FALSE;				
	}
	
	for(j = 0 ;j < str_right.GetLength();j++)
	{	
		str_tmp = str_right.GetAt(j);
		i = (int)str_right.GetAt(j);
		
		if ((i >= 97 && i <= 122) || (i >= 65 && i <= 90) || (i >= 48 && i <= 57) || i == 95 || i == 45) //后半截存在不是a~z A~Z 0~9 _ - 的字符 则不是
			continue;
		else
			return FALSE;		
	}
	
	return TRUE;
}

BOOL FreeGateDec::AnalyseData(const char* srcdata,int srcdatalen,   // srcdata为 url 或referer的数据
						 char*dstdata,int& dstdatalen,   // dstdata为去掉垃圾字节与随机密钥的数据
						 char* randkey,int& randkeylen)
{
	//////////////////////////////////////////////////////////////////////////
//	int offset=0;
//	for(int i=1;i<srcdatalen;i++)
//	{
//		const char *p;
//		p=srcdata+i;
//		if(*p==0x2f)
//		{
//			offset=i;
//			break;
//		}
//	}
//	srcdatalen-=(offset+1);	
//	memmove(srcdata,srcdata+offset+1,srcdatalen);
//	for(i=0;i<srcdatalen;i++)
//	{
//		if(srcdata[i]!=0x2f)
//		{
//			dstdata[dstdatalen]=srcdata[i];
//			dstdatalen++;
//		}
//	}
//	
//	char*p=dstdata+dstdatalen-1;
//	int tmp=*p-0x61;
//    while (tmp>7)
//	{
//		tmp=tmp-7;
//	} 
//	randkeylen=tmp;
//	dstdatalen=dstdatalen-tmp-1;
//	memcpy(randkey,dstdata+dstdatalen,tmp);
//	dstdata[dstdatalen]='\0';
	//////////////////////////////////////////////////////////////////////////
	srcdatalen--;
	const char* tp = strchr(srcdata+1, '/');
	if (tp==NULL)
		tp = srcdata;
	else
		srcdatalen -= (tp-srcdata);
	dstdatalen = 0;
	while(srcdatalen>0)
	{
		if (*++tp != '/')
			dstdata[dstdatalen++] = *tp;
		srcdatalen--;
	}
	
	randkeylen = ((BYTE)*tp-'a')%7;
	dstdatalen -= randkeylen+1;
	if(randkeylen<3) //随机密钥要大于等3小于等于6
		return FALSE;

	memcpy(randkey, dstdata+dstdatalen, randkeylen);
	dstdata[dstdatalen] = 0;
	return TRUE;
}

BOOL FreeGateDec::AnalyseDataFor62(const char* srcdata,int srcdatalen,   // srcdata为 url 或referer的数据
						 char*dstdata,int& dstdatalen)   // dstdata为去掉垃圾字节与随机密钥的数据
{
	//////////////////////////////////////////////////////////////////////////
	const char* tp = srcdata;
	dstdatalen=0;
	while(--srcdatalen>0)
	{
		if (*++tp != '/')
			dstdata[dstdatalen++] = *tp;
	}
	dstdata[dstdatalen] = 0;
	return TRUE;
}

void FreeGateDec::DecodeCookieBlock(char*buf,int len)
{
	if(len<=2)
		return;	
	buf[0] ^=buf[len-1];
	buf[len-1] ^= buf[0];
	buf[0] ^=buf[len-1];
	len &= ~1;
	for(int i=0;i<len;i+=2)
	{
		buf[i] ^=buf[i+1];
		buf[i+1] ^= buf[i];
		buf[i] ^=buf[i+1];
	}
}

BOOL FreeGateDec::DecodeCookie1(char* buf,int& len)
{
//	char searchkey[]="\x3d\x3b\x20\x0d\x0a";
	
	int startpos=0;
	for(int i=0;i<len;i++)
	{
		if(buf[i]==0x3d || buf[i]==0x3b || buf[i]==0x20 || buf[i]==0xd || buf[i]==0xa)
		{ 
			DecodeCookieBlock(buf+startpos,i-startpos);
			startpos=i+1;
		}
	}
	if(startpos<len)
	{
		DecodeCookieBlock(buf+startpos,i-startpos);
	}
	return TRUE;
}

BOOL FreeGateDec::GetGlobalkey(char* buf,int len,char*g_key)
{
	int count=0;
	for(int i=len-1;i>=0;i--)
	{
		count++;
		if(buf[i]==0x3D)
		{
			if(count==9)
			{
				return (1==sscanf(buf+i+1, "%08x", g_key));
			}
//			return ConvertgKey(buf+i+1,g_key);
		} 
	}	
	return FALSE;
}

//BOOL FreeGateDec::ConvertgKey(char*buf,char*g_key)
//{
//	return (1==sscanf(buf,"%08x", g_key));
//}

BOOL FreeGateDec::DecodeCookie2(char* buf,int& len,char*randkey,int randkeylen)
{
	int keycount=0;
	for(int i=0;i<len;i++)	
	{
		if(keycount==randkeylen)
			keycount=0;
		char *p;
		p=buf+i;
		
		if(*p>=0x41 && *p<=0x5a) //大写
		{
			char kk=randkey[keycount];
			int aa=*p-0x41+0x1a+0xa2-kk; //
			if(aa>0x5a)
			{
				aa-=0x1a;
				if(aa>0x5a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;
		}
		else if(*p>=0x61 && *p<=0x7a)//小写
		{
			char kk=randkey[keycount];
			int aa=*p-0x61+0x1a+0xc2-kk; //
			if(aa>0x7a)
			{
				aa-=0x1a;
				if(aa>0x7a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;
		}
	}
	return TRUE;
}

BOOL FreeGateDec::DecodeCookie(char* buf,int& len,char*randkey,int randkeylen)
{
	DecodeCookie1(buf,len);
	DecodeCookie2(buf,len,randkey,randkeylen);
	return TRUE;
}

BOOL FreeGateDec::GetOffsetTable(char c,DWORD&offset)
{
	if(c>=0x41 && c<=0x5a) //大写
	{
		offset=(c-0x41)+38;
	}
	else if(c>=0x61 && c<=0x7a)//小写
	{
		offset=(c-0x61);
	}
	else if(c>=0x30 && c<=0x39)//数字
	{
		offset=(c-0x30)+28;
	}
	else if(c=='-')
	{
		offset=26;
	}
	else if(c=='=')
	{
		offset=27;
	}
	else
		return FALSE;
	
	return TRUE;
}

BOOL FreeGateDec::Convert2to1(char* pSrc,char *pDst)
{
	BOOL flag;
	char byte1;
	char c2,c1;
	c1=*pSrc;
	c2=*(pSrc+1);
	DWORD offset=0;
	flag=GetOffsetTable(c1,offset);
	if(!flag)
		return FALSE;
	
	offset=offset*4;
	byte1=(char)offset;
	flag=GetOffsetTable(c2,offset);
	if(!flag)
		return FALSE;
	
	offset=offset>>4;
	byte1=byte1+(char)offset;
	*pDst=byte1;
	return TRUE;
}

BOOL FreeGateDec::Convert3to2(char* pSrc,char *pDst)
{
	BOOL flag;
	char byte1,byte2;
	char c3,c2,c1;
	c1=*pSrc;
	c2=*(pSrc+1);
	c3=*(pSrc+2);
	DWORD offset;
	flag=GetOffsetTable(c1,offset);
	if(!flag)
		return FALSE;
	offset=offset*4;
	byte1=(char)offset;
	flag=GetOffsetTable(c2,offset);
	if(!flag)
		return FALSE;
	
	byte1+=(char)(offset>>4);
	byte2=(char)offset&0xf;
	flag=GetOffsetTable(c3,offset);
	if(!flag)
		return FALSE;
	
	offset=offset/4;
	byte2=(byte2<<4)+(char)offset;
	*pDst=byte1;
	*(pDst+1)=byte2;
	return TRUE;
}

BOOL FreeGateDec::Convert4to3(char* pSrc,char *pDst)
{
	BOOL flag;
	DWORD tempdst = 0;
	for(int i=0;i<4;i++)
	{
		char* p=pSrc+i;
		DWORD offset;
		flag=GetOffsetTable(*p,offset);
		if(!flag)
			return FALSE;
		offset=offset& 0x3f;
		tempdst=tempdst << 6;
		tempdst=tempdst | offset;
	}
	tempdst=tempdst&0xffffff;
	__asm
	{
		mov edx,tempdst
		mov eax,edx
		mov ebx,pDst
		sar eax,16
		and eax,0xff
		mov [ebx],eax
		mov eax,edx
		sar eax ,8
		and eax,0xff
		mov [ebx+1],eax
		mov eax,edx
		and eax,0xff
		mov [ebx+2],eax
	}
	return TRUE;
}

BOOL FreeGateDec::Decode1(char* srcdata,int srcdatalen,char* dstdata,int &dstdatalen)
{
	BOOL flag;
	int num;
	int rsv;
	dstdatalen=0;
	num=srcdatalen/4;
	rsv=srcdatalen%4;
	for(int i=0;i<num;i++)
	{
		flag=Convert4to3(srcdata+i*4,dstdata+i*3);
		if(!flag)
			return FALSE;
		dstdatalen+=3;
	}
	if(rsv==3)
	{
		flag=Convert3to2(srcdata+i*4,dstdata+i*3);
		if(!flag)
			return FALSE;
		dstdatalen+=2;	
	}
	else if(rsv==2)
	{
		flag=Convert2to1(srcdata+i*4,dstdata+i*3);
		if(!flag)
			return FALSE;
		dstdatalen+=1;
	}
	return TRUE;
}


BOOL FreeGateDec::Decode2(char*buf,int& len,char*g_key)
{
	int keycount=0;
	for(int i=0;i<len;i++)	
	{
		if(keycount==4)
			keycount=0;
		char *p;
		p=buf+i;
		char kk=g_key[keycount];
		*p=*p^kk;
		keycount++;
	}
	return TRUE;
}

void __declspec(naked)sub_415E50(DWORD arg_0,DWORD arg_4)
{
	__asm
	{
		mov     eax, [esp+4]
		push    esi
		mov     esi, [esp+4+8]
		mov     cl, [eax]
		mov     dl, [esi]
		mov     [eax], dl
		mov     [esi], cl
		pop     esi
		ret
	}
}

void __declspec(naked)sub_415C80(DWORD arg_0,DWORD arg_4,DWORD arg_8)
{
	__asm
	{
		sub     esp, 8
		xor     edx, edx
		xor     eax, eax
		push    ebx
		push    ebp
		mov     ebp, [esp+10h+0ch]
		push    esi
		push    edi
		mov     ecx, ebp
loc_415C91:                             ; CODE XREF: sub_415C80+19.j
		mov     [ecx], al
		inc     eax
		inc     ecx
		cmp     ax, 100h
		jl      short loc_415C91
		mov     [ebp+100h], dl
		mov     [ebp+101h], dl
		mov     byte ptr [esp+18h+0ch], dl
		xor     esi, esi
		mov     ebx, ebp
		lea     eax,[esp+18h-8]
		mov     DWORD ptr [eax], 100h
loc_415CB7:                             ; CODE XREF: sub_415C80+93.j
		mov     edi, [esp+18h+0ch]
		mov     ecx, [esp+18h+4]
		and     edi, 0FFh
		xor     eax, eax
		xor     edx, edx
		mov     al, [edi+ecx]
		mov     dl, [ebx]
		add     esi, eax
		add     edx, esi
		and     edx, 800000FFh
		jns     short loc_415CE2
		dec     edx
		or      edx, 0FFFFFF00h
		inc     edx
loc_415CE2:                             ; CODE XREF: sub_415C80+58.j
		mov     byte ptr [esp+18h-4], dl
		mov     esi, [esp+18h-4]
		and     esi, 0FFh
		lea     eax, [esi+ebp]
		push    eax
		push    ebx
		call    sub_415E50
		lea     eax, [edi+1]
		add     esp, 8
		cdq
		idiv    DWORD ptr [esp+18h+8]
		mov     eax, [esp+18h-8]
		inc     ebx
		dec     eax
		mov     [esp+18h-8], eax
		mov     byte ptr [esp+18h+0ch], dl
		jnz     short loc_415CB7
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		add     esp, 8
		ret
	}
}

void  __declspec(naked) sub_415D20(DWORD arg_0,DWORD arg_4,DWORD arg_8)
{
	__asm
	{
		sub     esp, 10h
		mov     ecx, [esp+10h+0ch]
		push    ebp
		push    esi
		mov     esi, [esp+18h+8]
		mov     al, [ecx+100h]
		mov     dl, [ecx+101h]
		xor     ebp, ebp
		mov     byte ptr [esp+18h-10h], al
		cmp     esi, ebp
		mov     byte ptr [esp+18h-0ch], dl
		mov     [esp+18h-8h], ebp
		jle     loc_415E30
		mov     esi, [esp+18h-10h]
		push    ebx
		push    edi
		mov     edi, [esp+20h-0ch]
		and     esi, 0FFh
		and     edi, 0FFh
		jmp     short loc_415D6B
loc_415D67:                             ; CODE XREF: sub_415D20+EA.j
        mov     ecx, [esp+20h+0ch]
loc_415D6B:                             ; CODE XREF: sub_415D20+45.j
        lea     eax, [esi+1]
        and     eax, 800000FFh
        jns     short loc_415D7C
        dec     eax
        or      eax, 0FFFFFF00h
        inc     eax
loc_415D7C:                             ; CODE XREF: sub_415D20+53.j
        mov     byte ptr [esp+20h-10h], al
        xor     eax, eax
        mov     esi, [esp+20h-10h]
        and     esi, 0FFh
        mov     al, [esi+ecx]
        lea     ebx, [esi+ecx]
        add     eax, edi
        and     eax, 800000FFh
        jns     short loc_415DA2
        dec     eax
        or      eax, 0FFFFFF00h
        inc     eax
loc_415DA2:                             ; CODE XREF: sub_415D20+79.j
        mov     byte ptr [esp+20h-0ch], al
        mov     edi, [esp+20h-0ch]
        and     edi, 0FFh
        lea     eax, [edi+ecx]
        push    eax
        push    ebx
        mov     [esp+28h-4h], eax
        call    sub_415E50
        mov     edx, [esp+28h-4h]
        xor     ecx, ecx
        xor     eax, eax
        add     esp, 8
        mov     cl, [edx]
        mov     al, [ebx]
        add     ecx, eax
        and     ecx, 800000FFh
        jns     short loc_415DDF
        dec     ecx
        or      ecx, 0FFFFFF00h
        inc     ecx
loc_415DDF:                             ; CODE XREF: sub_415D20+B5.j
        mov     edx, [esp+20h+0ch]
        mov     eax, [esp+20h+4]
        and     ecx, 0FFh
        mov     cl, [ecx+edx]
        mov     dl, [eax+ebp]
        xor     dl, cl
        mov     [eax+ebp], dl
        mov     eax, [esp+20h-8h]
        inc     eax
        movsx   ebp, ax
        mov     [esp+20h-8h], eax
        mov     eax, [esp+20h+8]
        cmp     ebp, eax
        jl      loc_415D67
        mov     eax, [esp+20h+0ch]
        mov     dl, byte ptr [esp+20h-10h]
        mov     cl, byte ptr [esp+20h-0ch]
        pop     edi
        pop     ebx
        pop     esi
        mov     [eax+100h], dl
        mov     [eax+101h], cl
        pop     ebp
        add     esp, 10h
        retn
loc_415E30:                             ; CODE XREF: sub_415D20+29.j
        pop     esi
        mov     [ecx+100h], al
        mov     [ecx+101h], dl
        pop     ebp
        add     esp, 10h
        retn
	}
}

BOOL FreeGateDec::Decode3(char*buf,int& len)
{
	char bufkey[0x104];
	memset(bufkey,0,0x104);
	sub_415C80((DWORD)fixkey,0x20,(DWORD)bufkey);
	sub_415D20((DWORD)buf,len,(DWORD)bufkey);
// 	if(buf[0]!=8)
// 		return FALSE;
// 	len=len-1;
// 	memmove(buf,buf+1,len);
	return TRUE;
}

BOOL FreeGateDec::Decode4(char*buf,int&len,char*randkey,int randkeylen)
{
	char bufkey[0x104];
	memset(bufkey,0,0x104);
	sub_415C80((DWORD)randkey,randkeylen,(DWORD)bufkey);
	sub_415D20((DWORD)buf,len,(DWORD)bufkey);
	return TRUE;	
}

WORD FreeGateDec::DecodeUrlAndReferer(CString& url, CString& Referer, LPCTSTR cookie,
						 int cokelen, char* randkey, int& randkeylen)
{
	WORD ver = 0;
	// url和referer的最后一个字母必须大于'c'
	int urllen = url.GetLength();
	BYTE c[4];
	c[0] = url.GetAt(urllen-1);
	if ((c[0] <= 'c') || (3>(c[0]-'a')%7))
		return 0;
	
	char gkey[4];
	memset(gkey,0,4);
	if (AnalyseData((LPCTSTR)url, urllen, m_httpsession->m_dceprocotol->m_HttpFileBuffer, urllen,
		randkey, randkeylen))
	{
		// 用随机密钥解密cookie数据
		memcpy(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, cookie, cokelen+1);
		DecodeCookie(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, cokelen, randkey, randkeylen);
		//得到全程密钥
		if (GetGlobalkey(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, cokelen, gkey))
		{
			if (!Decode1(m_httpsession->m_dceprocotol->m_HttpFileBuffer, urllen, m_httpsession->m_dceprocotol->m_CHUNKBUFFER, urllen)) //字节变换
				return 0;
			Decode2(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, urllen, gkey);  //全程密钥
			Decode3(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, urllen);		 //固定密钥
			if (m_httpsession->m_dceprocotol->m_CHUNKBUFFER[0] == 8)
			{
				memmove(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, m_httpsession->m_dceprocotol->m_CHUNKBUFFER+1, urllen--);
				ver = 0x601;
			}
			else if (m_httpsession->m_dceprocotol->m_CHUNKBUFFER[0] == 9)
			{
				memmove(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, m_httpsession->m_dceprocotol->m_CHUNKBUFFER+1, urllen--);
				ver = 0x602;
			}
			else
				return 0;
			Decode4(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, urllen, randkey, randkeylen);//随机密钥
			m_httpsession->m_dceprocotol->m_CHUNKBUFFER[urllen] = 0;
			url = m_httpsession->m_dceprocotol->m_CHUNKBUFFER;
		}
		else
			return 0;
	}
	else
		return 0;

	int reflen = Referer.GetLength();
	if (reflen>22)
	{
		if (ver == 0x601)
		{
			reflen-=21;
			c[0] = Referer.GetAt(Referer.GetLength()-1);
			if ((c[0] <= 'c') || (3>(c[0]-'a')%7))
				return 0;
			if (!AnalyseData((LPCTSTR)Referer+21, reflen, m_httpsession->m_dceprocotol->m_HttpFileBuffer, reflen,
				randkey, randkeylen))
				return 0;
		}
		else if (ver == 0x602)
		{
			int pos = Referer.Find(".com/");
			if (pos>0)
			{
				pos += 4;
				reflen -= pos;
				if (!AnalyseDataFor62((LPCTSTR)Referer+pos, reflen, m_httpsession->m_dceprocotol->m_HttpFileBuffer, reflen))
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
		if (!Decode1(m_httpsession->m_dceprocotol->m_HttpFileBuffer, reflen, m_httpsession->m_dceprocotol->m_CHUNKBUFFER, reflen)) //字节变换
			return 0;
		Decode2(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, reflen, gkey);		//全程密钥
		Decode3(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, reflen);			//固定密钥
		if (ver == 0x601 && m_httpsession->m_dceprocotol->m_CHUNKBUFFER[0]!=8)
			return 0;
		else if(ver==0x601)
			memmove(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, m_httpsession->m_dceprocotol->m_CHUNKBUFFER+1, reflen--);
		Decode4(m_httpsession->m_dceprocotol->m_CHUNKBUFFER, reflen, randkey, randkeylen);//随机密钥
		m_httpsession->m_dceprocotol->m_CHUNKBUFFER[reflen] = 0;
		Referer = m_httpsession->m_dceprocotol->m_CHUNKBUFFER;
		Referer.TrimRight();
	}
	return ver;
}

BOOL FreeGateDec::InitDataKey(const char* key,int keylen)
{
	DWORD key1; //key的前4位
	DWORD key2; //key的+4 
	GetArgKey(key,&key1,&key2);
	sub_40E2A0((DWORD*)m_decbuf,key1,key2);
	return TRUE;
}

BOOL FreeGateDec::InitDataKey(DWORD key1, DWORD key2)
{
	sub_40E2A0((DWORD*)m_decbuf,key1,key2);
	return TRUE;
}

//得到密钥
BOOL FreeGateDec::Getkey(const char* buf,int len,char* key,int &keylen)
{
	BOOL flag=FALSE;
	int nextpos=-1;
	if(buf[0]==0x2f)  // "/" 
	{
	
		char *tp = strchr(buf+1, '/');
		if (tp!=NULL)
		{
			nextpos = tp-buf;
			if (strchr(tp+1, '/'))
				return FALSE;	// 有两个以上的'/'
		}
		else
			return FALSE;
//		for(int i=1;i<len;i++)
//		{
//			if(buf[i]==0x2f)  // 第二个 /
//			{
//				if(!flag)
//				{
//					nextpos=i;
//					flag=TRUE;
//				}
//				
//				if(buf[i]==0x2f)
//				{
//					xcount++;
//				}
//				
//			}
//		}
	}
	
    if(nextpos>=6 && nextpos<=10)	
	{
		keylen=nextpos-1-2;
		memcpy(key,buf+1,keylen);
		return TRUE;
	}
	return FALSE;
	
}


void FreeGateDec::sub_40E4D0(DWORD* arg_0,DWORD* arg_4,DWORD arg_8)
{
	__asm
	{
		mov     eax, arg_0
		mov     edx, arg_8
		xor     ecx, ecx
		push    esi
		mov     esi, [eax]
		test    edx, edx
		jle     short loc_40E51E
		push    ebx
		push    ebp
		mov     edx, arg_4
		push    edi
loc_40E4E8:                             ; CODE XREF: sub_40E4D0+49.j
		mov     bl, [ecx+edx]
		mov     edi, [eax]
		and     ebx, 7
		and     esi, 7
		xor     esi, ebx
		mov     ebx, [eax+4]
		sar     ebx, 4
		or      bl, [eax+4]
		lea     edi, [edi+eax+8]
		xor     [edi], bl
		mov     edi, [eax]
		mov     bl, [edi+eax+8]
		xor     [ecx+edx], bl
		mov     ebx, [eax+4]
		inc     ecx
		inc     ebx
		cmp     ecx, arg_8
		mov     [eax], esi
		mov     [eax+4], ebx
		jl      short loc_40E4E8
		pop     edi
		pop     ebp
		pop     ebx
loc_40E51E:                             ; CODE XREF: sub_40E4D0+F.j
		pop     esi
	}
}


void FreeGateDec::sub_40E2A0(DWORD* arg_0,DWORD arg_4,DWORD arg_8)
{
	__asm
	{
		mov     ecx, arg_4
			mov     eax, arg_0
			mov     edx, arg_8
			xor     ecx, 0AE985D36h
			xor     edx, 3F6CB254h
			mov     [eax+8], ecx
			or      ecx, edx
			mov     dword ptr [eax+4], 0
			mov     dword ptr [eax], 0
			mov     [eax+0Ch], edx
			jnz     short locret_40E2DD
			mov     dword ptr [eax+8], 78B4FEAEh
			mov     dword ptr [eax+0Ch], 3DCF578Ah
locret_40E2DD:                          ; CODE XREF: sub_40E2A0+2D.j
			;retn
	}
}


//判断是否为自由之门的refer
BOOL FreeGateDec::CheckRefer(const char*buf,int len)
{
	if(len<22)
		return FALSE;
	if(memcmp(buf,"http://www.",11)==0 && memcmp(buf+17,".com",4)==0) //样式www.aaaaaa.com
	{
		for(int i=11;i<15;i++)
		{
			int tmpsum;
			tmpsum=buf[i]+0x10-0x61;
			int tmpsum1;
			tmpsum1=buf[i+2]-0x61;
			if((tmpsum%0x18)!=tmpsum1)
			{	
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}


BOOL FreeGateDec::DecodeRefer(char* buf,int& len,char*key,int keylen)
{
	int keycount=0;
	for(int i=0;i<len;i++)	
	{
		if(keycount==keylen)
			keycount=0;
		char *p;
		p=buf+i;
		
		if(*p>=0x41 && *p<=0x5a) //大写
		{
			char kk=key[keycount];
			int aa=*p-0x41+0x1a+0xa2-kk; //
			if(aa>0x5a)
			{
				aa-=0x1a;
				if(aa>0x5a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;
		}
		else if(*p>=0x61 && *p<=0x7a)//小写
		{
			char kk=key[keycount];
			int aa=*p-0x61+0x1a+0xc2-kk; //
			if(aa>0x7a)
			{
				aa-=0x1a;
				if(aa>0x7a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;
		}
	}
	return TRUE;
}


BOOL FreeGateDec::Decodeurl(char* buf,int& len,char*key,int keylen)
{
	for(int i=0;i<len;i++)
	{
		char *p;
		p=buf+i;
		if(*p==0x65)//碰到e
		{
			char *next;
			if(i<(len-1))
			{
				next=buf+i+1;
				if(*next>=0x30 && *next<=0x39)//数字
				{
					//右移一位 本位置为0x2f 
					*p=0x2f;
					memmove(buf+i+1,buf+i+2,len-i-1);
					len--;
				}
				else if(*next>=0x61 && *next<=0x7a)//小写
				{
					//右移一位 本位置为0x65 
					memmove(buf+i+1,buf+i+2,len-i-1);
					len--;
				}
				else
					return FALSE;
			}
			
		}
	}
	

	int keycount=0;
	for( i=0;i<len;i++)	
	{
		if(keycount==keylen)
			keycount=0;
		char *p;
		p=buf+i;

	    if(*p==0x2e) // " . "
		{
			int slen=len-i;
			BOOL endflag=TRUE;
			for(int k=1;k<slen;k++)
			{
				char *pp;
				pp=buf+i+k;
				if(*pp==0x2e)
				{
					slen=k; //下一个"."
					endflag=FALSE;
					break;
				}
			}

			int pos=0;
			for(k=1;k<slen;k++)
			{
				char *pp;
				pp=buf+i+k;
				if(*pp==0x2f)
				{
					pos=k;
					break;
				}
				else if(*pp==0x3f)
				{
					pos=k;
					break;
				}

			}

			if(endflag) //完
			{
				if(pos==0)//没有 / ? 
				{
					break;
				}
				else
				{
					i+=pos+1;//跳过.后缀
					p=buf+i;
				}
			}
			else
			{
				i+=pos+1;//跳过.后缀
				p=buf+i;
			}


		}

		if(*p>=0x41 && *p<=0x5a) //大写
		{
			char kk=key[keycount];
			int aa=*p-0x41+0x1a+0xa2-kk; //
			if(aa>0x5a)
			{
				aa-=0x1a;
				if(aa>0x5a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;
		}
		else if(*p>=0x61 && *p<=0x7a)//小写
		{
			char kk=key[keycount];
			int aa=*p-0x61+0x1a+0xc2-kk; //
			if(aa>0x7a)
			{
				aa-=0x1a;
				if(aa>0x7a)
					aa-=0x1a;
			}
			*p=aa;
			keycount++;

			kk=key[1];
			int flag=*p-kk+0x1a;
			aa=flag%0x1a;
			
			if(aa<0xa)
			{
				kk=key[1];
				aa=*p-0x61+0x1a+0x91-kk;
				if(aa>0x39)
				{
					aa-=0x1a;
					 
				}
				*p=aa;
			}


			
		}
		else if(*p>=0x30 && *p<=0x39) //数字 转为小写
		{
			char kk=key[1];
			int aa=*p-0x30+0x1a+kk;
			if(aa>0x7a)
			{
				aa-=0x1a;
				if(aa>0x7a)
					aa-=0x1a;
			}
			*p=aa;
		
		}
	}

	return TRUE;
}

void FreeGateDec::GetArgKey(const char* key,DWORD* key1 ,DWORD* key2)
{
	*key1=*((DWORD*)key);
	*key2=*((DWORD*)(key+4));
}

//产生随机密钥
// void  FreeGateDec::sub_40E2E0(DWORD * value1,DWORD *value2)
// {
// 	DWORD var_4=0;
// 	DWORD var_8=0;
// 	
// 	
// 	__asm
// 	{
// 		sub     esp, 8
// 			push    esi
// 			push    edi
// 			push    0               ; time_t *
// 			call    ds:time
// 			push    eax             ; unsigned int
// 			call    ds:srand
// 			mov     edi, ds:rand
// 			xor     eax, eax
// 			mov     var_8, eax
// 			add     esp, 8
// 			mov     var_4, eax
// 			xor     esi, esi
// 			call    edi ; rand
// 			and     eax, 80000003h
// 			jns     short loc_40E317
// 			dec     eax
// 			or      eax, 0FFFFFFFCh
// 			inc     eax
// 			
// loc_40E317:                             ; CODE XREF: sub_40E2E0+30.j
// 			add     eax, 3
// 			test    eax, eax
// 			jle     short loc_40E345
// 			
// loc_40E31E:                             ; CODE XREF: sub_40E2E0+63.j
// 			call    edi ; rand
// 			cdq
// 			mov     ecx, 1Ah
// 			idiv    ecx
// 			add     dl, 61h
// 			lea     ebx,var_8
// 			mov     byte ptr [ebx+esi], dl
// 			inc     esi
// 			call    edi ; rand
// 			and     eax, 80000003h
// 			jns     short loc_40E33E
// 			dec     eax
// 			or      eax, 0FFFFFFFCh
// 			inc     eax
// 			
// loc_40E33E:                             ; CODE XREF: sub_40E2E0+57.j
// 			add     eax, 3
// 			cmp     esi, eax
// 			jl      short loc_40E31E
// 			
// loc_40E345:                             ; CODE XREF: sub_40E2E0+3C.j
// 			lea     ebx,var_8
// 			mov     byte ptr [esi+ebx], 0
// 			mov     eax, var_8
// 			mov     edx, var_4
// 			pop     edi
// 			pop     esi
// 			add     esp, 8
// 			;retn
// 			
// }
// 	
// 	*value1=var_8;
// 	*value2=var_4;
// }



// void FreeGateDec::sub_40E460(DWORD* arg_0,DWORD* arg_4,DWORD arg_8)
// {
// 	__asm
// 	{
// 			mov     eax,  arg_0
// 			push    ebp
// 			mov     ecx, [eax+8]
// 			mov     edx, [eax+0Ch]
// 			or      ecx, edx
// 			jz      short loc_40E4C2
// 			mov     edx, arg_8
// 			xor     ecx, ecx
// 			test    edx, edx
// 			jle     short loc_40E4C2
// 			push    ebx
// 			push    esi
// 			push    edi
// 			mov     edi, arg_4
// 			
// loc_40E480:                             ; CODE XREF: sub_40E460+5D.j
// 			mov     edx, [eax]
// 			mov     bl, [eax+4]
// 			lea     esi, [edx+eax+8]
// 			mov     edx, [eax+4]
// 			sar     edx, 4
// 			or      dl, bl
// 			mov     bl, [esi]
// 			xor     bl, dl
// 			mov     [esi], bl
// 			mov     edx, [eax]
// 			mov     bl, [ecx+edi]
// 			mov     dl, [edx+eax+8]
// 			xor     bl, dl
// 			mov     [ecx+edi], bl
// 			mov     esi, [eax]
// 			mov     dl, bl
// 			and     esi, 7
// 			and     edx, 7
// 			xor     edx, esi
// 			mov     esi, [eax+4]
// 			inc     ecx
// 			inc     esi
// 			cmp     ecx, arg_8
// 			mov     [eax], edx
// 			mov     [eax+4], esi
// 			jl      short loc_40E480
// 			pop     edi
// 			pop     esi
// 			pop     ebx
// 			
// loc_40E4C2:                             ; CODE XREF: sub_40E460+D.j
// 			; sub_40E460+17.j
// 			pop     ebp
// 			;retn
// 			
// 	}
// }

void FreeGateDec::DecodeData(char*buf,int datalen)
{
	int base = 0;
	if (memcmp(buf, "\x1f\x8b\x08", 3)==0)
		base = 3;
	else if (memcmp(buf, "GIF89a", 6) == 0)
		base = 6;
	else if (memcmp(buf, "GIF87a", 6) == 0)
		base = 6;
	else if (memcmp(buf, "GIF", 3) == 0)
		base = 3;
	if (datalen > base)
		sub_40E4D0((DWORD*)m_decbuf,(DWORD*)(buf+base),datalen-base);
}


//----------------------------------------------------------

// 判断并处理特殊浏览方式的包
// 返回TRUE表示是特殊浏览方式 FALSE不是特殊浏览方式
BOOL DealEspecialBrowse(HttpCommInfo& Request,HttpSession* s)
{
//	return FALSE;
	// Host不是dip的就不处理
	CString tmpuri = Request.m_URI;
#ifndef		_DEBUG
//	Request.m_URI.Empty();
#endif
	if (!Request.m_biphost)
		return FALSE;
	// 先判断是不是动网通
	if (Request.m_bproxy)
	{
		if (memcmp(tmpuri, "http://dong.stday.com/", 22)==0)
		{
			// 是动网通
			FreeGateDec fnet;
			if (!Request.m_Referer.IsEmpty())
			{
				if (!fnet.CheckRefer(Request.m_Referer, Request.m_Referer.GetLength()))
				{
					return FALSE;
				}
			}
			char urlkey[12];
			int klen = 10;
			int len = tmpuri.GetLength()-21;
			memset(urlkey, 0, sizeof(urlkey));
			memcpy(s->m_dceprocotol->m_HttpFileBuffer, (LPCTSTR)tmpuri+21, len+1);
			if (fnet.Getkey(s->m_dceprocotol->m_HttpFileBuffer, len, urlkey, klen))
			{
				// 解码URL
				char *p = strchr(s->m_dceprocotol->m_HttpFileBuffer+1, '/');
				if (p > s->m_dceprocotol->m_HttpFileBuffer)
				{
					len = len-(p-s->m_dceprocotol->m_HttpFileBuffer)-1;
					p++;
				}
				else
					return FALSE;
				if (fnet.Decodeurl(p, len, urlkey, klen))
				{
					if (memicmp(p, "http/", 5)==0)
						Request.m_URL.Format("http:/%s", p);
					else
						Request.m_URL.Format("http://%s/%s", Request.m_Host, p);
				}

				// 解码Referer
				if (!Request.m_Referer.IsEmpty())
				{
					int reflen = Request.m_Referer.GetLength()-22;
					memcpy(s->m_dceprocotol->m_HttpFileBuffer, (LPCTSTR)Request.m_Referer+22, reflen+1);
					if (!fnet.DecodeRefer(s->m_dceprocotol->m_HttpFileBuffer, reflen, urlkey, klen))
						return FALSE;
					// 重新生成Referer
					if (memicmp(s->m_dceprocotol->m_HttpFileBuffer, "http/", 5)==0)
						Request.m_Referer.Format("http:/%s", s->m_dceprocotol->m_HttpFileBuffer+4);
					else
						Request.m_Referer = s->m_dceprocotol->m_HttpFileBuffer;
				}
				Request.m_Browser = TE_DYNAPASS;
				Request.m_BrowserVer = 0x200;	//2.0
				Request.m_bproxy = 0;
				fnet.GetArgKey(urlkey, &Request.m_FreenetKey1, &Request.m_FreenetKey2);
				return TRUE;
			}
		}
	}
	else
	{
		// 判断是不是无界浏览
		// 2007-11-5 rong 无界6.8 和 6.9的定位取消了
// 		if (Request.m_Cookie.GetLength()>27 &&
// 			(tmpuri.GetLength() <= 42))
// 		{
// 			DWORD ver=0;
// 			if (isUltrasurf6(Request.m_Cookie, Request.m_Cookie.GetLength(), ver))
// 			{
// 				Request.m_Browser = TE_WUJIE;
// 				Request.m_BrowserVer = ver;	// ver==0x608/0x609
// 				return TRUE;
// 			}
// 		}

		// 判断是不是自由之门
		if (tmpuri.GetLength()>11)
		{
			FreeGateDec fnet;
			fnet.m_httpsession=s;
			if (!Request.m_Cookie.IsEmpty())
			{
				// 自由之门6.1的处理 6.1必须有Cookie
				char randkey[8];
				memset(randkey,0,8);
				int randkeylen=0;
				Request.m_BrowserVer = fnet.DecodeUrlAndReferer(tmpuri, Request.m_Referer,
					Request.m_Cookie, Request.m_Cookie.GetLength(), randkey, randkeylen);
				if (Request.m_BrowserVer!=0)
				{
					tmpuri.TrimLeft('/');
					if (memicmp((LPCTSTR)tmpuri, "http/", 5)==0)
						Request.m_URL.Format("http:/%s", (LPCTSTR)tmpuri+4);
					else 
						Request.m_URL.Format("http://%s/%s", Request.m_Host, tmpuri);
//					Request.URL = tmpuri;
					Request.m_Browser = TE_FREEGATE;
					fnet.GetArgKey(randkey, &Request.m_FreenetKey1, &Request.m_FreenetKey2);
					return TRUE;
				}
				else
				{
					if( fnet.CheckUrlFor63(tmpuri) && 
						fnet.CheckCookieFor63(Request.m_Cookie))
					{
						Request.m_Browser = TE_FREEGATE;
						Request.m_BrowserVer = 0X603;
						
						return TRUE;
					}
				}
				Request.m_BrowserVer = 0;
			}
// 			return FALSE;
			if (!Request.m_Referer.IsEmpty())
			{
				// Referer 不为空的处理
				if (!fnet.CheckRefer(Request.m_Referer, Request.m_Referer.GetLength()))
				{
					return FALSE;
				}
			}
			// 获得随机密钥
			char key1[16];
			memset(key1, 0, sizeof(key1));
			int keylen1 = 15;
			if (!fnet.Getkey(tmpuri, tmpuri.GetLength(), key1, keylen1))
				return FALSE;
			
			// 解码URL
			int urllen = tmpuri.GetLength()-keylen1-4;
			memcpy(s->m_dceprocotol->m_HttpFileBuffer, (LPCTSTR)tmpuri+keylen1+4, urllen+1);
			if (!fnet.Decodeurl(s->m_dceprocotol->m_HttpFileBuffer, urllen, key1, keylen1))
				return FALSE;
			
			// 重新生成URL
			if (strnicmp(s->m_dceprocotol->m_HttpFileBuffer, "http/", 5)==0)
				Request.m_URL.Format("http:/%s", s->m_dceprocotol->m_HttpFileBuffer+4);
			else if (Request.m_Referer.IsEmpty())
				return FALSE;
			else 
				Request.m_URL.Format("http://%s/%s", Request.m_Host, s->m_dceprocotol->m_HttpFileBuffer);

			// 解码Referer
			if (!Request.m_Referer.IsEmpty())
			{
				int reflen = Request.m_Referer.GetLength()-22;
				memcpy(s->m_dceprocotol->m_HttpFileBuffer, (LPCTSTR)Request.m_Referer+22, reflen+1);
				if (!fnet.DecodeRefer(s->m_dceprocotol->m_HttpFileBuffer, reflen, key1, keylen1))
					return FALSE;
				// 重新生成Referer
				if (strnicmp(s->m_dceprocotol->m_HttpFileBuffer, "http/", 5)==0)
					Request.m_Referer.Format("http:/%s", s->m_dceprocotol->m_HttpFileBuffer+4);
				else
					Request.m_Referer = s->m_dceprocotol->m_HttpFileBuffer;
			}
			Request.m_Browser = 2;
			fnet.GetArgKey(key1, &Request.m_FreenetKey1, &Request.m_FreenetKey2);
			Request.m_BrowserVer = 0x600;	// 自由之门6.0
			return TRUE;
		}
	}
	return FALSE;
}

// 解码类型为Cookie: %s=%08x%s;的cookie
// 传人'=' 和 ';' 之间的部分
BOOL DecodeCookie1(const char* cookie, int len, DWORD& ver)
{
	ASSERT(len == 24);
	if (len != 24)
		return FALSE;
	// 首先还原16进制的数字串
	BYTE buf[12];
	DWORD d = 0;
	for (int i=0; i<24; *cookie++)
	{
		i++;
		if (*cookie>='a' && *cookie<='z')
		{
			d = (d<<4) | (*cookie-'a'+10);
		}
		else if (*cookie>='0' && *cookie<='9')
		{
			d = (d<<4) | (*cookie-'0');
		}
		else if (*cookie>='A' && *cookie<='Z')
		{
			d = (d<<4) | (*cookie-'A'+10);
		}
		else
			return FALSE;
		if (0==(i&7))
		{
			memcpy(buf+(i-8)/2, &d, 4);
			d = 0;
		}
	}

	buf[0] = (buf[1+4] ^ buf[2+4]) ^ buf[3+4];
	buf[1] = (buf[0+4] ^ buf[3+4]) ^ buf[1+4];
	if (buf[0] == 6)
	{
		if (buf[1]==8)
			ver = 0x608;
		else if (buf[1]==9)
			ver = 0x609;
		else
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

const char v_Base64Table[64] =
{
	0x41,0x42,0x43,0x44, 0x45,0x46,0x47,0x48, 0x49,0x4a,0x4b,0x4c, 0x4d,0x4e,0x4f,0x50,  // ABCDEFGHIJKLMNOP
	0x51,0x52,0x53,0x54, 0x55,0x56,0x57,0x58, 0x59,0x5a,0x61,0x62, 0x63,0x64,0x65,0x66,  // QRSTUVWXYZabcdef
	0x67,0x68,0x69,0x6a, 0x6b,0x6c,0x6d,0x6e, 0x6f,0x70,0x71,0x72, 0x73,0x74,0x75,0x76,  // ghijklmnopqrstuv
	0x77,0x78,0x79,0x7a, 0x30,0x31,0x32,0x33, 0x34,0x35,0x36,0x37, 0x38,0x39,0x2b,0x2f   // wxyz0123456789+/
};

BYTE v_DeBase64Table[] = 
{
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x20  ~~  0x27
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x28  ~~  0x2f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x30  ~~  0x37
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x38  ~~  0x3f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x40  ~~  0x47
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x48  ~~  0x4f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x50  ~~  0x57
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x58  ~~  0x5f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x60  ~~  0x67
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x68  ~~  0x6f
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		// 0x70  ~~  0x77
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff		// 0x88  ~~  0x7f
};


// Base64解码
inline BOOL GetOnechr(BYTE chr, BYTE& index)
{
	if ((chr <= 0x20) || (chr >= 0x7F))
		return FALSE;
	index = v_DeBase64Table[chr-0x20];
	return (index<64);
}

BOOL DeBase64(const char* inbuf, int inlen, char* outbuf, int& outlen)
{
	if (inlen & 3)
		return FALSE;	// 不是Base64编码
	if (v_DeBase64Table[0] == 0xFF)
	{
		for (int i=0; i<64; i++)
		{
			DWORD d = v_Base64Table[i];
			ASSERT(d > 0x20);
			ASSERT(d <= 0x7F);
			v_DeBase64Table[d-0x20] = i;
		}
		v_DeBase64Table[0] = 0xEE;
	}
	ASSERT(0==(inlen&3));
	BYTE tmpbuf[4];
	int tmplen = 0;
	memset(tmpbuf, 0, sizeof(tmpbuf));
	if (inbuf[inlen-1] == '=')
	{
		if (inbuf[inlen-2] == '=')
		{
			// 处理结尾是两个'='的数据
			if (!GetOnechr(inbuf[inlen-3], tmpbuf[1]))
				return FALSE;
			if (tmpbuf[1] & 0xF)
				return FALSE;
			if (!GetOnechr(inbuf[inlen-4], tmpbuf[2]))
				return FALSE;
			tmpbuf[0] = (tmpbuf[2]<<2) | (tmpbuf[1]>>4);
			tmplen = 1;
			inlen -= 4;
		}
		else
		{
			// 处理结尾是一个'='的数据
			if (!GetOnechr(inbuf[inlen-2], tmpbuf[3]))
				return FALSE;
			if (tmpbuf[3] & 3)
				return FALSE;
			if (!GetOnechr(inbuf[inlen-3], tmpbuf[2]))
				return FALSE;
			if (!GetOnechr(inbuf[inlen-4], tmpbuf[1]))
				return FALSE;
			tmpbuf[0] = (tmpbuf[1]<<2) | (tmpbuf[2]>>4);
			tmpbuf[1] = (tmpbuf[2]<<4) | (tmpbuf[3]>>4);
			tmplen = 2;
			inlen -= 4;
		}
	}
	ASSERT(0==(inlen&3));
	outlen = 0;
	int len = inlen>>2;
	DWORD dat;
	for (int i=0; i<len; i++)
	{
		if (!GetOnechr(*inbuf, tmpbuf[3]))
			return FALSE;
		dat = tmpbuf[3];
		inbuf++;
		if (!GetOnechr(*inbuf, tmpbuf[3]))
			return FALSE;
		dat = (dat<<6) | tmpbuf[3];
		inbuf++;
		if (!GetOnechr(*inbuf, tmpbuf[3]))
			return FALSE;
		dat = (dat<<6) | tmpbuf[3];
		inbuf++;
		if (!GetOnechr(*inbuf, tmpbuf[3]))
			return FALSE;
		dat = (dat<<6) | tmpbuf[3];
		inbuf++;
		
		*outbuf++ = (dat>>16)&0xFF;
		*outbuf++ = (dat>>8)&0xFF;
		*outbuf++ = (dat)&0xFF;
		outlen += 3;
	}
	if(tmplen>0)
	{
		memcpy(outbuf, tmpbuf, tmplen);
		outlen += tmplen;
	}
	return TRUE;
}

// 解码类型为Cookie: %s=%s;的cookie
// 传人'=' 和 ';' 之间的部分
BOOL DecodeCookie2(const char* cookie, int len, DWORD& ver)
{
	// Base64解码cookie
	int outlen = 0;
	char tmpbuf[2000];
	memset(tmpbuf,0,2000);
	if (len>1000)
	{
		return FALSE;
	}
	if (DeBase64(cookie, len, tmpbuf, outlen))
	{
		int b = (BYTE)*tmpbuf;
		if (outlen-b < 11)
			return FALSE;
		char *tp = tmpbuf+b+1;
		tp[0x0A] = tp[0x0A] ^ tp[0];
		tp[0x09] = tp[0x09] ^ tp[0];
		if (tp[10] == 6)
		{
			if (tp[9]==8)
				ver = 0x608;
			else if (tp[9]==9)
				ver = 0x609;
			else
				return FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL isUltrasurf6(const char* pCookie, int Cookielen, DWORD& ver)
{
	if (Cookielen < 27)
		return FALSE;
	// Cookie的第3/6/9字节等于'='
	// 估计是无界浏览
	// if (cookie[0]>='0' && cookie[0] <= '9')	cookie[3] == '='
	// if (cookie[0]>='a' && cookie[0] <= 'z')  cookie[6] == '='
	// if (cookie[0]>='A' && cookie[0] <= 'Z')  cookie[9] == '='
	LPCTSTR tmpCookie = pCookie;
	int Browser = 0;
	if (tmpCookie[3] == '=')
	{
		if ((tmpCookie[0] >= '0') && (tmpCookie[0] <= '9'))
		{
			Browser = 3;
			tmpCookie += 4;
		}
	}
	else if (tmpCookie[6] == '=')
	{
		if ((tmpCookie[0] >= 'a') && (tmpCookie[0] <= 'z'))
		{
			Browser = 3;
			tmpCookie += 7;
		}
	}
	else if (tmpCookie[9] == '=')
	{
		if ((tmpCookie[0] >= 'A') && (tmpCookie[0] <= 'Z'))
		{
			Browser = 3;
			tmpCookie += 10;
		}
	}
	if (Browser == 3) // 无界浏览
	{
		if ((tmpCookie[24]==';') || (tmpCookie[24]==0))
		{
			// Cookie: %s=%08x%s;
			// 解码这个Cookie
			return DecodeCookie1(tmpCookie, 24, ver);
		}
		else
		{
			char *tp = strchr(tmpCookie, ';');
			return DecodeCookie2(tmpCookie, tp-tmpCookie, ver);
		}
		return TRUE;
	}
	return FALSE;
}

// 写无界浏览的对照文件
// void WriteTeUrlIndexFile(DATEHEADER& packh, int PacketFrom, SocketSession& socketsion, int TeType)
// {
// 	LPCTSTR pro="";
// 	LPCTSTR ver;
// 
// 	switch(TeType)
// 	{
// 	case 0x608:	// 无界浏览6.8
// 		pro = "ultrabrowse";
// 		ver = "6.8";
// 		break;
// 	case 0x609:	// 无界浏览6.9
// 		pro = "ultrabrowse";
// 		ver = "6.9";
// 		break;
// 	case 0x800:	// 无界浏览8.0
// 		pro = "ultrabrowse";
// 		ver = "8.0";
// 		break;
// 	case 0x801:	// 无界浏览8.1
// 		pro = "ultrabrowse";
// 		ver = "8.1";
// 		break;
// 	case 0x802:	// 无界浏览8.2
// 		pro = "ultrabrowse";
// 		ver = "8.2";
// 		break;
// 	case 0x805:	// 无界浏览8.5
// 		pro = "ultrabrowse";
// 		ver = "8.5";
// 		break;
// 	case 0x100:	//火凤凰
// 		pro = "firephoenix";
// 		ver = "1.0";
// 		break;
// 	case 0x603:	// 自由之门6.3
// 		pro = "freegateurl";
// 		ver = "6.3";
// 		break;
// 	default:
// 		return;
// 	}
// 
// 	SYSTEMTIME now;
// 	GetLocalTime(&now);
// 	CString tmppath = g_httpMissObj->GetTmpPath();
// 	socketsion.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
// 		tmppath, now.wYear, now.wMonth, now.wDay,
// 		now.wHour, now.wMinute, now.wSecond, socketsion.m_sip, socketsion.m_dip, ++g_httpFileNum, pro);
// 	CString text;		
// 	text.Format("ver= %s\r\n", ver);
// 
// 	socketsion.WriteIndexFile(pro, pro, text, g_httpMissObj);
// 
// }

// 写无界浏览的对照文件
void WriteFreeGateFile(DATEHEADER& packh, HttpCommInfo& Request, HttpSession& httpsession,
					   LPCTSTR pSpyaddr, LPCTSTR pro)
{
	LPCTSTR ver = "";
	if (Request.m_Browser==TE_FREEGATE)
	{
		if (Request.m_BrowserVer==0x600)
			ver = "6.0";
		else if (Request.m_BrowserVer==0x601)
			ver = "6.1";
		else if (Request.m_BrowserVer==0x602)
			ver = "6.2";
		else if (Request.m_BrowserVer==0x603)
			ver = "6.3";
	}

	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = httpsession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	if (httpsession.m_indexfname.IsEmpty())
	{
		httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_%s.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, httpsession.m_sip, httpsession.m_dip, ++httpsession.m_dceprocotol->m_httpFileNum, pro);
	}
	CString text;
	text.Format("ver= %s\r\n%s", ver, GetHttpInfo(Request));
	if( v_stat && v_statOther &&
		(stricmp(pro,"dynapassdown")==0||
		stricmp(pro,"freegatedown")==0||
		stricmp(pro,"dynapassup")==0||
		stricmp(pro,"freegateup")==0||
		stricmp(pro,"freegateurl")==0||
		stricmp(pro,"ultrabrowse")==0))
	{
		HistoryRecord hisRec;
		CString tmpstrhis;
		httpsession.WriteHisData(pro, pro, &hisRec, httpsession.m_dceprocotol->m_httpMissObj, HTTP_PROTOCOL);//, HISTORY_DATA);
	}
	httpsession.WriteIndexFile(pro, pro, text, httpsession.m_dceprocotol->m_httpMissObj);
}

//******************************** 无界8.0的还原 ************************************

void sub_40E760(DWORD, DWORD);
void sub_40E5B0(void*, int, void*);
void sub_40E640(void*, DWORD, void*);
class CXorKey :public CObject
{
public:
	BYTE c_xorstring[300];
	BYTE s_xorstring[300];
	CXorKey(){
		memset(c_xorstring, 0, 300);
		memset(s_xorstring, 0, 300);
	}
};

BOOL isUltrasurf8(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl)
{
	if(packh.m_datalen == 102 && 0 == memcmp(pPacket, U89_CLIENTHELLO, 11))
	{
		CXorKey pObKey;
		
		unsigned char cKey[16];
		GetFristKey((const unsigned char *)(pPacket+12), cKey);
		sub_makekey_40EB56(cKey, 16, pObKey.c_xorstring);
		sub_makekey_40EB56(cKey, 16, pObKey.s_xorstring);

		sub_xorcode_40EBC7((unsigned char *)(pPacket+0x14), 0xc, pObKey.c_xorstring);

		if(CheckU89ClientHello((const unsigned char *)(pPacket+0x10)))
		{
				// 是无界的包
//			g_UltrasurfPacketCount++;
			// 匹配案件和预警
			HttpSession tmpsession(packh, g_httpFrom);
			tmpsession.m_OpenCaseFlg = v_opencasewujie;
			tmpsession.m_OpenWarnFlg = v_openwarnwujie;
			tmpsession.m_OpenIndxFlg = v_openflagwujie;
			tmpsession.m_spyaddr = g_DDEPOS;
			DWORD state = 0;
			SearchCaseOrWarn(&tmpsession, NULL, 0, state);

			// 写对照文件
			tmpsession.m_Requestline.m_Browser = TE_WUJIE;
			tmpsession.m_Requestline.m_BrowserVer = 0x808;
			tmpsession.WriteTeUrlIndexFile();
			//WriteTeUrlIndexFile(packh, g_httpFrom, tmpsession, ver);
			return TRUE;
		}

	}
	else if (packh.m_datalen >= 32)
	{
		DWORD ver;
		if (memcmp(pPacket+30, "\x8\x0", 2)==0)
		{
			ver = 0x800;		// 无界8.0
		}
		else if (memcmp(pPacket+30, "\x8\x1", 2)==0)
		{
			ver = 0x801;		// 无界8.1
		}
		else if (memcmp(pPacket+30, "\x8\x2", 2)==0)
		{
			ver = 0x802;		// 无界8.2
		}
		else if (packh.m_datalen == 512 && 	memcmp(pPacket+13,"\x6d\x09\x77\x68\x64\x6a\x6f\x67\x75\x70\x72\x04\x69\x6e\x66\x6f",16) ==0)
		{
			HttpSession tmpsession(packh, g_httpFrom);
			tmpsession.m_dceprocotol=httpprotocl;
			tmpsession.m_OpenCaseFlg = v_opencasewujie;
			tmpsession.m_OpenWarnFlg = v_openwarnwujie;
			tmpsession.m_OpenIndxFlg = v_openflagwujie;
			tmpsession.m_spyaddr = g_DDEPOS;

			DWORD state = 0;
			SearchCaseOrWarn(&tmpsession, NULL, 0, state);

			// 写对照文件
			tmpsession.m_Requestline.m_Browser = TE_WUJIE;
			tmpsession.m_Requestline.m_BrowserVer = 0x1301;
			tmpsession.WriteTeUrlIndexFile();

			return TRUE;
		}
		else if(((packh.m_datalen == 116 || packh.m_datalen == 114 ) && 
			( memcmp(pPacket+30, "\x65\x64\x6f\x6f\x72\x73", 6)==0 || //t模式
			memcmp(pPacket+69, "\x65\x64\x6f\x6f\x72\x73", 6)==0 ) && //u模式
			packh.m_sport == 1863) || 
			(packh.m_sport == 53 &&									 // e模式,域名为 www.edoors.com
			memcmp(pPacket+12,"\x03\x77\x77\x77\x06\x65\x64\x6f\x6f\x72\x73\x03\x63\x6f\x6d",15) ==0) ||
			(packh.m_sport == 53 &&									 // e模式,域名为 news.edoors.com
			memcmp(pPacket+12,"\x04\x6e\x65\x77\x73\x06\x65\x64\x6f\x6f\x72\x73\x03\x63\x6f\x6d",16) ==0))
		{
			// 是火凤凰的包匹配认证和案件并写对照文件
				// 匹配案件和预警

			HttpSession tmpsession(packh, g_httpFrom);
			tmpsession.m_OpenCaseFlg = v_opencasewujie;
			tmpsession.m_OpenWarnFlg = v_openwarnwujie;
			tmpsession.m_OpenIndxFlg = v_openflagwujie;
			tmpsession.m_spyaddr = g_DDEPOS;

			// 交换IP方向
			memcpy(tmpsession.m_cdip,packh.m_sip,4);
			memcpy(tmpsession.m_csip,packh.m_dip,4);
			tmpsession.m_AuthInfo.SwapAuthInfo();
			CString s;
			s = tmpsession.m_dmac;
			tmpsession.m_dmac = tmpsession.m_smac;
			tmpsession.m_smac = s;
			s = tmpsession.m_sip;
			tmpsession.m_sip = tmpsession.m_dip;
			tmpsession.m_dip = s;

			DWORD state = 0;
			SearchCaseOrWarn(&tmpsession, NULL, 0, state);

			// 写对照文件
			tmpsession.m_Requestline.m_Browser = TE_FIREPHONENIX;
			tmpsession.m_Requestline.m_BrowserVer = 0;	// 写文件时判断赋值
			tmpsession.WriteTeUrlIndexFile();

			return TRUE;

		}
		else if (packh.m_datalen <= 0xE0)
		{
			srand(*(DWORD*)(pPacket + 8));
			int len = (rand() % 0x80) + 0x40 + 32;
			if (len == packh.m_datalen)
			{
				ver = 0x805;
			}
			else
			{
				ver = 0x807;// 可能8.7 FALSE;
			}
		}
		else
		{
			return FALSE;
		}
		
		// 进一步判断是否是无界的包
		char keybuf[260];
		char buf1[8];
		char buf2[8];
		memset(buf1, 0, sizeof(buf1));
		memset(buf2, 0, sizeof(buf2));
		memset(keybuf, 0, sizeof(keybuf));
		sub_40E5B0((void*)(pPacket+8), 16, keybuf);
		memcpy(buf1, pPacket, 8);
		memcpy(buf2, pPacket+24, 8);
	 	sub_40E640(buf1, 8, keybuf);
		if (ver == 0x805)
		{
	 		sub_40E640(buf2, 8, keybuf);
			if (memcmp(buf2+6, "\x8\x5", 2) != 0)
			{
				if (memcmp(buf2+6, "\x8\x6", 2) == 0)
				{
					ver = 0x806;
				}
				else
				{
					return FALSE;
				}
			}
		}
		else if(ver == 0x807)
		{
	 		sub_40E640(buf2, 8, keybuf);
			if (memcmp(buf2+6, "\x8\x7", 2) != 0)
			{
				return FALSE;
			}
		}
		else
		{
	 		sub_40E640(buf2, 6, keybuf);
		}
		if (memcmp(buf2, packh.m_dip, 4) == 0)
		{
			if (packh.m_dport == Swap2Byte(*((WORD*)(buf2+4))))
			{
				// 是无界的包
//				g_UltrasurfPacketCount++;
				// 匹配案件和预警
				HttpSession tmpsession(packh, g_httpFrom);
				tmpsession.m_OpenCaseFlg = v_opencasewujie;
				tmpsession.m_OpenWarnFlg = v_openwarnwujie;
				tmpsession.m_OpenIndxFlg = v_openflagwujie;
				tmpsession.m_spyaddr = g_DDEPOS;
				DWORD state = 0;
				SearchCaseOrWarn(&tmpsession, NULL, 0, state);

				// 写对照文件
				tmpsession.m_Requestline.m_Browser = TE_WUJIE;
				tmpsession.m_Requestline.m_BrowserVer = ver;
				tmpsession.WriteTeUrlIndexFile();
				//WriteTeUrlIndexFile(packh, g_httpFrom, tmpsession, ver);
				return TRUE;
			}	
		}
	}
	return FALSE;
}

void GetFristKey(const BYTE *fpacket8, unsigned char key[16])
{
	int tmp_var138;
	{
		int tmp_al, tmp_cl, tmp_bl, tmp_dl;
		int tmp_ecx, tmp_edx;
		for (int i = 0; i < 8 ; i++)
		{
			tmp_al = fpacket8[i];
			tmp_dl = g_fixKey[tmp_al+(i+1)*0x100];
			tmp_var138 = tmp_dl;
			tmp_edx = tmp_var138;
			tmp_edx &= 0xff;
			tmp_al ^= tmp_edx;
			tmp_al = g_fixKey[tmp_al+(i+1)*0x100];
			tmp_al ^= g_fixKey[tmp_edx+0x900];
// 			printf("%02x ", tmp_al);
			key[i] = tmp_al;
		}
		for ( i = 0; i< 8; i++)
		{
			tmp_al = fpacket8[i];
			tmp_cl = g_fixKey[tmp_al+0x900];
			tmp_var138 = tmp_cl;
			tmp_ecx = tmp_var138;
			tmp_ecx &= 0xff;
			tmp_al ^= tmp_ecx;

			tmp_bl = g_fixKey[tmp_ecx+(i+1)*0x100];
			tmp_al = g_fixKey[tmp_al+0x900];
			tmp_al ^= tmp_bl;
// 			printf("%02x ", tmp_al);
			key[i+8] = tmp_al;
		}
	}
// 	printf("\n");
}
void __declspec(naked) sub_40E760(DWORD, DWORD)
{
// 	4           = dword ptr  4
// 	8           = dword ptr  8
	__asm
	{
		mov     eax, [esp+4]
		push    esi
		mov     esi, [esp+4+8]
		mov     cl, [eax]
		mov     dl, [esi]
		mov     [eax], dl
		mov     [esi], cl
		pop     esi
		retn
	}
}


void __declspec(naked) sub_40E5B0(void*, int, void*)
{
	__asm
	{
        push    ecx
        push    ebx
        push    ebp
        mov     ebp, [esp+9+0x0F]
        push    esi
        xor     ebx, ebx
        push    edi
        xor     eax, eax
loc_40E5BD:                             ; CODE XREF: sub_40E5B0+16.j
        mov     [eax+ebp], al
        inc     eax
        cmp     eax, 100h
        jl      short loc_40E5BD
        mov     [ebp+100h], bl
        mov     [ebp+101h], bl
        mov     byte ptr [esp+11h+0x0F], bl
        xor     esi, esi
loc_40E5DA:                             ; CODE XREF: sub_40E5B0+86.j
        mov     edi, [esp+11h+0x0F]
        mov     edx, [esp+11h+7]
        and     edi, 0FFh
        xor     ecx, ecx
        lea     eax, [ebx+ebp]
        mov     cl, [edi+edx]
        xor     edx, edx
        mov     dl, [eax]
        add     esi, ecx
        add     edx, esi
        and     edx, 800000FFh
        jns     short loc_40E608
        dec     edx
        or      edx, 0FFFFFF00h
        inc     edx
loc_40E608:                             ; CODE XREF: sub_40E5B0+4E.j
        mov     byte ptr [esp+11h-1], dl
        mov     esi, [esp+11h-1]
        and     esi, 0FFh
        lea     ecx, [esi+ebp]
        push    ecx
        push    eax
        call    sub_40E760
        lea     eax, [edi+1]
        add     esp, 8
        cdq
        idiv    dword ptr [esp+11h+0x0B]
        inc     ebx
        cmp     ebx, 100h
        mov     byte ptr [esp+11h+0x0F], dl
        jl      short loc_40E5DA
        pop     edi
        pop     esi
        pop     ebp
        pop     ebx
        pop     ecx
        retn
	}
}

void __declspec(naked) sub_40E640(void*, DWORD, void*)
{
	__asm
	{
        sub     esp, 0Ch
        mov     ecx, [esp+0Ch+0x0C]
        push    ebp
        push    esi
        mov     esi, [esp+14h+8]
        mov     al, [ecx+100h]
        mov     dl, [ecx+101h]
        xor     ebp, ebp
        mov     byte ptr [esp+14h-0xC], al
        test    esi, esi
        mov     byte ptr [esp+14h-0x8], dl
        jle     loc_40E741
        mov     esi, [esp+14h-0xC]
        push    ebx
        push    edi
        mov     edi, [esp+1Ch-0x8]
        and     esi, 0FFh
        and     edi, 0FFh
        jmp     short loc_40E687
loc_40E683:                             ; CODE XREF: sub_40E640+DB.j
        mov     ecx, [esp+1Ch+0x0C]
loc_40E687:                             ; CODE XREF: sub_40E640+41.j
        lea     eax, [esi+1]
        and     eax, 800000FFh
        jns     short loc_40E698
        dec     eax
        or      eax, 0FFFFFF00h
        inc     eax
loc_40E698:                             ; CODE XREF: sub_40E640+4F.j
        mov     byte ptr [esp+1Ch-0xC], al
        xor     eax, eax
        mov     esi, [esp+1Ch-0xC]
        and     esi, 0FFh
        mov     al, [esi+ecx]
        lea     ebx, [esi+ecx]
        add     eax, edi
        and     eax, 800000FFh
        jns     short loc_40E6BE
        dec     eax
        or      eax, 0FFFFFF00h
        inc     eax
loc_40E6BE:                             ; CODE XREF: sub_40E640+75.j
        mov     byte ptr [esp+1Ch-0x8], al
        mov     edi, [esp+1Ch-0x8]
        and     edi, 0FFh
        lea     eax, [edi+ecx]
        push    eax
        push    ebx
        mov     [esp+24h-0x4], eax
        call    sub_40E760
        mov     edx, [esp+24h-0x4]
        xor     ecx, ecx
        xor     eax, eax
        add     esp, 8
        mov     cl, [edx]
        mov     al, [ebx]
        add     ecx, eax
        and     ecx, 800000FFh
        jns     short loc_40E6FB
        dec     ecx
        or      ecx, 0FFFFFF00h
        inc     ecx
loc_40E6FB:                             ; CODE XREF: sub_40E640+B1.j
        mov     edx, [esp+1Ch+0x0C]
        mov     eax, [esp+1Ch+4]
        and     ecx, 0FFh
        mov     cl, [ecx+edx]
        mov     dl, [eax+ebp]
        xor     dl, cl
        mov     [eax+ebp], dl
        mov     eax, [esp+1Ch+8]
        inc     ebp
        cmp     ebp, eax
        jl      loc_40E683
        mov     eax, [esp+1Ch+0x0C]
        mov     dl, byte ptr [esp+1Ch-0xC]
        mov     cl, byte ptr [esp+1Ch-0x8]
        pop     edi
        pop     ebx
        pop     esi
        mov     [eax+100h], dl
        mov     [eax+101h], cl
        pop     ebp
        add     esp, 0Ch
        retn
loc_40E741:                             ; CODE XREF: sub_40E640+25.j
        pop     esi
        mov     [ecx+100h], al
        mov     [ecx+101h], dl
        pop     ebp
        add     esp, 0Ch
        retn
	}
}
//无界最新版本处理

BOOL CheckU89ClientHello(const BYTE *fpacke)
{
	int tmp_al, tmp_cl;
	tmp_cl = fpacke[0];
	tmp_al = g_fixKey[0x100+tmp_cl];
	if(tmp_al != fpacke[4])
		return FALSE;

	tmp_cl = fpacke[1];
	tmp_al &= 7;
	tmp_al <<= 8;
	tmp_al = g_fixKey[0x100+tmp_al+tmp_cl];
	if(tmp_al != fpacke[5])
		return FALSE;

	tmp_cl = fpacke[2];
	tmp_al &= 7;
	tmp_al <<= 8;
	tmp_al = g_fixKey[0x100+tmp_al+tmp_cl];
	if(tmp_al != fpacke[6])
		return FALSE;

	tmp_cl = fpacke[3];
	tmp_al &= 7;
	tmp_al <<= 8;
	tmp_al = g_fixKey[0x100+tmp_al+tmp_cl];
	if(tmp_al != fpacke[7])
		return FALSE;

	// 后面尚有8字节, 含义未知, 是参数传递进来的, 通过还原可见
	// 前4字节未知, 一般为 'Q\Uf', 后4字节为服务器IP
	return TRUE;
}
void sub_xorcode_40EBC7(BYTE* arg_0, int arg_4, BYTE* arg_8)
{
	// arg_0: 输入和输出缓冲区
	// arg_4: 缓冲区长度
	// arg_8: 加密的因子长度258Byte
	BYTE  var_1;
    DWORD var_8;
//    DWORD var_C;
    DWORD var_10;
    BYTE* var_14;
    BYTE a = arg_8[0x100];
    BYTE c = arg_8[0x101];
	
    if (arg_4>0)
    {
        DWORD da = a;
        DWORD dc = c;
		BYTE  b_arg8_3 = 0;
        for(var_8 = 0; var_8 < arg_4; var_8++, dc = var_10)
        {
//            var_C = var_8;
            da = (da+1) % 0x100;
            b_arg8_3 = da & 0xFF;
            BYTE* pebx = arg_8+da;
            var_10 = (dc+(*pebx))%0x100;
            var_14 = arg_8+var_10;
            var_1 = var_10 & 0xFF;
            //sub_40EC7D(pebx, var_14);	// *pebx <==> *var_14
			BYTE tmp = *pebx;
			*pebx = *var_14;
			*var_14 = tmp;
            BYTE d = arg_8[(*pebx + *var_14)%0x100];
            arg_0[var_8] = arg_0[var_8] ^ d;
        }
        a = b_arg8_3;
        c = var_1;
    }
    arg_8[0x100] = a;
    arg_8[0x101] = c;
}
void sub_makekey_40EB56(BYTE* arg_0, int arg_4, BYTE* arg_8)	// 测试ok
{
	//arg_0: 是密钥
	//arg_4: arg_0中有效数据的长度(一般是16)
	//arg_8: 存放生成的加密因子长度258Byte
	   BYTE* parg_8 = arg_8;
    for(int i=0; i<0x100; i++)
        arg_8[i] = i;
    arg_8[0x100] = 0;
    arg_8[0x101] = 0;
    DWORD var_4 = 0x100;
    BYTE b_dl = 0;
    DWORD d_ebx = 0;
    do
    {
        DWORD d1 = arg_0[b_dl];
        DWORD d2 = *arg_8;
        d_ebx = (d1+d_ebx+d2)%0x100;
        //sub_40EC7D(arg_8, parg_8+d_ebx);   // *arg_8 <==> parg_8[d_ebx]
		BYTE tmp = arg_8[0];
		arg_8[0] = parg_8[d_ebx];
		parg_8[d_ebx] = tmp;
        b_dl = (b_dl+1) % arg_4;
        arg_8++;
    }while(--var_4!=0);
}

