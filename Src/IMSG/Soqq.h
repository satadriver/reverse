//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Soqq
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	Soqq即时消息还原
//	主要函数:	IsSoQQPacket
//**************************************************************** 

#ifndef		INCLUDE_AFX_SOQQ_H_20070712
#define		INCLUDE_AFX_SOQQ_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

#define		SOQQSTAT_EMLTOID	1	// 邮箱换ID
#define		SOQQSTAT_ONLINE		2	// 上线
#define		SOQQSTAT_OFFLINE	3	// 下线
#define		SOQQSTAT_SENDMSG	4	// 发送消息


//*************************************************
//	类名称:			SoqqSession
//	类继承:       	SocketSession
//	描述:			Soqq即时消息会话类
//	主要函数:		
//	使用事项:		
//**************************************************
class SoqqSession : public SocketSession
{
public:
	SoqqSession(DATEHEADER& packh, int aT);

	virtual ~SoqqSession();

	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

public:
	void FormatSoqqPacket(DATEHEADER& packh, const char* pPacket);

	void OutputFile(time_t optm, LPCTSTR optxt, int direction);

	int			m_direction;
	CString		m_SoqqEml;
	CString		m_Colloquist;
	SimpleSeq	m_Seq;

};

//*************************************************
//	函数名称:		IsSoQQPacket
//	描述:			检查一个包是否是SoQQ的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是SoQQ的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsSoQQPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_datalen > 12)
		&& ((packh.m_type == TCPPacket) || (packh.m_type == ProxyTCP)))
	{
		DWORD datlen = 0;
		BYTE* p = (BYTE*)pPacket;
		for (int i = 0; i < 10; i++)
		{
			if ((*p==0x12)&&(p[2]==0)/*&&(p[3]==1)*/)
			{
				WORD len = Swap2Byte(*((WORD*)(p+6)));
				if (len < 8)
				{
					return FALSE;
				}
				datlen += len;
				if (datlen == packh.m_datalen)
				{
					return TRUE;
				}
				if (datlen > packh.m_datalen)
				{
					return FALSE;
				}
				p += len;
			}
			else
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}


#endif	// INCLUDE_AFX_SOQQ_H_20070712