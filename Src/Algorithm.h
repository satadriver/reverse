//***************************************************************
//	Copyright (C) 2007 - 
//	�ļ���: 	Algorithm
//	�汾:		1.0
//	��������:	2008-2-23
//	����:		��ǿ
//	��Ҫ����:	���ֳ��õ��㷨(BASE64, ����ƥ��, CRC32)
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************** QQTEA�㷨 ****************************
#include "QQ_Crypt.h"


//����ƥ���㷨
#define MAXCHARS 256

class Search
{
public:
    BYTE d[MAXCHARS];
    int m;
    char patt[MAXCHARS];
    Search(const char*);
    Search();
	void InitSearch(const char *);

    int find(const char*);
	int find(const char*,int len);
};

// CRS32�㷨
class CCrc32  
{
public:
	CCrc32();
	~CCrc32();
	int GetCRC(LPCTSTR csData, DWORD dwSize); // Creates a CRC from LPCTSTR
	CString GetEmlCrc(LPCTSTR filename);

private:
	ULONG	m_crc32Table[256];	// Lookup table arrays
	
	void	InitCRC32Table();	// Builds Lookup table array
	inline  ULONG	Reflect(ULONG ref, char ch); // Reflects CRC bits in the lookup table
	BOOL SplicEmail(const char *emlFile, CString &strHead, CString &strBody);
};


// BASE64�㷨
class CMailCoder
{
public:
	static unsigned int m_LineWidth;						//ָ�������ÿ�еĳ��ȣ�ȱʡ��76
	static const char BASE64_ENCODE_TABLE[64];				//Base64�����
	static const unsigned int BASE64_DECODE_TABLE[256];		//Base64�����
	static const unsigned char QUOTED_ENCODE_TABLE[256];	//Quoted�����
private:
	static unsigned char DecToHex(unsigned char B);		//ΪQuoted������������ַ�ת��
	static unsigned char HexToDec(unsigned char C);		//ΪQuoted������������ַ�ת��
public:
	static int Base64EncodeSize(int iSize);		//�����ļ�ʵ�ʳ��Ȼ�ȡ����Base64��ĳ���  
	static int Base64DecodeSize(int iSize);		//�����ѱ����ļ����Ȼ�ȡBase64�Ľ��볤��
	static int UUEncodeSize(int iSize);			//�����ļ�ʵ�ʳ��Ȼ�ȡUUCode�����ĳ���
	static int UUDecodeSize(int iSize);			//�����ѱ����ļ����Ȼ�ȡUUCode�����ĳ���
	static int QuotedEncodeSize(int iSize);		//����ʵ���ļ��ĳ��Ȼ�ȡQuoted����

	/*
	*  ��һ��Buffer����Base64����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� Base64EncodeSize(int) ����ȡ��
	*/
	static int base64_encode(char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  ��һ��Buffer����Base64����
	*	
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� Base64DecodeSize(int) ����ȡ��
	*/
	static int  base64_decode(char *pSrc, unsigned int nSize, char *pDest);

	/*
	*  ��һ��Buffer����UUCODE����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� UUEncodeSize(int) ����ȡ��
	*/
	static int UU_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  ��һ��Buffer����UUCODE����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� UUDecodeSize(int) ����ȡ��
	*/
	static int UU_decode(char *pSrc, unsigned int nSize, char *pDest);
	/*
	*  ��һ��Buffer����Quoted-Printble����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע: ���Buffer�ĳ��ȿ���ʹ�� QuotedDecodeSize(int) ����ȡ��
	*/
	static int quoted_encode(char *pSrc, unsigned int nSize, char *pDest);
	
	/*
	*  ��һ��Buffer����Quoted-Printble����
	*
	*	 	pSrc	����Buffer
	*		nSize	Buffer����
	*		pDest	�������
	*		return	������ʵ�ʳ���
	*
	*	 ע������û���ṩ������볤�ȵķ��� ֱ��ʹ������Buffer�����Buffer�Ϳ�����
	*/
	static int quoted_decode(char *pSrc, unsigned int nSize, char *pDest);
	
};
CString int10str_to_hexstr(const char *s,int len = 0);
CString hexstrtoint10str(const char *s, char *outbuf);


#endif
