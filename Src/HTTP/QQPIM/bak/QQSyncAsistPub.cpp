#include "stdafx.h"
#include "defs.h"
#include "QQSyncAsistPub.h"






unsigned char TeaKey[256] = {0};

unsigned char unk_19578[] = {
0x44,0x46,0x47,0x00,0x00,0x00,0x00,0x3C,0x3E,0x3C,0x00,0x00,0x00,0x00,0x26,0x2A,
0x4D,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x06,0x00,0x03,0x00,
0x05,0x00,0x04,0x00,0x00 };

unsigned char  * unk_1957F = unk_19578 + 7;
unsigned char * unk_19586 = unk_19578 + 14;
unsigned char  * unk_1958E = unk_19578 + 22;

unsigned char unk_1A43C[] = {
0x78,0xA4,0x6A,0xD7,0x56,0xB7,0xC7,0xE8,0xDB,0x70,0x20,0x24,0xEE,0xCE,0xBD,0xC1,
0xAF,0x0F,0x7C,0xF5,0x2A,0xC6,0x87,0x47,0x13,0x46,0x30,0xA8,0x01,0x95,0x46,0xFD,
0xD8,0x98,0x80,0x69,0xAF,0xF7,0x44,0x8B,0xB1,0x5B,0xFF,0xFF,0xBE,0xD7,0x5C,0x89,
0x22,0x11,0x90,0x6B,0x93,0x71,0x98,0xFD,0x8E,0x43,0x79,0xA6,0x21,0x08,0xB4,0x49,
//1a47c
0x62,0x25,0x1e,0xf6,0x40,0xB3,0x40,0xC0,0x51,0x5A,0x5E,0x26,0xAA,0xC7,0xB6,0xE9,
0x5D,0x10,0x2F,0xD6,0x53,0x14,0x44,0x02,0x81,0xE6,0xA1,0xD8,0xC8,0xFB,0xD3,0xE7,
0xE6,0xCD,0xE1,0x21,0xD6,0x07,0x37,0xC3,0x87,0x0D,0xD5,0xF4,0xED,0x14,0x5A,0x45,
0x05,0xE9,0xE3,0xA9,0xF8,0xA3,0xEF,0xFC,0xD9,0x02,0x6F,0x67,0x8A,0x4C,0x2A,0x8D,
0x42,0x39,0xFA,0xFF,0x81,0xF6,0x71,0x87,0x22,0x61,0x9D,0x6D,0x0C,0x38,0xE5,0xFD,
0x44,0xEA,0xBE,0xA4,0xA9,0xCF,0xDE,0x4B,0x60,0x4B,0xBB,0xF6,0x70,0xBC,0xBF,0xBE,
0xC6,0x7E,0x9B,0x28,0xFA,0x27,0xA1,0xEA,0x85,0x30,0xEF,0xD4,0x05,0x1D,0x88,0x04,
0x39,0xD0,0xD4,0xD9,0xE5,0x99,0xDB,0xE6,0xF8,0x7C,0xA2,0x1F,0x65,0x56,0xAC,0xC4,
0x44,0x22,0x29,0xF4,0x97,0xFF,0x2A,0x43,0xA7,0x23,0x94,0xAB,0x39,0xA0,0x93,0xFC,
0xC3,0x59,0x5B,0x65,0x92,0xCC,0x0C,0x8F,0x7D,0xF4,0xEF,0xFF,0xD1,0x5D,0x84,0x85,
0x4F,0x7E,0xA8,0x6F,0xE0,0xE6,0x2C,0xFE,0x14,0x43,0x01,0xA3,0xA1,0x11,0x08,0x4E,
0x82,0x7E,0x53,0xF7,0x35,0xF2,0x3A,0xBD,0xBB,0xD2,0xD7,0x2A,0x91,0xD3,0x86,0xEB,
//1a53c
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x01,0x06,0x0B,0x00,0x05,0x0A,0x0F,0x04,0x09,0x0E,0x03,0x08,0x0D,0x02,0x07,0x0C,
0x05,0x08,0x0B,0x0E,0x01,0x04,0x07,0x0A,0x0D,0x00,0x03,0x06,0x09,0x0C,0x0F,0x02,
0x00,0x07,0x0E,0x05,0x0C,0x03,0x0A,0x01,0x08,0x0F,0x06,0x0D,0x04,0x0B,0x02,0x09,
//1a57c
0x07,0x0C,0x11,0x16,0x05,0x09,0x0E,0x14,0x04,0x0B,0x10,0x17,0x06,0x0A,0x0F,0x15,
0x98,0x2F,0x8A,0x42,0x91,0x44,0x37,0x71,0xCF,0xFB,0xC0,0xB5,0xA5,0xDB,0xB5,0xE9,
0x5B,0xC2,0x56,0x39,0xF1,0x11,0xF1,0x59,0xA4,0x82,0x3F,0x92,0xD5,0x5E,0x1C,0xAB,
0x98,0xAA,0x07,0xD8,0x01,0x5B,0x83,0x12,0xBE,0x85,0x31,0x24,0xC3,0x7D,0x0C,0x55,
0x74,0x5D,0xBE,0x72,0xFE,0xB1,0xDE,0x80,0xA7,0x06,0xDC,0x9B,0x74,0xF1,0x9B,0xC1,
0xC1,0x69,0x9B,0xE4,0x86,0x47,0xBE,0xEF,0xC6,0x9D,0xC1,0x0F,0xCC,0xA1,0x0C,0x24,
0x6F,0x2C,0xE9,0x2D,0xAA,0x84,0x74,0x4A,0xDC,0xA9,0xB0,0x5C,0xDA,0x88,0xF9,0x76,
0x52,0x51,0x3E,0x98,0x6D,0xC6,0x31,0xA8,0xC8,0x27,0x03,0xB0,0xC7,0x7F,0x59,0xBF,
0xF3,0x0B,0xE0,0xC6,0x47,0x91,0xA7,0xD5,0x51,0x63,0xCA,0x06,0x67,0x29,0x29,0x14,
0x85,0x0A,0xB7,0x27,0x38,0x21,0x1B,0x2E,0xFC,0x6D,0x2C,0x4D,0x13,0x0D,0x38,0x53,
0x54,0x73,0x0A,0x65,0xBB,0x0A,0x6A,0x76,0x2E,0xC9,0xC2,0x81,0x85,0x2C,0x72,0x92,
0xA1,0xE8,0xBF,0xA2,0x4B,0x66,0x1A,0xA8,0x70,0x8B,0x4B,0xC2,0xA3,0x51,0x6C,0xC7,

0x19,0xE8,0x92,0xD1,0x24,0x06,0x99,0xD6,0x85,0x35,0x0E,0xF4,0x70,0xA0,0x6A,0x10,
0x16,0xC1,0xA4,0x19,0x08,0x6C,0x37,0x1E,0x4C,0x77,0x48,0x27,0xB5,0xBC,0xB0,0x34,
0xB3,0x0C,0x1C,0x39,0x4A,0xAA,0xD8,0x4E,0x4F,0xCA,0x9C,0x5B,0xF3,0x6F,0x2E,0x68,
0xEE,0x82,0x8F,0x74,0x6F,0x63,0xA5,0x78,0x14,0x78,0xC8,0x84,0x08,0x02,0xC7,0x8C,
0xFA,0xFF,0xBE,0x90,0xEB,0x6C,0x50,0xA4,0xF7,0xA3,0xF9,0xBE,0xF2,0x78,0x71,0xC6};

