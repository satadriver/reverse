// Https.cpp: implementation of the Https class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "http.h"
#include "Https.h"
// #include <afxtempl.h>

#include   <openssl/x509.h>
//#include   <openssl/x509v3.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using   namespace   std;

#include "FreeGate757.h"

HttpsSession::HttpsSession(DATEHEADER& packh, int aT, BOOL bC2S)
: SocketSession(aT, packh)
{
	// 初始化IP
	if(bC2S)
	{
		memcpy(m_csip, packh.m_sip,4);
		memcpy(m_cdip, packh.m_dip,4);
		this->m_dip.Format("%03d.%03d.%03d.%03d_%05d",\
			packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3],packh.m_dport);
		this->m_sip.Format("%03d.%03d.%03d.%03d_%05d",\
			packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3],packh.m_sport);
		FormatMac(packh.m_mac, m_smac, m_dmac, packh);
	}
	else
	{
		memcpy(m_csip, packh.m_dip,4);
		memcpy(m_cdip, packh.m_sip,4);
		this->m_dip.Format("%03d.%03d.%03d.%03d_%05d",\
			packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3],packh.m_sport);
		this->m_sip.Format("%03d.%03d.%03d.%03d_%05d",\
			packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3],packh.m_dport);
		FormatMac(packh.m_mac, m_smac, m_dmac, packh);
	}

	m_closetm = packh.m_tm + 60;	// 1分钟可也
	m_begfiletm = 0;
	m_cerlen = 0;
	m_havelen = 0;
	m_certext = NULL;

	m_needseq = 0;
}

HttpsSession::~HttpsSession()
{

}

BOOL isHttps(const char* pPacket, DATEHEADER& packh)
{
	int Direct = 0;
	DWORD xorlen = 0;
	unsigned char *pdata = (unsigned char*)pPacket;
	DWORD datalen = packh.m_datalen;
	// 16 vv vv ll ll 01(clienthello)/02(serverhello) 
	// TLS1为0301,SSL3为0300，SSL2为0002
	if( pdata[0] == 0x16)
	{
		if( memcmp(pdata+1, "\x3\x1", 2) == 0 
			|| memcmp(pdata+1, "\x3\x0", 2) == 0 
			|| memcmp(pdata+1, "\x0\x2", 2) == 0 
			)
		{
			if(pdata[5] == 1)
				Direct = 1;
			else if(pdata[5] == 2)
				Direct = 2;
		}
	}
	return Direct ;
// 		
// 	BOOL bneedDeal = FALSE;
// 	BOOL bSession = FALSE;		// 是否需建会话多包处理
// 	if (pdata[0] == 0x16 && pdata[5] == 0xb && datalen > 15)
// 	{
// 		// 16 vv vv ll ll 0b 3bytelen 3bytelen onelen(3byte) firstcer
// 		{
// 			int chk1 = pdata[6];
// 			chk1 = (chk1<<8)+pdata[7];
// 			chk1 = (chk1<<8)+pdata[8];
// 			int chk2 = pdata[9];
// 			chk2 = (chk2<<8)+pdata[10];
// 			chk2 = (chk2<<8)+pdata[11];
// 			if(chk1 == chk2+3)
// 			{
// 				//////////////////
// 				xorlen = pdata[12];
// 				xorlen = (xorlen<<8)+pdata[13];
// 				xorlen = (xorlen<<8)+pdata[14];
// 				if(xorlen <= (datalen-15))	// 15=头5字节 0b 3个长度9字节
// 				{
// 					memcpy(buf,pdata+15,xorlen);
// 					x509len = xorlen;
// 					bneedDeal = TRUE;
// 					bSession = FALSE;
// 				}
// 				else
// 				{
// 					// 多包, 需会话
// 					printf("find multi packet ssl cer\n");
// 					bSession = TRUE;
// 					bneedDeal = FALSE;
// 				}
// 			}
// 		}
// 	}
// 
// 	if(bneedDeal)
// 	{
// 		HttpsSession https(packh, g_httpFrom);
// 		https.m_dceprocotol=m_dceprocotol;
// 		https.SetCerBuf(buf, x509len);
// 		https.CloseSession(0);
// // 		if(https.InsertPacket(packh, pPacket) == 8)
// // 		{
// // 			https.CloseSession();
// // 		}
// 	}
// 
// 	return bSession;
}

