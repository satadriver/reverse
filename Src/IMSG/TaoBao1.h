// TaoBao.h: interface for the TaoBao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_)
#define AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	类名称:			TaoBaoSession
//	类继承:       	SocketSession
//	描述:			TaoBao聊天的还原类
//	主要函数:		
//	使用事项:		
//**************************************************

class TaoBaoSession : public SocketSession
{
public:
	TaoBaoSession(DATEHEADER& packh, int aT);
	virtual ~TaoBaoSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
private:
	int FormatLoginOrOut(DATEHEADER& packh, const char* pPacket);
	int FormatMsg(DATEHEADER& packh, const char* pPacket,int Direction);//方向1发向服务器，2服务器返回

	// 处理VP的聊天
//	int FormatVpIM(DATEHEADER& packh, const char* pPacket);

	// 处理用户登录的包
//	int UserLogin(DATEHEADER& packh, const char* pPacket);

	// 获得聊天信息
//	CString GetImsg(const char* pPacket);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);
};

//*************************************************
//	函数名称:		IsTaoBaoPacket
//	描述:			检查一个包是否是VP的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是VP的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsTaoBaoPacket(DATEHEADER& packh, const char* pPacket)
{
// 	if (memcmp(pPacket,"\x8f\x01\x01\x00",4) == 0 &&
// 		packh.m_datalen > 0x50 &&
// 		(memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||	//上线
// 		memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||	//下线
// 		memcmp(pPacket+0x48,"\x01\x2b",2) == 0 ||	//聊天给服务器的包命令号 01 2b
// 		memcmp(pPacket+0x48,"\x01\x30",2) == 0))	//聊天服务器给客户端的包命令号 01 30 
	if ((memcmp(pPacket,"\x8f\x01\x01\x00",4) == 0 ||
		memcmp(pPacket,"\x88\x06\x00\x00\x00\x00\x00",7) == 0 )&&
		packh.m_datalen > 0x50 &&
		(memcmp(pPacket+24,"\x03\x06\x00\x01",4) == 0||
		memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||	//上线
		memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||	//下线
		memcmp(pPacket+0x48,"\x01\x2b",2) == 0 ||	//聊天给服务器的包命令号 01 2b
		memcmp(pPacket+0x48,"\x01\x30",2) == 0))	//聊天服务器给客户端的包命令号 01 30 

	{
		return TRUE;
	}
	return FALSE;
}

#endif // !defined(AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_)