unsigned char * unk_1A47C = unk_1A43C + 0x40;
unsigned char *  unk_1A53C = unk_1A43C + 0x100;
unsigned char *  unk_1A57C = unk_1A43C + 0x140;









int __fastcall sub_144C8(int result, unsigned int a2)		//52,packet size/4
{
  char v2  = 0; // nf@0
  int v3; // r12@1
  int v4; // r3@4
  char v5; // r0@8
  unsigned int v6; // r1@8
  unsigned int v7; // r2@8
  bool v8; // zf@17

  v3 = result ^ a2;

  if ( a2 <= 0 )
    a2 = -a2;

  if ( a2 == 1 )
  {
    if ( (v3 ^ result) < 0 )
      result = -result;
	return result;
  }
  else
  {
    v4 = result;
    if ( result < 0 )
      v4 = -result;

    if ( v4 <= a2 )
    {
      if ( v4 < a2 )
        result = 0;	

      if ( v4 == a2 )
        result = (v3 >> 31) | 1;
    }
    else if ( a2 & (a2 - 1) )
    {
      //v5 = __clz(a2) - __clz(v4);
	  __asm
	  {
		  pushad
		  mov eax,a2
		  bsr ecx,eax
		  mov edx,32
		  sub edx,ecx
		  sub edx,1
		  mov ebx,edx

		  mov eax,v4
		  bsr ecx,eax
		  mov edx,32
		  sub edx,ecx
		  sub edx,1
		  
		  sub ebx,edx
		  mov v5,bl
		  popad
	  }

      v6 = a2 << v5;
      v7 = 1 << v5;
      result = 0;
      while ( 1 )
      {
        if ( v4 >= v6 )
        {
          v4 -= v6;
          result |= v7;
        }
        if ( v4 >= v6 >> 1 )
        {
          v4 -= v6 >> 1;
          result |= v7 >> 1;
        }
        if ( v4 >= v6 >> 2 )
        {
          v4 -= v6 >> 2;
          result |= v7 >> 2;
        }
        if ( v4 >= v6 >> 3 )
        {
          v4 -= v6 >> 3;
          result |= v7 >> 3;
        }
        v8 = v4 == 0;
        if ( v4 )
        {
          v7 >>= 4;
          v8 = v7 == 0;
        }
        if ( v8 )
          break;
        v6 >>= 4;
      }
      if ( v3 < 0 )
        result = -result;
    }
    else
    {
		int Tmp = 0;
		__asm
		{
			pushad
			mov eax,a2
			bsr ecx,eax
			mov edx,32
			sub edx,ecx
			sub edx,1
			mov Tmp,edx
			popad
	  }
		result = (unsigned int)v4 >> (31 - Tmp);
      //result = (unsigned int)v4 >> (31 - __clz(a2));

      if ( result < 0 )
        result = -result;
    }
  }
  return result;
}