BOOL HttpsSession::DealOneCerBuf()
{
	ASSERT(m_cerlen >0 && m_certext );

	// 输出.cer
	if (m_realfname.IsEmpty())
	{
		CString pro;
		pro.Format("https");

		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_realfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.cer",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum, pro);
	}
#ifdef _DEBUG
			CString cerName;
			cerName.Format("%s%02u_%s.cer", 
				m_dceprocotol->m_httpMissObj->GetTmpPath(), g_httpFileNum, m_sip.Left(15));
			CFile fpCer;
			if(fpCer.Open(cerName, CFile::modeCreate|CFile::modeWrite))
			{
				fpCer.WriteHuge(m_certext, m_cerlen);
				fpCer.Close();
			}
#endif
	FILE* fp = fopen(m_realfname, "ab");
	if (fp)
	{
		fwrite(m_certext, 1, m_cerlen, fp);
		fclose(fp);
	}

	// 提取信息
	X509   *x   =   0;
	int x509len = m_cerlen;
	unsigned char *p=(unsigned char*)m_certext;
	d2i_X509(&x,(const   unsigned   char   **)&p,x509len);
	if(x == NULL)
	{
//		printf("analy fail\n");
		return -2;
	}
	X509_NAME   *subject   =   X509_get_subject_name(x);
	char *title = X509_NAME_oneline(subject, 0, 0);
	char text[2048];
	memset(text,0,2048);
	Parsehttps(title, text);

//	printf("title %s\n", title);
	if (strstr(text,"Amazon.com, Inc."))
	{
		FreeGate757 *fg757 = FreeGate757::GetInstance();
		CString dip = this->m_sip;
		int tmppos = dip.Find("_");
		if (tmppos != -1)
		{
			dip = dip.Mid(0, tmppos);
			fg757->Insert(this->m_dip,time(NULL));
		}
	}

	// psiphon 翻墙软件
	if (strstr(text, "ssl328456.cloudflaressl.com") || strstr(text, "a248.e.akamai.net"))
	{
		CString pro = "PSIPHON";
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum, pro);
		
		WriteIndexFile(pro, pro, "state=登录\r\n", m_dceprocotol->m_httpMissObj);
	}
	// Path 聊天软件
	if (strstr(text, "*.crashlytics.com") || strstr(text, "*.path.com"))
	{
		CString pro = "IMSG_PATH";
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum, pro);
		
		WriteIndexFile(pro, pro, "state=登录\r\n", m_dceprocotol->m_httpMissObj);
		
		/*
		IMPath *impath = IMPath::GetInstance();
		CString ip = this->m_sip;
		int canum = 0;
		if (strstr(text, "*.crashlytics.com"))
		{
			canum = 1;
		}
		else if(strstr(text, "*.path.com"))
		{
			canum = 2;
		}

		if (impath->Insert(ip, time(NULL), canum))
		{
			CString pro = "IMSG_PATH";
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
			m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay,
				now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_httpFileNum, pro);
			
			WriteIndexFile(pro, pro, m_idxtext, m_dceprocotol->m_httpMissObj);
		}
		*/
	}

	subject   =   X509_get_issuer_name(x);
	title = X509_NAME_oneline(subject, 0, 0);
//	printf("issuer %s\n", title);
	char issuer[500];
	memset(issuer,0,500);
	Parissuer(title, issuer);

// 	HttpsSession https(packh, g_httpFrom);
	m_begfiletm = time(0);//packh.m_tm;
	char   buffer[1024];
	memset(buffer,0,1024);
	ASN1_BIT_STRING   *pubkey   =   X509_get0_pubkey_bitstr(x);   //得到pubkey
// 	https.m_certext = (const char*)buf;
// 	https.m_cerlen = x509len;
	X509_free(x);
	m_spyaddr = g_DDEPOS;
	unsigned char MD51[16];
	MD5Tran(MD51, (void *)m_certext, x509len);
	CString tmp;
	tmp = int10str_to_hexstr((const char *)MD51,16);

	m_idxtext.Format("%s%smd5=%s\r\n",text,issuer,tmp);
