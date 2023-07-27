
#if !defined(AFX_SMTPSESSION_H__A30620CE_38E9_41C6_A4A9_E39599C8291D__INCLUDED_)
#define AFX_SMTPSESSION_H__A30620CE_38E9_41C6_A4A9_E39599C8291D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

#define		SmtpBufPacketCount		100

/*************************************************
	类名称:			SmtpSession
	类继承:			SocketSession
	描述:			还原Smtp协议
	主要属性:		
	主要函数:		
	使用事项:		
*************************************************/
class SmtpSession : public SocketSession  
{
public:
	SmtpSession(DATEHEADER& packh, int aT);
	virtual ~SmtpSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	DWORD			m_CSeqOffset;
	DWORD			m_CAckOffset;
	DWORD			m_UserSeq	;
	DWORD			m_PassSeq	;
	int				m_FLAG		;	// 邮件还原状态:
									// 取值范围: NOEML / EMLPROCING / EMLOVER
	BOOL			m_mailflag	;
	int				m_sortflag	;	// 0: 表示有序	其他值表示无序
	UINT			m_nseq		;	// 指下一个包的seq
	DWORD			m_fileseq	;
	TcpPacketList	m_Packetlst	;	// 数据包链表
	char*			m_buffer	;
	int				m_datlen	;
//	CString			m_USER		;
	CString			m_PASS		;
	CString			m_emlfield	;
	CString			m_from		;
	CString			m_to		;

private:
	// 邮件还原相关的函数(实现在SmtpSession.cpp)

	// 处理链表中的包
	int SmtpLstToEml(char * buf, int bufsize);

	// 查找下一个邮件头
	BOOL SMTPNextSeqSession();

	// 清除中间变量
	void Clear();

	// 创建实体文件名
	BOOL CreateFilename();

	// 保存实体文件
	int SaveEmlTmpFile(int flg, char* pData, int datlen);

	// 获取对象的用户名和密码
	void GetSmtpPass(DATEHEADER& packh, const char* pPacket);

	// 邮件解析相关的函数(实现在SmtpMailFile.cpp中)

	// 拆解邮件(邮件属性, 正文和附件)
	int StrippingMail(CString& filestr);

	// 提取邮件属性
	int ExtractMailHeader(IMessagePtr& iMsg);

	// 提取邮件正文并生成文本附加文件
	int ExtractMailText(IMessagePtr& iMsg);

	// 邮件输出相关的函数(实现在SmtpMailFile.cpp中)

	// 输出一封邮件
	void DumpEmlFile();
};


CString SplitEmlName(CString emlname,CStringArray* arrto,BOOL isfrom);

#endif