int __fastcall sub_144C0(int a1, int a2)			// 52, packet size
{
	if ( !a2 )
		return 0;
		//JUMPOUT(&loc_152A0);
	return sub_144C8(a1, a2);
}







int __fastcall sub_B764(unsigned int a1, unsigned int a2, int a3)			//a1 = key  a2 = 64, a3 = int [] return by c160
{
	int v3; // r4@1
	int v4; // r5@1
	int v5; // r3@1
	int v6; // r6@1
	int v7; // r3@1
	int v8; // r1@4
	unsigned int v9; // r2@4
	int v10; // r4@4
	int *v11; // r3@4
	int v12; // r2@5
	int v13; // r0@5
	int v14; // r1@5
	int v15; // r6@5
	int v16; // r3@5
	int v17; // r4@7
	int v18; // r4@8
	int v19; // r6@8
	int v20; // r3@8
	int v21; // r2@10
	char v22; // r6@10
	int v23; // r2@10
	int v24; // r2@10
  int v25; // r3@11
  int v26; // r5@11
  int v27; // r0@13
  int v28; // r5@13
  int v29; // r0@13
  int v30; // r3@14
  int v31; // r5@14
  int v32; // r1@16
  int v33; // r5@16
  int v34; // r1@16
  int result; // r0@18
  int v36; // r2@20
  int v37; // r1@20
  int v38; // r3@20
  int v39; // r4@20

  int v41; // [sp+4h] [bp+4h]@5
  int v42; // [sp+8h] [bp+8h]@3
  int v43; // [sp+Ch] [bp+Ch]@3
  int v44; // [sp+10h] [bp+10h]@1
  int v45; // [sp+14h] [bp+14h]@11
  int v46; // [sp+18h] [bp+18h]@3
  int v47; // [sp+1Ch] [bp+1Ch]@1
  int v48; // [sp+20h] [bp+20h]@1
  int v49; // [sp+24h] [bp+24h]@1
  int v50; // [sp+28h] [bp+28h]@1
  unsigned int v51; // [sp+2Ch] [bp+2Ch]@3
  int v52; // [sp+30h] [bp+30h]@7
  int v53; // [sp+34h] [bp+34h]@3
  int v54[17]; // [sp+38h] [bp+38h]@19			//////////////////////////////////////////////////////////////////////////


  v44 = a1 + 4 * (a2 >> 2);
  v3 = *(_DWORD *)(a3 + 4);
  v4 = *(_DWORD *)(a3 + 8);
  v50 = *(_DWORD *)a3;
  v5 = *(_DWORD *)(a3 + 16);
  v6 = *(_DWORD *)(a3 + 12);
  v49 = v3;
  v7 = a2 + v5;
  v48 = v4;
  v47 = v6;
  *(_DWORD *)(a3 + 16) = v7;
  if ( v7 < a2 )
    ++*(_DWORD *)(a3 + 20);
  v51 = a1;
  v46 = (int)unk_1A53C;		//////////////////////////////////////////////////////////////////////////
  v53 = (int)&v54[16];		//////////////////////////////////////////////////////////////////////////
  v43 = (int)unk_1A57C;		//////////////////////////////////////////////////////////////////////////
  v42 = a3;

  int  Tmp = (int)v54;

  while ( 1 )
  {
    result = v44;
    if ( v51 >= v44 )
      break;
    v9 = v51;
    v11 = v54;
    do
    {
      v8 = *(_DWORD *)v9;
      v9 += 4;
      v10 = v53;
      *v11 = v8;
      ++v11;			//将16字节数据和接下来的原始密钥拷贝到此处
    }
    while ( v11 != (int *)v10 );
    v12 = v47;
    v51 += 64;
    v13 = v48;
    v14 = v49;
    v15 = v50;
    v16 = 0;
    v41 = (int)unk_1A53C;
    while ( 1 )
    {
		
      v52 = (v12 ^ v13) & v14 ^ v12;			//顺序正确
      v52 += *((_DWORD *)unk_1A43C + v16) + v15 + *(DWORD*)((DWORD*)Tmp + *(_BYTE *)(v16 + v41) );			//////////////////////////////////////////////
      v17 = __ROR4__(v52, 32 - *(_BYTE *)(v41 + 64 + (v16++ & 3)));
      if ( v16 == 16 )
        break;
      v15 = v12;
      v12 = v13;
      v13 = v14;
      v14 += v17;
    }
    v18 = v17 + v14;
    v19 = v12;
    v20 = 0;
    while ( 1 )
    {
      v21 = ((v14 ^ v18) & v13 ^ v14)
          + v19
          + *((_DWORD *)unk_1A43C + v20 + 16)
          + *(DWORD*)((DWORD*)Tmp + *(_BYTE *)(v46 + v20 + 16));
      v22 = *(_BYTE *)(v43 + (v20++ & 3) + 4);
      v23 = __ROR4__(v21, 32 - v22);
      v24 = v23 + v18;
      if ( v20 == 16 )
        break;
      v19 = v13;
      v13 = v14;
      v14 = v18;
      v18 = v24;
    }
    v45 = (int)unk_1A57C;
    v25 = 0;
    v26 = v13;
    while ( 1 )
    {
      v27 = (v18 ^ v24 ^ v14) + v26 + *((_DWORD *)unk_1A43C + v25 + 32);
      v52 = v27;
      v52 = v27 + *(DWORD*)((DWORD*)Tmp + *((_BYTE *)unk_1A53C + v25 + 32) );
      LOBYTE1(v27) = *(_BYTE *)(v45 + (v25++ & 3) + 8);
      v28 = __ROR4__(v52, 32 - v27);
      v29 = v28 + v24;
      if ( v25 == 16 )
        break;
      v26 = v14;
      v14 = v18;
      v18 = v24;
      v24 = v29;
    }
    v45 = (int)unk_1A57C;
    v30 = 0;
    v31 = v14;
    while ( 1 )
    {
      v32 = ((~v18 | v29) ^ v24) + v31 + *((_DWORD *)unk_1A43C + v30 + 48);
      v52 = v32;
      v52 = v32 + *(DWORD*)((DWORD*)Tmp + *((_BYTE *)unk_1A53C + v30 + 48) );
      LOBYTE1(v32) = *(_BYTE *)(v45 + (v30++ & 3) + 12);
      v33 = __ROR4__(v52, 32 - v32);
      v34 = v33 + v29;
      if ( v30 == 16 )
        break;
      v31 = v18;
      v18 = v24;
      v24 = v29;
      v29 = v34;
    }
    v50 += v18;
    v49 += v34;
    v48 += v29;
    v47 += v24;
  }

  //////////////////////////////////////////////////////////////////////////
  v36 = v42;
  v37 = v50;
  v38 = v49;
  v39 = v48;
  *(_DWORD *)(v42 + 12) = v47;
  *(_DWORD *)v36 = v37;
  *(_DWORD *)(v36 + 4) = v38;
  *(_DWORD *)(v36 + 8) = v39;
  return result;
}