//WriteLog("dce","md5:%s,x509len=%d",https.m_idxtext,x509len);
// 	https.CloseSession();
	return TRUE;
}

BOOL HttpsSession::SetCerBuf( unsigned char *buf, int len )
{
	ASSERT(m_certext == NULL);
	if(m_certext)
	{
		delete []m_certext;
		m_certext = NULL;
	}
	m_certext = new char[len+1];
	memcpy(m_certext, buf, len);
	m_cerlen = len;
	m_havelen = len;
	
	return TRUE;
}

BOOL HttpsSession::CloseSession(time_t now)
{
	try	{

		LPCTSTR pro;
		pro = _T("https");
		// 写实体文件
		if (m_certext != NULL && Ready())
		{
			DealOneCerBuf();	// 处理证书, 获得证书信息

			m_OpenCaseFlg = TRUE;
			m_OpenWarnFlg = TRUE;
			m_OpenIndxFlg = v_openflaghttps;
			// 匹配案件、预警和屏蔽信息
			DWORD dwstate = 0;
			// userid在实体文件
			SearchCaseOrWarn(this, "", 0, dwstate);

			//VOXER -- TEMP
			/*
			if (m_idxtext.Find("md5=786d00c99a4b1f7109d43acf26129c89") != -1)
			{
				CString voxer = "state= 上线\r\n";
				SYSTEMTIME now;
				GetLocalTime(&now);
				CString tmppath = m_dceprocotol->m_httpMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				
		
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_%s.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
					m_sip, m_dip, rand(),"imsg_voxer");
				WriteIndexFile("imsg_voxer","imsg_voxer",voxer,m_dceprocotol->m_httpMissObj);
			}
*/
			if (!m_realfname.IsEmpty() && GetFileLength(m_realfname) > 0)
			{
				// 写对照文件
				
				m_indexfname = m_realfname + ".indexfile.txt";
				WriteIndexFile(pro, pro, m_idxtext, m_dceprocotol->m_httpMissObj);
				
				
			}
		}

		if(m_certext)
		{
			// 清缓存
			delete []m_certext;
			m_cerlen=0;
			m_certext = NULL;
		}
	}
	catch(...)
	{
		WriteLog(HTTPPRO, "HttpsSession Session() Error!!");
	}
	return TRUE;	// 一个会话只保持5分钟
}

int HttpsSession::InsertPacket( DATEHEADER& packh,const char *pPacket )
{
	int error=0;
	try
	{
		BakProData(PRO_HTTPS, packh, pPacket);

		if(memcmp(packh.m_sip, this->m_csip, 4) == 0)	// 是客户端发出的包
		{
			// 客户端发出的证书, 忽略, 仅处理服务器下发的证书还原
			return TRUE;
		}
		
		if(m_cerlen == 0)
		{
			ASSERT(m_certext == NULL);
			m_needseq = packh.m_sequence+packh.m_datalen;	// 期待的下一包
			error=1;
			// 首包
			DWORD xorlen = 0;
			unsigned char *pdata = (unsigned char*)pPacket;
			DWORD datalen = packh.m_datalen;
			while(datalen > 5)
			{
				// 16 vv vv ll ll 0b(证书) 
				if( pdata[0] == 0x16 && pdata[5] != 0xb)
				{
					xorlen = pdata[3]*256+pdata[4];
					if(xorlen >= datalen)
					{
						break;
					}
					else
					{
						// 如果有, 跳过Server Hello
						pdata += (5+xorlen);
						datalen -= (5+xorlen);
					}
				}
				else
				{
					break;
				}
			}
			error=2;		
			BOOL bneedDeal = FALSE;
			BOOL bSession = TRUE;		// 是否需建会话多包处理
			if (pdata[0] == 0x16 && pdata[5] == 0xb && datalen > 15)
			{
				// 16 vv vv ll ll 0b 3bytelen 3bytelen onelen(3byte) firstcer
				xorlen = pdata[12];
				xorlen = (xorlen<<8)+pdata[13];
				xorlen = (xorlen<<8)+pdata[14];
				error=3;		
				if (xorlen<datalen)
				{
					error=31;		
					m_cerlen = xorlen;
					m_certext = new char[xorlen];
					error=32;		
					if(xorlen <= (datalen-15))	// 14=头5字节 0b 3个长度9字节
					{
						m_havelen = xorlen;
						memcpy(m_certext,pdata+15,xorlen);
					}
					else
					{
						m_havelen = datalen-15;
						memcpy(m_certext, pdata+15, datalen-15);
					}
				}
			}
		}
		else
		{
			error=4;
			ASSERT(m_certext != NULL);
			if(packh.m_sequence == m_needseq)
			{
				error=5;
				m_needseq = packh.m_sequence+packh.m_datalen;	// 期待的下一包
				// 是期望的包
				if(packh.m_datalen+m_havelen > m_cerlen)
				{
					memcpy(m_certext+m_havelen, pPacket, m_cerlen-m_havelen);
					m_havelen = m_cerlen;
				}
				else
				{
					memcpy(m_certext+m_havelen, pPacket, packh.m_datalen);
					m_havelen += packh.m_datalen;
				}
			}
		}
		error=6;
		if(Ready())
		{
			ASSERT(m_cerlen == m_havelen);
			
			return 8;		// 立即结束
		}
		
	}
	catch (...) 
	{
		WriteLog(HTTPPRO, "https InsertPacket Error!!=%d",error);
	}
	return TRUE;
}

