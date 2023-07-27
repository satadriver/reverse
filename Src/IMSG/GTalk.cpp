// GTalk.cpp: implementation of the GTalk class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GTalk.h"
#include "imsg.h"
#include "Markup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GTalkSession::GTalkSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_processtm = packh.m_tm;
}

GTalkSession::~GTalkSession()
{



}


int GTalkSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	 
	//request
	if(memcmp(packh.m_sip, m_csip, 4) == 0)
	{
		TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
		m_ReqPacketlst.push_back(Tcpp);
		Tcpp.buf = NULL;
	 

	}
	//response
	else 
	{
		TcpPacket	Tcpp(packh, pPacket, 0, 0) ;
		m_ResponsePacketlst.push_back(Tcpp);
		Tcpp.buf = NULL;
	}


	if (abs(packh.m_tm - m_processtm) > 30)
	{
		// 保留20s 数据
		m_processtm = packh.m_tm-20;

		m_ReqPacketlst.sort();
		m_ResponsePacketlst.sort();
		
		ListToFile(&m_ResponsePacketlst,FALSE);
		ListToFile(&m_ReqPacketlst,  TRUE);
	}
		 

	return 0;
}


BOOL GTalkSession::CloseSession(time_t nowtm)
{
	m_processtm=nowtm;
	m_ReqPacketlst.sort();
	m_ResponsePacketlst.sort();
	
	ListToFile(&m_ResponsePacketlst,FALSE);
	ListToFile(&m_ReqPacketlst,TRUE);
	return FALSE;	
}


BOOL GetNodeName( unsigned char *buf, int lenbuf,CString& nodename)
{
	
	UCHAR * p=buf;
	UCHAR * pStartLine = buf;
	UCHAR * pEnd = p + lenbuf;
	INT LineLen = 0;
	while (p < pEnd)
	{
		if(p[0] == ' ' || p[0]=='>' )  
		{
			if(LineLen > 0)
			{
				nodename=CString((char*)pStartLine,LineLen);
				return TRUE;
			}
			
			pStartLine = (p+1) ;
			LineLen = 0;
		}
		else
		{
			LineLen++;
		}
		p++;
	}
	return FALSE;
}


BOOL GTalkSession::ListToFile(TcpPacketList * pTcpList,BOOL b_request)
{
	TcpPacketListIt tmpitem, item, end;
	item = pTcpList->begin();
	end = pTcpList->end();	
	BOOL b_havehead=FALSE;
	TcpPacketListIt pfirst,plast;
	CString nodename;
	while(item != end)
	{
		TcpPacket* pPacket = &(*item);
		if (pPacket->tm > m_processtm)
		{
			break;
		}
		tmpitem=item;
		item++;
		if(memcmp(pPacket->buf,"<iq to=",7)==0 )
		{
			nodename="iq";
		}
		else if (memcmp(pPacket->buf,"<message",8)==0)
		{
			nodename="message";
		}
		if(memcmp(pPacket->buf,"<iq to=",7)==0 || memcmp(pPacket->buf,"<message",8)==0)
		{
			if(b_havehead)
			{
				pTcpList->erase(pfirst,plast);
				b_havehead=FALSE;
			}
				b_havehead=FALSE;
			int offset=pPacket->Slen-1;
			if(memcmp(pPacket->buf+pPacket->Slen-2,"\r\n",2)==0)
			{
				offset=	pPacket->Slen-3;
			}
			if(memcmp(pPacket->buf+offset-1 ,"/>",2)==0)
			{
				nodename.Empty();
				pTcpList->erase(tmpitem);
			}
			else if(pPacket->buf[offset]=='>')
			{			
				CString endnodename;
				endnodename.Format("</%s>",nodename);
				//单包
				if(memcmp(pPacket->buf+pPacket->Slen-endnodename.GetLength(),(char*)(LPCTSTR)endnodename,endnodename.GetLength())==0)
				{
					OutXmlMessage(pPacket->buf,pPacket->Slen,b_request);
					nodename.Empty();
					pTcpList->erase(tmpitem);
				}
				else  //多包
				{
					plast=pfirst=tmpitem;
					b_havehead=TRUE;
					memcpy(g_pMsgBuffer,pPacket->buf,pPacket->Slen);
					g_Msglen=pPacket->Slen;
				}
			}
			else 
			{
				plast=pfirst=tmpitem;
				b_havehead=TRUE;
				memcpy(g_pMsgBuffer,pPacket->buf,pPacket->Slen);
				g_Msglen=pPacket->Slen;
			}
		}
		else if(b_havehead)//中间包
		{
			if (pPacket->Nextsequence <= plast->Nextsequence)
			{
				plast=tmpitem;
				// 重复的包
				continue;
			}
			if (pPacket->Ssequence > plast->Nextsequence)
			{
				plast=tmpitem;
				b_havehead=FALSE;
				pTcpList->erase(pfirst,plast);
				// 包不连续
				continue;
			}
			plast=tmpitem;
			memcpy(g_pMsgBuffer+g_Msglen,pPacket->buf,pPacket->Slen);
			g_Msglen+=pPacket->Slen;		
			if(pPacket->buf[pPacket->Slen-1]=='>')
			{
				CString endnodename;
				endnodename.Format("</%s>",nodename);
				if(memcmp(pPacket->buf+pPacket->Slen-endnodename.GetLength(),(char*)(LPCTSTR)endnodename,endnodename.GetLength())==0)
				{
					OutXmlMessage(g_pMsgBuffer,g_Msglen,b_request);
					nodename.Empty();

					b_havehead=FALSE;
					pTcpList->erase(pfirst,plast);
				}
			}
		
		}
		else //不处理的包
		{
			pTcpList->erase(tmpitem);	
			continue;
		}
	}
	return TRUE;
}