int __fastcall sub_C160(int result)
{
  *(_DWORD *)result = 1732584193;
  *(_DWORD *)(result + 4) = -271733879;
  *(_DWORD *)(result + 8) = -1732584194;
  *(_DWORD *)(result + 12) = 271733878;
  *(_DWORD *)(result + 20) = 0;
  *(_DWORD *)(result + 16) = 0;
  *(_DWORD *)(result + 24) = 0;

  //C190拷贝原始密钥到后面的字节中
  return result;
}




//将原始密钥传入C160返回的数组中的第28开始的字节中
void *__fastcall sub_C190(void *result, int a2, unsigned int a3)	//int[] return by c160 ,key,keylen
{
  int v3; // r6@1
  int v4; // r5@3
  int v5; // r5@6
  unsigned int v6; // r4@7
  int v7; // r5@7
  unsigned int v8; // [sp+4h] [bp+4h]@1
  const void *src; // [sp+8h] [bp+8h]@1
  int n; // [sp+Ch] [bp+Ch]@1
  int na; // [sp+Ch] [bp+Ch]@4

  v3 = (int)result;
  src = (const void *)a2;
  v8 = a3;
  n = a3;
  if ( a3 << 26 )
  {
    v4 = *((_DWORD *)result + 6);
    if ( v4 )
    {
      na = a3;
      if ( a3 > 128 - v4 )
        na = 128 - v4;
      result = memcpy((char *)result + v4 + 28, (const void *)a2, na);
      v5 = na + v4;
      *(_DWORD *)(v3 + 24) += na;
      if ( (unsigned int)v5 > 0x40 )
      {
        v6 = v3 + (v5 & 0xFFFFFFC0);
        sub_B764(v3 + 28, v5 & 0xFFFFFFC0, v3);
        v7 = v5 & 0x3F;
        result = memcpy((void *)(v3 + 28), (const void *)(v6 + 28), v7);
        *(_DWORD *)(v3 + 24) = v7;
      }
      src = (char *)src + na;
      n = v8 - na;
    }
    if ( n > 64 )
    {
      result = (void *)sub_B764((unsigned int)src, n & 0xFFFFFFC0, v3);
      src = (char *)src + (n & 0xFFFFFFC0);
      n &= 0x3Fu;
    }
    if ( n > 0 )
    {
      result = memcpy((void *)(v3 + 28), src, n);
      *(_DWORD *)(v3 + 24) = n;
    }
  }
  else
  {
    result = (void *)sub_B764(a2, a3, (int)result);
  }
  return result;
}




