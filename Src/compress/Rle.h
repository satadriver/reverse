//////////////////////////////////////////////////////////////////
//																//
//		��; : RLE ѹ���㷨	(PCX, TGAͼ��)						//
//		���� : [Foolish] / 2000-9-6								//
//		���� : 2002-4-7											//
//		��ҳ :													//
//		���� : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= ����	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_RLE_COMPRESS_H__
#define		__FOO_RLE_COMPRESS_H__
#include <windows.h>
#pragma once

//====================================================================
//	RLE - ѹ���㷨
//====================================================================
//////////////////////////////////////////////////////////////////////
//	��  �� ��RLEѹ / ��ѹ�� ( PCX, TGA ͼ�� )						//
//																	//
//	��  �� ��iColorBit Ϊ������ɫλ��								//
//					PCXΪ ��1, 4, 8, 24								//
//					TGAΪ ��8, 16, 24, 32							//
//			 iNumPixel Ϊд��OutBuffer�е����ظ���					//
//																	//
//	����ֵ ��EncodeLine ���ر���� OutBuffer ��ָ��					//
//			 DecodeLine ���ؽ���� InBuffer ��ָ��					//
//																	//
//	˵  �� ��PCXͼ�� ��1,4,8λɫλͼ���ֽڴ���24λɫ�ֳ�3ͨ������	//
//			 TGAͼ�� ��֧��8, 16, 24, 32ɫ, ��֧��15λɫ			//
//			 ѹ��ʱOutBuffer��������InBuffer��С					//
//////////////////////////////////////////////////////////////////////
BYTE * RLE_PCX_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;
BYTE * RLE_PCX_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel, // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel��Ϊͼ���
						   BYTE * OutBuffer) ;

