//////////////////////////////////////////////////////////////////
//																//
//		用途 : RLE 压缩算法	(PCX, TGA图像)						//
//		创建 : [Foolish] / 2000-9-6								//
//		更新 : 2002-4-7											//
//		主页 :													//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_RLE_COMPRESS_H__
#define		__FOO_RLE_COMPRESS_H__
#include <windows.h>
#pragma once

//====================================================================
//	RLE - 压缩算法
//====================================================================
//////////////////////////////////////////////////////////////////////
//	功  能 ：RLE压 / 解压缩 ( PCX, TGA 图像 )						//
//																	//
//	参  数 ：iColorBit 为像素颜色位数								//
//					PCX为 ：1, 4, 8, 24								//
//					TGA为 ：8, 16, 24, 32							//
//			 iNumPixel 为写入OutBuffer中的像素个数					//
//																	//
//	返回值 ：EncodeLine 返回编码后 OutBuffer 的指针					//
//			 DecodeLine 返回解码后 InBuffer 的指针					//
//																	//
//	说  明 ：PCX图像 ：1,4,8位色位图按字节处理，24位色分成3通道处理	//
//			 TGA图像 ：支持8, 16, 24, 32色, 不支持15位色			//
//			 压缩时OutBuffer至少两倍InBuffer大小					//
//////////////////////////////////////////////////////////////////////
BYTE * RLE_PCX_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;
BYTE * RLE_PCX_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel, // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_EncodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;
BYTE * RLE_TGA_DecodeLine (BYTE * InBuffer, int iColorBit,
						   int iNumPixel,  // iNumPixel即为图像宽
						   BYTE * OutBuffer) ;

//===================================================================
//	Implement
//===================================================================
inline BYTE *  RLE_PCX_DecodeLine (BYTE * InBuffer, int iColorBit,
								   int iNumPixel, BYTE * OutBuffer) {
	register BYTE	  Data, Num, Pack = 8 / iColorBit ;
	if (iColorBit <= 8) // 1,4,8位色
		while (iNumPixel > 0)
		{
			Data = *InBuffer++ ; // Next Byte
			if ( (Data & 0xC0) == 0xC0 ) // Data >= 0xC0
			{
				Num = Data & 0x3F ; // 重复字节
				memset (OutBuffer, *InBuffer++, Num) ;
				OutBuffer += Num ; iNumPixel -= Num * Pack ;
			}
			else
			{
				*OutBuffer++ = Data ;
				iNumPixel -= Pack ;
			}
		}
	else // 24位色分Channel
		for (int i = 2 ; i >= 0 ; i--) // RGB channel
		{
			int		iCount = iNumPixel ;
			BYTE	* pChannel = OutBuffer + i ;
			while (iCount-- > 0) // 一定会写入一个像素(--)
			{
				Data = *InBuffer++ ; // Next Byte
				if ( (Data & 0xC0) == 0xC0 ) // Data >= 0xC0
				{
					Num = Data & 0x3F ; // 重复像素
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
								  int iNumPixel,  // iNumPixel即为图像宽
								  BYTE * OutBuffer) {
	register BYTE	data, cCount ;
	int 			PcxRow = 2 * ( (iNumPixel * (iColorBit % 16) + 15) / 16 ) ;
	if (iColorBit <= 8) // 1,4,8位色
		while (PcxRow > 0)
		{
			cCount = 1 ; data = *InBuffer++ ; PcxRow-- ;
			while ( (cCount < 0x3F) && (PcxRow != 0) )
				if (*InBuffer != data)// 统计重复字节个数
					break ;
				else
				{
					cCount++ ; InBuffer++ ; PcxRow-- ;
				}

			if (cCount == 1) // 无重复像素
				if ( (data & 0xC0) == 0xC0 ) // Data >= 0xC0
				{
					*OutBuffer++ = 0xC1 ;
					*OutBuffer++ = data ;
				}
				else
					*OutBuffer++ = data ; // Data < 0xC0, 直接写字节
			else // 重复像素
			{
				*OutBuffer++ = 0xC0 | cCount ;
				*OutBuffer++ = data ;
			}
		} // 1,4,8位色结束
	else // 24位色分Channel
		for (int i = 2 ; i >= 0 ; i--) // RGB channel
		{
			DWORD	RowBak = PcxRow ; // 每Channel字节数
			BYTE	* pChannel = InBuffer + i ;
			while (RowBak > 0)
			{
				cCount = 1 ; data = *pChannel ; pChannel += 3 ; RowBak-- ;
				while ( (cCount < 0x3F) && (RowBak != 0) )
					if (*pChannel != data) // 统计重复字节个数
						break ;
					else
					{
						cCount++ ; pChannel += 3 ; RowBak-- ;
					}

				if (cCount == 1) // 无重复像素
					if ( (data & 0xC0) == 0xC0 ) // Data >= 0xC0
					{
						*OutBuffer++ = 0xC1 ;
						*OutBuffer++ = data ;
					}
					else
						*OutBuffer++ = data ;
				else // 重复像素
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
	iColorBit /= 8 ; // 转换为字节数
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
		else // 直接copy Num个像素
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

	iColorBit /= 8 ; // 转换为字节数
	while (iNumPixel > 0)
	{
		pBak = InBuffer ; // 记数指针
		memcpy (&Data, InBuffer, iColorBit) ; // 第一个像素
		InBuffer += iColorBit ; iNumPixel-- ; Count = 1 ;
		while ((Count < 0x7F) && (iNumPixel > 0)) // 统计重复像素
		{
			memcpy (&Next, InBuffer, iColorBit) ; // 下一个像素
			if (Next != Data)
				break ;
			InBuffer += iColorBit ; iNumPixel-- ; Count++ ;
		}

		if (Count == 1) // 无重复像素
		{
			while ((Count < 0x7F) && (iNumPixel > 0)) // 统计不重复像素
			{
				Count++ ; Data = Next ;
				InBuffer += iColorBit ; iNumPixel-- ;
				memcpy (&Next, InBuffer, iColorBit) ; // 下一个像素
				if (Data == Next)
					break ;
			}
			// 直接copy不重复像素
			*OutBuffer++ = (BYTE)(Count - 1) ;
			Count = InBuffer - pBak ; // Count->临时变量
			memcpy (OutBuffer, pBak, Count) ;
			OutBuffer += Count ;
		}
		else // 重复像素
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