//低位是C160 C190制造的密钥，高位指向一个整数 a2是一个整数
__int64 __fastcall sub_C250(__int64 a1, int a2)		//完成密钥的生成和复制，参数为两个地址
{
  int _8; // ST08_4@1
  unsigned int v3; // r5@1
  int v4; // r4@1
  int v5; // r3@1
  int v6; // r6@1
  signed int v7; // r3@3
  int v8; // r5@5
  int v9; // r3@5
  __int64 _0; // [sp+0h] [bp+0h]@1

  _0 = a1;
  _8 = a2;
  v3 = *(_DWORD *)(a1 + 24);		//原始密钥长度21//
  v4 = a1;
  v5 = v3 + *(_DWORD *)(a1 + 16);	//21
  v6 = HIDWORD(a1);					//数组
  *(_DWORD *)(a1 + 16) = v5;		//0
  if ( v5 < v3 )
    ++*(_DWORD *)(a1 + 20);
  v7 = 120;
  if ( v3 <= 0x37 )
    v7 = 56;
  HIDWORD(_0) = v7 - v3;				//35
  memset((void *)(a1 + v3 + 28), 0, v7 - v3);
  *(_BYTE *)(v4 + v3 + 28) = -128;
  v8 = HIDWORD(_0) + v3;
  v9 = v4 + v8;
  *(_DWORD *)(v9 + 28) = 8 * *(_DWORD *)(v4 + 16);
  *(_DWORD *)(v9 + 32) = *(_QWORD *)(v4 + 16) >> 29;
  sub_B764(v4 + 28, v8 + 8, v4);
  *(_DWORD *)v6 = *(_DWORD *)v4;							//返回真实的密钥 key, 64 ,16 bytes params
  *(_DWORD *)(v6 + 4) = *(_DWORD *)(v4 + 4);
  *(_DWORD *)(v6 + 8) = *(_DWORD *)(v4 + 8);
  *(_DWORD *)(v6 + 12) = *(_DWORD *)(v4 + 12);
  return _0;
}





int __fastcall sub_C2C4(const void *a1, unsigned int a2, void *a3)		//a1== key, a2 == keylen, a3 == int[] 
{
  void *v3; // r6@1
  unsigned int v4; // r5@1
  int result; // r0@1
  unsigned __int64 v6; // r2@1
  __int64 v7; // r0@3
  void * v8 = a3; // r2@3			//V8初始化为A3
  const void *src; // [sp+4h] [bp+4h]@1
  int v10[0x100]; // [sp+8h] [bp+8h]@3										//////////////////////////////////////////////////////////////////////////


  v3 = a3;
  src = a1;
  v4 = a2;
  //v11 = _stack_chk_guard;
  result = (int)memset(a3, 0, 0x10);
  v6 = __PAIR__((unsigned int)src, (unsigned int)src) - __PAIR__((unsigned int)((char *)src - 1), 1);	//此处编译有问题
  if ( (signed int)v4 > 16 && HIDWORD(v6) )
  {
    sub_C160((int)v10);				//////////////////////////////////////////////////////////////////////////
    sub_C190(v10, (int)src, v4);	//////////////////////////////////////////////////////////////////////////
    LODWORD(v7) = (unsigned int)v10;
    HIDWORD(v7) = (unsigned int)v3;
    result = sub_C250(v7, (int)v8);			//低位是C160 C190制造的密钥，根据汇编V8指向a3
  }
  else if ( (signed int)v4 > 0 && HIDWORD(v6) )
  {
    result = (int)memcpy(v3, src, v4);
  }
//   if ( v11 != _stack_chk_guard )
//     _stack_chk_fail(result);
  return result;
}
// 14FC: using guessed type int __fastcall _stack_chk_fail(_DWORD);











