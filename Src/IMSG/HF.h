//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	HF
//	版本:		1.0
//	创建日期:	2007-7-11
//	作者:		荣强
//	主要功能:	浩方游戏的处理
//	主要函数:	IsHfPacket
//	模块描述:	
//**************************************************************** 

#ifndef	INCLUDE_HF_H_20070711
#define INCLUDE_HF_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	类名称:			HfSession
//	类继承:			SocketSession
//	描述:			浩方游戏的处理类
//	主要函数:		InsertPacket
//	使用事项:		
//**************************************************
class HfSession : public SocketSession
{
public:
	HfSession(DATEHEADER& packh, int aT);
	virtual ~HfSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	
public:
	void WriteHfIndexFile(time_t optm);
	void FormatHfPacket(DATEHEADER& packh, const char* pPacket);
		
	int			m_infonum;		// 处理结果()
	int			m_acctype;		// 帐号类型
	CString		m_password;
};


//*************************************************
//	函数名称:		IsHfPacket
//	描述:			检查一个包是否是浩方的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是浩方的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsHfPacket(DATEHEADER& packh, const char* pPacket)
{
	if( (memcmp(pPacket, "\x28\x0\x0\x0", 4) == 0) ||
		(memcmp(pPacket, "\xb0\x01\x0\x0", 4) == 0) ||
		(memcmp(pPacket, "\x68\x04\x0\x0", 4) == 0))
	{
		return TRUE;
	}
	else
		return FALSE;
}


#endif // !defined(AFX_HF_H__EFD7986D_26CD_436F_999D_8D32BC451F86__INCLUDED_)
