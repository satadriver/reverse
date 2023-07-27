// ImapSession.h: interface for the ImapSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_)
#define AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

class ImapSession  : public SocketSession  
{
public:
	ImapSession(DATEHEADER& packh, int aT, BOOL isServer);
	virtual ~ImapSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);

public:
	DWORD			m_CSeqOffset	;
	DWORD			m_CAckOffset	;
	int				m_FLAG		;	// 邮件还原状态:
									// 取值范围: NOEML / EMLPROCING / EMLOVER
	int				m_sortflag	;	// 0:  表示有序, 其他值表示无序
	BOOL			m_mailflag	;
	int				m_cntMail;		// 会话中的邮件数
	int				m_emlfilecount;	// 还原出的邮件个数
	UINT			m_nseq		;	//指下一个包的seq
	unsigned int	m_fileseq		;
	unsigned int	m_endseq		;	//crlf.crlf
	CString			m_emlfield		;
	TcpPacketList	m_PacketList	;	// 数据包链表
	CString			m_PASS	;
	char*			m_buffer;
	int				m_datlen;

private:
	// 查找下一个邮件头
	BOOL FindImapEmlHead();
	// 处理链表中的包
	int Pop3LstToEml(char * buf, int bufsize);
	// 创建实体文件名
	BOOL CreateFilename();
	// 保存实体文件
	int SaveEmlTmpFile(int flg, char* pData, int datlen);
	// 清除中间变量
	void Clear();

	//////////////////////////////////////////////////////////////////////////
		// 拆解邮件(邮件属性, 正文和附件)
	int StrippingMail(CString& filestr);

	// 提取邮件属性
	int ExtractMailHeader(IMessagePtr& iMsg);

	// 提取邮件正文并生成文本附加文件
	int ExtractMailText(IMessagePtr& iMsg);

	// 邮件输出相关的函数(实现在Pop3MailFile.cpp中)

	// 输出一封邮件
	void DumpEmlFile();

};

#endif // !defined(AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_)