unsigned int __fastcall sub_C340(void *src, size_t n, int a3, int a4, void *dest, int a6)
{
  unsigned int v6; // r6@11
  int v7; // r4@8
  unsigned int v8; // r3@9
  unsigned int v9; // r12@9
  char *v10; // r2@10
  int v11; // r0@11
  int v12; // r4@11
  int v13; // r6@11
  int v14; // r1@12
  signed int v15; // r3@15
  int v16; // r4@16
  bool v17; // cf@16
  unsigned int v19; // [sp+0h] [bp+0h]@11
  int v20; // [sp+4h] [bp+4h]@9
  unsigned int v21; // [sp+8h] [bp+8h]@11
  char *v22; // [sp+Ch] [bp+Ch]@9
  unsigned int v23; // [sp+10h] [bp+10h]@10
  int v24; // [sp+14h] [bp+14h]@9
  int v25; // [sp+18h] [bp+18h]@1
  int v26; // [sp+1Ch] [bp+1Ch]@12
  
  int v27[0x100] = {0}; // [sp+20h] [bp+20h]@8			//此处应该是个数组

	//v19 = (int)(v27-8);
	//memmove((char*)v35,"\xf1\x5e\x78\xd2\x30\x33\xc2\xc4\xba\xc8\xa0\x25\x67\x7e\x86\x3a",16);
  v6 = n;
  v26 = a3;
  v25 = a4;
  if ( (signed int)n <= 0 || !src )
    return 0;
  if ( n << 30 )			//长度为4的倍数
    return -32227;
  if ( !dest )
    return v6;
  if ( a6 < (signed int)n )
    return -32227;
  if ( dest != src )							//这里肯定相等
    memcpy(dest, src, n);
  sub_C2C4((const void *)v26, v25, v27);		//v27 is int[]
  v26 = v6 >> 2;
  v25 = (v6 >> 2) - 1;
  v7 = *(_DWORD *)dest;
  if ( v25 <= 0 )
  {
    v15 = 3;
    do
    {
      v16 = __ROR4__(v7 - (*((int*)v27 + v15) ^ 0x9E3779B9), 16);
      v7 = v16 ^ 0x79B99E37;
      v17 = (unsigned int)v15-- >= 1;
    }
    while ( v17 );
    *(_DWORD *)dest = v7;
  }
  else
  {
    v24 = sub_144C0(52, v26);
    v8 = -1640531527 * (v24 + 6);
    v9 = v6;
    v22 = (char *)dest + 4 * v25;
    v20 = 4 * (v26 + 0x3FFFFFFF);
    while ( v24 != -6 )
    {
      v23 = (v8 >> 2) & 3;
      v10 = (char *)dest + v20;
      v26 = v25;
      do
      {
        v21 = (*(v27 + (v26 & 3 ^ v23) ) ^ *((_DWORD *)v10 - 1)) + (v7 ^ v8);
        v19 = ((unsigned int)v7 >> 3) ^ 16 * *((_DWORD *)v10 - 1);
        v11 = (v19 + (4 * v7 ^ (*((_DWORD *)v10 - 1) >> 5))) ^ v21;
        v12 = *(_DWORD *)v10;
        --v26;
        v13 = v26;
        v7 = v12 - v11;
        *(_DWORD *)v10 = v7;
        v10 -= 4;
      }
      while ( v13 );
      v7 = *(_DWORD *)dest
         - (((16 * *(_DWORD *)v22 ^ ((unsigned int)v7 >> 3)) + ((*(_DWORD *)v22 >> 5) ^ 4 * v7)) ^ ((v7 ^ v8)
                                                                                                  + (*(_DWORD *)v22 ^ *(v27 + v23))));
      v14 = v24 - 1;
      *(_DWORD *)dest = v7;
      v8 += 1640531527;
      v24 = v14;
    }
    v6 = v9;
  }

  return v6;
}