//===================================================================
//	Implement
//===================================================================
inline BYTE *  RLE_PCX_DecodeLine (BYTE * InBuffer, int iColorBit,
								   int iNumPixel, BYTE * OutBuffer) {
	register BYTE	  Data, Num, Pack = 8 / iColorBit ;
	if (iColorBit <= 8) // 1,4,8λɫ
		while (iNumPixel > 0)
		{
			Data = *InBuffer++ ; // Next Byte
			if ( (Data & 0xC0) == 0xC0 ) // Data >= 0xC0
			{
				Num = Data & 0x3F ; // �ظ��ֽ�
				memset (OutBuffer, *InBuffer++, Num) ;
				OutBuffer += Num ; iNumPixel -= Num * Pack ;
			}
			else
			{
				*OutBuffer++ = Data ;
				iNumPixel -= Pack ;
			}
		}
	else // 24λɫ��Channel
		for (int i = 2 ; i >= 0 ; i--) // RGB channel
		{
			int		iCount = iNumPixel ;
			BYTE	* pChannel = OutBuffer + i ;
			while (iCount-- > 0) // һ����д��һ������(--)
			{
				Data = *InBuffer++ ; // Next Byte
				if ( (Data & 0xC0) == 0xC0 ) // Data >= 0xC0
				{
					Num = Data & 0x3F ; // �ظ�����
					for (int x = 0 ; x < Num ; x++, pChannel += 3)
						*pChannel = *InBuffer ;
					InBuffer++ ;
					iCount -= Num - 1 ;
				}
				else
				{	*pChannel = Data ; pChannel += 3 ;	}
			}
		}
	return InBuffer ;
}
//===================================================================
inline BYTE * RLE_PCX_EncodeLine (BYTE * InBuffer, int iColorBit,
								  int iNumPixel,  // iNumPixel��Ϊͼ���
								  BYTE * OutBuffer) {
	register BYTE	data, cCount ;
	int 			PcxRow = 2 * ( (iNumPixel * (iColorBit % 16) + 15) / 16 ) ;
	if (iColorBit <= 8) // 1,4,8λɫ
		while (PcxRow > 0)
		{
			cCount = 1 ; data = *InBuffer++ ; PcxRow-- ;
			while ( (cCount < 0x3F) && (PcxRow != 0) )
				if (*InBuffer != data)// ͳ���ظ��ֽڸ���
					break ;
				else
				{
					cCount++ ; InBuffer++ ; PcxRow-- ;
				}

			if (cCount == 1) // ���ظ�����
				if ( (data & 0xC0) == 0xC0 ) // Data >= 0xC0
				{
					*OutBuffer++ = 0xC1 ;
					*OutBuffer++ = data ;
				}
				else
					*OutBuffer++ = data ; // Data < 0xC0, ֱ��д�ֽ�
			else // �ظ�����
			{
				*OutBuffer++ = 0xC0 | cCount ;
				*OutBuffer++ = data ;
			}
		} // 1,4,8λɫ����
	else // 24λɫ��Channel
		for (int i = 2 ; i >= 0 ; i--) // RGB channel
		{
			DWORD	RowBak = PcxRow ; // ÿChannel�ֽ���
			BYTE	* pChannel = InBuffer + i ;
			while (RowBak > 0)
			{
				cCount = 1 ; data = *pChannel ; pChannel += 3 ; RowBak-- ;
				while ( (cCount < 0x3F) && (RowBak != 0) )
					if (*pChannel != data) // ͳ���ظ��ֽڸ���
						break ;
					else
					{
						cCount++ ; pChannel += 3 ; RowBak-- ;
					}

				if (cCount == 1) // ���ظ�����
					if ( (data & 0xC0) == 0xC0 ) // Data >= 0xC0
					{
						*OutBuffer++ = 0xC1 ;
						*OutBuffer++ = data ;
					}
					else
						*OutBuffer++ = data ;
				else // �ظ�����
				{
					*OutBuffer++ = 0xC0 | cCount ;
					*OutBuffer++ = data ;
				}
			}
		}
	return OutBuffer ;
}
//===================================================================
inline BYTE * RLE_TGA_DecodeLine (BYTE * InBuffer, int iColorBit,
								  int iNumPixel, BYTE * OutBuffer) {
	register BYTE	  Data ;
	register int	  Num ;
	iColorBit /= 8 ; // ת��Ϊ�ֽ���
	while (iNumPixel > 0)
	{
		Data = *InBuffer++ ; // Next Byte
		if (Data & 0x80) // Data >= 0x80
		{
			iNumPixel -= (Num = (Data & 0x7F) + 1) ;
			for (int i = 0 ; i < Num ; i++, OutBuffer += iColorBit)
				memcpy (OutBuffer, InBuffer, iColorBit) ;
			InBuffer += iColorBit ;
		}
		else // ֱ��copy Num������
		{
			iNumPixel -= (Num = ++Data) ;
			Num *= iColorBit ;
			memcpy (OutBuffer, InBuffer, Num) ;
			OutBuffer += Num ;
			InBuffer += Num ;
		}
	} // End of while
	return InBuffer ;
}
//===================================================================
inline BYTE * RLE_TGA_EncodeLine (BYTE * InBuffer, int iColorBit,
								  int iNumPixel, BYTE * OutBuffer) {
	DWORD			Data, Next ;
	BYTE			* pBak ;
	register DWORD	Count ;

	iColorBit /= 8 ; // ת��Ϊ�ֽ���
	while (iNumPixel > 0)
	{
		pBak = InBuffer ; // ����ָ��
		memcpy (&Data, InBuffer, iColorBit) ; // ��һ������
		InBuffer += iColorBit ; iNumPixel-- ; Count = 1 ;
		while ((Count < 0x7F) && (iNumPixel > 0)) // ͳ���ظ�����
		{
			memcpy (&Next, InBuffer, iColorBit) ; // ��һ������
			if (Next != Data)
				break ;
			InBuffer += iColorBit ; iNumPixel-- ; Count++ ;
		}

		if (Count == 1) // ���ظ�����
		{
			while ((Count < 0x7F) && (iNumPixel > 0)) // ͳ�Ʋ��ظ�����
			{
				Count++ ; Data = Next ;
				InBuffer += iColorBit ; iNumPixel-- ;
				memcpy (&Next, InBuffer, iColorBit) ; // ��һ������
				if (Data == Next)
					break ;
			}
			// ֱ��copy���ظ�����
			*OutBuffer++ = (BYTE)(Count - 1) ;
			Count = InBuffer - pBak ; // Count->��ʱ����
			memcpy (OutBuffer, pBak, Count) ;
			OutBuffer += Count ;
		}
		else // �ظ�����
		{
			*OutBuffer++ = 0x80 | (BYTE)--Count ;
			memcpy (OutBuffer, &Data, iColorBit) ;
			OutBuffer += iColorBit ;
		}		
	} // End of while
	return OutBuffer ;
}
//===================================================================
#endif