void Parsehttps(LPCTSTR strLine, char* text)
{
	char *tp = (char*)strLine;
	char *p = tp;
	char *befor;
	int c = 0;
	BOOL Isbegin = FALSE;
	int pos = 0;
	int zzh = 0;
	for (;;)
	{
		switch(*tp)
		{
		case '/':			//
			if (Isbegin)
			{

				Isbegin = FALSE;
				memcpy(text+pos,befor,tp-befor);
				pos =pos + tp-befor;
				memcpy(text+pos,"\r\n",2);
				pos+=2;
			}

			if (memcmp(tp+1,"C=",2)==0)
			{
				memcpy(text+pos,"countryname=",12);
				pos+=12;
				Isbegin = TRUE;
				befor = tp+3;
			}
			else if (memcmp(tp+1,"CN=",3)==0)
			{
				memcpy(text+pos,"domain=",7);
				pos+=7;
				Isbegin = TRUE;
				befor = tp+4;
			}
			else if (memcmp(tp+1,"O=",2)==0)
			{
				memcpy(text+pos,"orgname=",8);
				pos+=8;
				Isbegin = TRUE;
				befor = tp+3;

			}
			else if (memcmp(tp+1,"emailAddress=",13)==0)
			{
				memcpy(text+pos,"email=",6);
				pos+=6;
				Isbegin = TRUE;
				befor = tp+14;

			}
			break;
		case 0:
			if (Isbegin)
			{

				Isbegin = FALSE;
				memcpy(text+pos,befor,tp-befor);
				pos =pos + tp-befor;
				memcpy(text+pos,"\r\n",2);
				pos+=2;
			}

			return;
		}
		tp++;
	}
	return;
}

void Parissuer(LPCTSTR strLine, char* text)
{
	char *tp = (char*)strLine;
	char *p = tp;
	char *befor;
	int c = 0;
	BOOL Isbegin = FALSE;
	int pos = 0;
	int zzh = 0;
	for (;;)
	{
		switch(*tp)
		{
		case '/':			//
			if (Isbegin)
			{

				Isbegin = FALSE;
				memcpy(text+pos,befor,tp-befor);
				pos =pos + tp-befor;
				memcpy(text+pos,"\r\n",2);
				pos+=2;
			}

			if (memcmp(tp+1,"O=",2)==0)
			{
				memcpy(text+pos,"OWNER=",6);
				pos+=6;
				Isbegin = TRUE;
				befor = tp+3;

			}
			break;
		case 0:
			if (Isbegin)
			{

				Isbegin = FALSE;
				memcpy(text+pos,befor,tp-befor);
				pos =pos + tp-befor;
				memcpy(text+pos,"\r\n",2);
				pos+=2;
			}

			return;
		}
		tp++;
	}
	return;
}