unsigned int __fastcall sub_C49C(void *a1, int a2, int a3, int a4, void *a5, int a6)		
{
  unsigned int result; // r0@1
  int v7; // r3@3

  result = sub_C340(a1, a2, a3, a4, a5, a6);
  if ( (signed int)result > 0 && a5 )
  {
    v7 = *(_DWORD *)((char *)a5 + result - 4);
    if ( v7 < 0 || (signed int)(result - 3) <= v7 )
    {
      result = -32227;
    }
    else
    {
      *((_BYTE *)a5 + v7) = 0;
      result = v7;
    }
  }
  return result;
}











	
//src == dest == packet, a2== a6 == pack len, a3 = key, a4 = keylen
int __fastcall sub_C4D8(void *src, int a2, int a3, int a4, void *dest, int a6)	
{
  int v6; // r5@3
  size_t i; // r2@8
  unsigned int v8; // r3@11
  char *v9; // r6@11
  unsigned int v10; // r5@11
  unsigned int v11; // r3@12
  void *v12; // r1@13
  unsigned int v13; // r3@13
  int v14; // r2@13
  unsigned int v15; // r12@13
  int v16; // r0@14
  int v17; // r3@14
  int v18; // r3@14
  unsigned int v19; // r0@14
  int v20; // r0@14
  unsigned int v21; // r2@15
  int v22; // r1@15
  int *v23; // r2@18
  int v24; // r6@19
  int v25; // r3@19

  unsigned int v28; // [sp+4h] [bp+4h]@14
  size_t v29; // [sp+8h] [bp+8h]@12
  unsigned int v30; // [sp+Ch] [bp+Ch]@14
  unsigned int v31; // [sp+10h] [bp+10h]@13
  int v32; // [sp+14h] [bp+14h]@1
  size_t v33; // [sp+18h] [bp+18h]@1
  size_t n; // [sp+1Ch] [bp+1Ch]@1
  int v35[0x10] = {0}; // [sp+20h] [bp+20h]@11



  //memmove((char*)v35,"\xf1\x5e\x78\xd2\x30\x33\xc2\xc4\xba\xc8\xa0\x25\x67\x7e\x86\x3a",16);
  n = a2;
  v33 = a3;
  v32 = a4;
  if ( a2 > 0 && src )
  {
    v6 = (a2 + 3) & 0xFFFFFFFC;
    if ( dest )
    {
      if ( a6 >= v6 )
      {
        if ( dest != src )
          memcpy(dest, src, n);
        for ( i = n; (signed int)i < v6; ++i )
          *((_BYTE *)dest + i) = 0;
        n = i;
        sub_C2C4((const void *)v33, v32, v35);
        v8 = *(_DWORD *)dest;
        v9 = (char *)dest + 4 * ((n >> 2) - 1);
        v33 = (n >> 2) - 1;
        v10 = *(_DWORD *)v9;
        if ( (signed int)v33 <= 0 )
        {
          v23 = v35;
          do
          {
            v24 = *v23;
            ++v23;
            v25 = __ROR4__(v8, 16);
            v8 = (v24 ^ 0x9E3779B9) + (v25 ^ 0x9E3779B9);
          }
          while ( v23 != &v35[4] );
          *(_DWORD *)dest = v8;
        }
        else
        {
          v32 = sub_144C0(52, n >> 2);
          v11 = 0;
          v29 = v33 & 3;
          while ( v32 != -6 )
          {
            v12 = dest;
            v13 = v11 - 1640531527;		//0xffffffff9E3779B9
            v31 = (v13 >> 2) & 3;
            v14 = 0;
            v15 = v13;
            do
            {
				//v27 = 0;
              v16 = *(DWORD*)(v35 + (v14 & 3 ^ v31) );
              v17 = *(_DWORD *)((_DWORD *)v12 + 1);
              ++v14;
              v30 = v16 ^ v10;
              v18 = (v16 ^ v10) + (v17 ^ v15);
              v19 = *((_DWORD *)v12 + 1);
              v30 = v18;
              v28 = 16 * v10 ^ (v19 >> 3);
              v10 = ((v28 + ((v10 >> 5) ^ 4 * v19)) ^ v18) + *(_DWORD *)v12;
              v20 = v33;
              *(_DWORD *)v12 = v10;
              v12 = (char *)v12 + 4;
            }
            while ( v14 < v20 );
            v21 = *(_DWORD *)dest;
            v11 = v15;
            v30 = (*(_DWORD *)dest >> 3) ^ 16 * v10;
            v10 = ((v30 + (4 * v21 ^ (v10 >> 5))) ^ ((v10 ^ *(v35 + (v31 ^ v29) )) + (v21 ^ v15))) + *(_DWORD *)v9;
            v22 = v32;
            *(_DWORD *)v9 = v10;
            v32 = v22 - 1;
          }
        }
		
        v6 = n;
      }
      else
      {
        v6 = -32226;
      }
//	  Utf8ToGB2312((char*)dest,v6);
    }
  }
  else
  {
    v6 = 0;
  }
  return v6;
}