BOOL  GTalkSession::OutXmlMessage(CHAR* buf ,int buflen,BOOL b_request)
{
	buf[buflen]='\0';

	CMarkup xml;
	if(memcmp(buf,"<iq ",4)==0)
	{
		if(b_request)//不处理请求的
		{
			return FALSE;
		}
		char* pdest=strstr(buf,"<nos:query");
		if(!pdest)
			return FALSE;	 
		//处理好友列表
		xml.SetDoc(CString(buf,buflen));
		if(!xml.FindElem("iq"))	// iq
			return FALSE;	
		CString to=xml.GetAttrib("to");
		CString from=xml.GetAttrib("from");
		if(from.IsEmpty())
			return FALSE;
		CString iqtype=xml.GetAttrib("type");
		if(iqtype=="result")
		{
			if(m_userid.IsEmpty())
			{
				m_userid=from; //用户名
			}
		}
		else 
			return FALSE;	 
		if(xml.IntoElem())
		{
			if(xml.FindElem("nos:query"))	// nos:query
			{
				if(xml.IntoElem())
				{
					CString result;
					int num=0;
					result.Format("gtalklist\t%u\t",m_userid);
					while(xml.FindElem("nos:item")) // 
					{
						CString jid=xml.GetAttrib("jid");
						CString tmpstr;
						tmpstr.Format("%s\t",jid);
						result+=tmpstr;
						num++;
					}		
				//	if(num>0)
				//		WriteArrToData(IMSG_PROTOCOL , WEBID_DATA,result);
				}
			}
		}
	}
	else if(memcmp(buf,"<message ",9)==0)
	{
		//处理消息
		//处理好友列表
		xml.SetDoc(CString(buf,buflen));
		if(!xml.FindElem("message"))	// message
			return FALSE;
		
		CString to=xml.GetAttrib("to");
		CString from=xml.GetAttrib("from");
		if(to.IsEmpty())
			return FALSE;
		if(!b_request && m_userid.IsEmpty())
		{
			m_userid=to;
		}

		if(from.IsEmpty() && b_request)
		{
			from=m_userid;
		}

		CString iqtype=xml.GetAttrib("type");
		if(iqtype=="chat")
		{
			 m_from=from;
			 m_to=to;
		}
		else 
			return FALSE;
		if(xml.IntoElem())
		{
			if(xml.FindElem("body"))	// body
			{
				CString body=xml.GetData();
			 
				CString outstr;
				if(!body.IsEmpty())
				{
					ConvertUtf8toGbk(body, body.GetLength(), outstr);

				 
					if(m_from.Find("@appspot.com")>0 || m_to.Find("@appspot.com")>0)
					{
						m_weiboname="推特微博";
					}

					//printf("%s->%s : %s\r\n",m_from,m_to,body);
					OutData(outstr,outstr.GetLength());
				}			
			}
		}					
	}	
	return TRUE;
}

void GTalkSession::OutData(const char* pData, int datalen)
{
	if(datalen<=0)
		return; 
	WriteRealFile(pData, datalen);
	// 输出txt文件
 
	m_OpenIndxFlg = v_openflagoicq;
	m_OpenCaseFlg = v_opencaseoicq;
	m_OpenWarnFlg = v_openwarnoicq;
	
	// 写对照文件
	m_realfname = m_filename;
	m_filename.Empty();
	m_indexfname = m_realfname + ".indexfile.txt";
	
	// 匹配案件预警和屏蔽信息
	DWORD dwState = 0;
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwState);
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
	CString stetxt;
	CString Pro="imsg_gtalk";
	if(m_from.IsEmpty())
	{
		if(!m_weiboname.IsEmpty())
		{
			stetxt.Format("user= %s\r\nweiboname=%s\r\n", m_userid,m_weiboname);
			Pro="wb_twitter";
		}
		else 
			stetxt.Format("user= %s\r\n", m_userid);
	}
	else 
	{
			if(!m_weiboname.IsEmpty())
			{
				stetxt.Format("user= %s\r\nsender= %s\r\ncontentid=%s\r\nweiboname=%s\r\n", 
				m_userid,m_from,m_to,m_weiboname);	
				Pro="wb_twitter";
			}
			else 
				stetxt.Format("user= %s\r\nsender= %s\r\ncontentid=%s\r\n", 
			m_userid,m_from,m_to);
	}

	WriteIndexFile(Pro, Pro, stetxt, g_pImsgMissObj);
}

void GTalkSession::WriteRealFile(const char* pData, int datalen)
{
	if (datalen>0)
	{
		if (m_filename.IsEmpty())
		{
			m_filename = CreateFilename(m_sip, m_dip, "imsg_gtalk.txt");
			ASSERT(!m_filename.IsEmpty());
		}
		FILE* fp = fopen(m_filename, "ab");
		if (fp == NULL)
		{
			return;
		}
		fwrite(pData, datalen, 1, fp);
		fclose(fp);
	}
}
