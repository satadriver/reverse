// TeSpread.h: interface for the CTeSpread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESPREAD_H__EA352FF6_9D49_4A49_9E53_0C2D966A1B6A__INCLUDED_)
#define AFX_TESPREAD_H__EA352FF6_9D49_4A49_9E53_0C2D966A1B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!\fn isFreegate686p
 * \brief 检查是否为自由之门686p特征
*/
BOOL isFreegate686p(const unsigned char* pPacket, DATEHEADER& packh);

/*!\fn isFreegate689b3
 * \brief 检查是否为自由门689正式版测试版3 搜索服务器包特征
*/
BOOL isFreegate689b3(const unsigned char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);


BOOL isFreegate7(const unsigned char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);

BOOL isFreegate740(const unsigned char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);

 

#endif // !defined(AFX_TESPREAD_H__EA352FF6_9D49_4A49_9E53_0C2D966A1B6A__INCLUDED_)