int __fastcall sub_C62C(void *src, int a2, int a3, int a4, void *dest, int a6)
{
  int v6; // r6@1
  int v7; // r5@1
  int i; // r1@5
  int v10; // [sp+8h] [bp+0h]@1
  int v11; // [sp+Ch] [bp+4h]@1

  v10 = a4;
  v6 = a2;
  v11 = a3;
  v7 = ((a2 + 3) & 0xFFFFFFFC) + 4;
  if ( dest )
  {
    if ( a6 < v7 )
    {
      v7 = -1;
    }
    else
    {
      if ( dest != src )
        memcpy(dest, src, a2);
      for ( i = v6; i < v7; ++i )
        *((_BYTE *)dest + i) = 0;
      *((_DWORD *)dest + (i >> 2) - 1) = v6;
      v7 = sub_C4D8(dest, i, v11, v10, dest, i);
    }
  }
  return v7;
}




int  __fastcall sub_183C(char * str,int strlen,char * key, int keylen, char * decbuf, int decbuflen,int Flag)
{

	int Cnt = 0;
	if ( Flag )
		Cnt = sub_C62C(str, strlen, (int)key, keylen, decbuf, decbuflen);
	else
		Cnt = sub_C49C(str, strlen, (int)key, keylen, decbuf, decbuflen);

	if (Cnt <= 0)
	{
		return 0;
	}

	return Cnt;
}

/*
void __cdecl New_sub_16f4()
{
	__asm
	{
		//push edi
		//push esi
		//push ebx
		push ebp
		mov ebp,esp
		sub esp,0x7c
		
		push 64
		push 0
		push offset TeaKey
		call memset
		add esp ,12

		push dword ptr 0x7c
		push dword ptr 0
		push esp
		call memset
		add esp,12
		add esp,0x20

		mov ebx, esp
		add ebx ,8
		
		push 4
		push offset unk_19578
		mov eax, ebx
		add eax,0x1f
		push eax
		call memcpy
		add esp,12


		mov eax,0x252423
		mov [ebx + 0x54 - 0x3c], eax			
		mov eax, offset unk_19578
		mov [ebx + 0x54 - 0x50],eax		
		mov ax,0x235e
		mov [ebx + 0x54 - 0x2e],ax			
		mov ax,0x25
		mov [ebx + 0x54 - 0x2c],ax		

		push 4
		mov eax,offset unk_19578
		add eax, 7
		push eax
		mov eax,ebx
		add eax,0x2d
		push eax
		call memcpy
		add esp,12

		mov eax,0x475224
		mov [ebx + 0x54 - 0x20],eax

		push 4
		mov eax,offset unk_19578
		add eax,0x0e
		push eax
		mov eax,ebx
		add eax,0x3b
		push eax
		call memcpy
		add esp,12


		mov ax,0x5248
		mov [ebx + 0x42],ax
		mov ax,0x28
		mov [ebx + 0x42 + 2],ax

		push 0x0e
		mov eax,offset unk_19578
		add eax, 0x16
		push eax
		mov eax,ebx
		add eax,8
		push eax
		call memcpy
		add esp,12

		sub esp,0x20
		//push 0x20
		//push 0
		//push esp
		//call memset
		//add esp,12

		mov esi,esp
		add esi,8

		lea esi,TeaKey
		
		push esi

		mov [ebx + 0x54 - 0x50],esi
		mov edi,ebx
		add edi,0x18


		//r0 =esp - 0x20 + 8 == esi
		mov edx,0
		SetKey:
		push esi

		mov ecx,8
		add ecx,edx
		movzx eax,word ptr [ebx + ecx]
		add edx,2

		mov ecx,eax
		shl eax,1
		add eax,ecx
		add esi,eax

		mov cl,[edi]
		mov [esi ],cl
		mov cl,[edi+1]
		mov [esi + 1],cl
		mov cl, [edi + 2]
		mov [esi + 2],cl
		add edi,7

		pop esi
		cmp edx, 0x0e
		jnz SetKey

		pop eax
		mov esp,ebp
		pop ebp
	
		//pop ebx
		//pop esi
		//pop edi	
		//retn 0
	}

	return ;
}

*/




int AscToHex(unsigned char * Asc,int AscLen, unsigned char * Hex)
{
	unsigned char Table[] = "0123456789abcdef";
	unsigned int Index = 0;
	unsigned int Cnt = 0;
	unsigned int Counter = 0;
	unsigned char Tmp = 0;
	
	for (Cnt = 0; Cnt < AscLen; )
	{
		for (Index = 0; Index < 16; Index ++)
		{
			if ( Asc[Cnt] == Table[Index] )
			{
				Tmp = Index << 4;
			}
		}
		
		Cnt ++;
		
		for (Index = 0; Index < 16; Index ++)
		{
			if ( Asc[Cnt] == Table[Index] )
			{
				Tmp = Tmp | Index;
			}
		}
		
		Cnt ++;
		
		Hex[Counter] = Tmp;
		Counter ++ ;
	}
	
	return Counter;
}
