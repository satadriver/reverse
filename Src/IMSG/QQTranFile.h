// QQTranFile.h: interface for the QQTranFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQTRANFILE_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_)
#define AFX_QQTRANFILE_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\\SocketSession.h"

class QQTranFileSession : public SocketSession
{
public:
	QQTranFileSession(DATEHEADER& packh, int aT);
	virtual ~QQTranFileSession();

	//*************************************************
	//	函数名称:		BfSession::InsertPacket
	//	描述:			处理一个会话的一个包
	//	输入参数:		packh 是包头结构
	//					pPacket 是包数据
	//	输出参数:		
	//	返回值:			
	//	其它:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

protected:
	void WriteQQFileIndexFile(CString filepath);
protected:


	//unsigned char m_bakbuf[2000];
	//int m_bakbuflen;
	
	CString m_tranfilename;
	CString m_tranfilepath;

	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
	DWORD  m_tranfilelen;
	DWORD  m_packetnum;
	DWORD m_recvpacketnum;
	UCHAR m_tranfilemd5[16];

	CFile  m_file;
	BOOL  m_bSaveFile;
	char * m_filterbuf;
};



inline int IsQQTranFilePacket(DATEHEADER& packh, const char* pPacket)
{
int error=0;
	try
	{
		if (packh.m_type ==TCPPacket)
		{
			if(packh.m_dport==443)
			{
error=1;
				if(packh.m_datalen>600 
					&& (BYTE)pPacket[0]==0x4 
					&&  memcmp(pPacket+4,"\x2c\x00",2)==0 
					&& (BYTE)pPacket[0x2c]==0x4 
					&& (BYTE)pPacket[0x2c-1]==0x3
					&& memcmp(pPacket+0x10,"\x0\x0\x0\x0",4)==0	 
				 )
				{
					 return 1;
				}
			}
			else //tcp 点对点
			{
error=2;
				if(packh.m_datalen==39 && memcmp(pPacket,"\x27\x00\x00\x00\x00\x14",6)==0 
					)
				{
					return 2; //点对点qq号
				}
				else  if(
					(packh.m_datalen==583 && memcmp(pPacket,"\x0c\x00\x00\x00",4)==0	&& memcmp(pPacket+12,"\x3b\x02\x00\x00",4)==0)
					|| (packh.m_datalen==587 && memcmp(pPacket,"\x0c\x00\x00\x00",4)==0	&& memcmp(pPacket+12,"\x3f\x02\x00\x00",4)==0)
					)//文件名
				{
					
					return 3;			
				}
			}

		}
		else  if(packh.m_type==UDPPacket)
		{
error=3;
			if(packh.m_datalen==27 && memcmp(pPacket,"\x03\x00",2)==0 
				&& memcmp(pPacket+6,"\x0\x0\x0\x0",4)==0 )
			{
				return 4;
			}
			else  if(
				(packh.m_datalen==567 || packh.m_datalen==571)
				&& pPacket[0]=='\x05'
				&& memcmp(pPacket+6,"\x0\x0",2)==0 
				&& memcmp(pPacket+0x4,"\x2b\x52",2)==0 //发送文件名
				)
			{
				return 5;
			}
			else if(packh.m_dport==8000 
				&& packh.m_datalen>0x40
				&& pPacket[0]==0x3
				&& pPacket[1]==0x60
				&& memcmp(pPacket+0x16,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16)==0
				&& memcmp(pPacket+0x38 ,"\x0b\x00",2)==0 )  //获取qq空间好友 03 60 ,单包处理,不再建session  
			{

				return 6;
			}
		

		}

		return 0;
	}
	catch (...)
	{
		WriteLog("dce", "IsQQTranFilePacket() Error!!=%d",error);

	}

}


#endif // !defined(AFX_QQTRANFILE_H__453996A7_AB78_498D_B9A7_6840ADF0444D__INCLUDED_)
