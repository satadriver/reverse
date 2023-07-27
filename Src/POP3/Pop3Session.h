// Pop3Session.h: interface for the Pop3Session class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_)
#define AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

class Pop3Session : public SocketSession  
{
public:
	Pop3Session(DATEHEADER& packh, int aT, BOOL isServer);
	virtual ~Pop3Session();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	DWORD			m_CSeqOffset	;
	DWORD			m_CAckOffset	;
	DWORD			m_UserSeq;
	DWORD			m_PassSeq;
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
	CString			m_from;
	CString			m_to;
private:
	// 邮件还原相关的函数(实现在Pop3Session.cpp)

	// 处理链表中的包
	int Pop3LstToEml(char * buf, int bufsize);

	// 查找下一个邮件头
	BOOL Pop3NextSeqSession();
	
	// 获得该邮件的结束序列号
	static UINT GetemlEndSeq(const char *buf, UINT seq);

	// 清除中间变量
	void Clear();

	// 创建实体文件名
	BOOL CreateFilename();

	// 保存实体文件
	int SaveEmlTmpFile(int flg, char* pData, int datlen);

	// 邮件解析相关的函数(实现在Pop3MailFile.cpp中)

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

CString SplitEmlName(CString emlname, CString username, BOOL isfrom);


#endif // !defined(AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_)
