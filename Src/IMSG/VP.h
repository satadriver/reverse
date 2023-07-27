//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	VP
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	VP聊天的还原
//	主要函数:	IsVpPacket
//**************************************************************** 

#ifndef INCLUDE_AFX_VP_H_20070712
#define INCLUDE_AFX_VP_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	类名称:			VPIMSession
//	类继承:       	SocketSession
//	描述:			VP聊天的还原类
//	主要函数:		
//	使用事项:		
//**************************************************
class VPIMSession : public SocketSession
{
public:
	VPIMSession(DATEHEADER& packh, int aT);
	virtual ~VPIMSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
private:

	// 处理VP的聊天
	int FormatVpIM(DATEHEADER& packh, const char* pPacket);

	// 处理用户登录的包
	int UserLogin(DATEHEADER& packh, const char* pPacket);

	// 获得聊天信息
	CString GetImsg(const char* pPacket);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);
};

//*************************************************
//	函数名称:		IsVpPacket
//	描述:			检查一个包是否是VP的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是VP的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsVpPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen > 12)
	{
		if (stricmp(pPacket+12, "ClientText") == 0)		// 聊天信息
			return TRUE;
		if (stricmp(pPacket+12, "SystemMessage") == 0)	// 系统消息 (用于区分客户端)
			return TRUE;
		if (stricmp(pPacket+12, "VerifyUser") == 0)		// 登陆 
			return TRUE;
	}
	return FALSE;
}


#endif // !defined(AFX_VP_H__50708853_14F3_4CFD_93B9_9CE547C28E90__INCLUDED_)
