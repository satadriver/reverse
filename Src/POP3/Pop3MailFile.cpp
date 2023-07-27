//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技科技发展有限公司版权所有
//	文件名: 	Pop3MailFile
//	版本:		1.0
//	创建日期:	2007-7-28
//	作者:		荣强
//	主要功能:	
//	模块描述:
//	主要函数:	
//**************************************************************** 

#include "StdAfx.h"
#include <afxdisp.h>
#include "pop3.h"
#include "Pop3Session.h"
#include "ImapSession.h"

//*************************************************
//	函数名称:			Pop3Session::StrippingMail
//	描述:				拆解*.Eml文件 提取邮件属性,
//						正文和附件
//	输入参数:			
//	输出参数:			
//	返回值:				如果成功拆解返回0 失败返回负数
//	其它:				
//*************************************************
int Pop3Session::StrippingMail(CString& attfileslst)
{
	int re = -2;
	int error = 0;
//	CString attfileslst;
	CString strNewFileName;
	try
	{
error=2;
		// 解码邮件实体
		IMessagePtr iMsg(__uuidof(Message));
		_StreamPtr  pStm(__uuidof(Stream));	
		IBodyPartPtr iBp;	
		CString strTemp;

//		_bstr_t AttName;
		CString AttName;
		int j= -1;
		CString strFN;
		_bstr_t path;
		path = m_realfname;
		_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
error=3;
		try
		{
			pStm->raw_Open( varOptional, 
				adModeUnknown,   
				adOpenStreamUnspecified,
				NULL,
				NULL);
			pStm->LoadFromFile(path);
		}
		catch(_com_error e)
		{
			WriteLog(POP3PRO, "SaveAffixTo() comErr1!!");
			return -2;
		}
error=4;		
		IDataSourcePtr iDsrc;
		iMsg->AutoGenerateTextBody=TRUE;
		iDsrc = iMsg;

		try
		{
			iDsrc->OpenObject(pStm,_bstr_t("_Stream"));
		}
		catch(_com_error e)
		{
			WriteLog(POP3PRO, "SaveAffixTo() comErr2!!");
			return -2;
		}
error=5;

		// 提取邮件属性
		if (0 > ExtractMailHeader(iMsg))
		{
			return -3;
		}

//		attfileslst = m_emlfield;

		// 提取正文 并生成文本附加文件 
		if (0 > ExtractMailText(iMsg))
		{
			return -4;
		}
		m_from	=(char *)iMsg->From;
		m_to	=(char *)iMsg->To;

		// 保存附件
error=6;
		for(int i=1;i<iMsg->Attachments->Count+1;i++)
		{
error=61;
			iBp=iMsg->Attachments->Item[i];
			AttName=(char*)iBp->FileName;
			if (AttName.GetLength() <=0 )
			{
				continue;
			}

			// 转换url编码的文件名
			AttName = ConverHttp2Html(AttName);
			
			// 转换UTF-8编码的文件名
			ConvertUtf8toGbk(AttName, AttName.GetLength(), AttName);

			attfileslst += AttName + ",";
			strNewFileName.Format("%s@%s", m_realfname, AttName);
			strTemp=GetExtName(AttName);
			strTemp.TrimLeft();
			strTemp.TrimRight();

//strNewFileName=m_realfname;
			CString trueFname = strNewFileName;		// 实际的文件名
#ifdef		_DEBUG
			if (strNewFileName.GetLength() >= 50)
#else
			if (strNewFileName.GetLength() >= 200)
#endif
			{
				trueFname.Format("%s§%d.%s", m_realfname, m_attfiles.GetSize(), strTemp);
			}
error=7;
			BOOL suc = FALSE;
			try
			{
				suc = (iBp->SaveToFile(_bstr_t(trueFname)) == 0);
			}
			catch (...)
			{
				suc = FALSE;
			}
//			if(iBp->SaveToFile(_bstr_t(trueFname))==0)
			if(suc)
			{
				if (strTemp == "eml")
				{
					DealEmlFile(trueFname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst,g_Pop3FILEBUFFER);
				}
error=8;
				CString addfname;
				if (trueFname != strNewFileName)
				{
					addfname.Format("%s|%s", trueFname, GetShortName(strNewFileName));
				}
				else
				{
					addfname = strNewFileName;
				}
				char iret=checkfiletype(addfname,strTemp);
				int index = 0;
				if (iret=='5'||iret=='6'||iret=='7')
				{
					index =m_attfiles.Add(addfname+"*1");
					// 解压失败
					CString& tmpstr = m_attfiles.ElementAt(index);
					ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
					tmpstr.SetAt(tmpstr.GetLength()-1, iret);
					continue;
				}
				else
				{
					if(strTemp=="" || strTemp=="exe" || strTemp=="com" || strTemp=="pif" || strTemp =="scr"/*strTemp=="gif" || strTemp=="jpg"*/)
					{
						m_attfiles.Add(strNewFileName + "*2");
						DeleteFile(addfname);
						continue;
					}

				}
				

				// 如果是压缩文件(*.rar/*.zip)就要尝试解压
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// 尝试解压
					if (!DealRarFile(addfname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst))
					{
						// 解压失败
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// 解压成功
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '3');
					}
				}
				else
				{
					m_attfiles.Add(addfname + "*1");
				}

			}
			else
			{
				// 附件输出失败
				m_attfiles.Add(strNewFileName + "*2");
			}
		}

		// 匹配Mail的屏蔽信息
		m_rubbishflag = SearchBurrishByMail(m_emlfield + attfileslst);

	}
	catch(...)
	{
		WriteLog(POP3PRO, "Mail SaveAffixTo() Error=%d!!", error);
		return -3;
	}
	return 0;
}

//*************************************************
//	函数名称:			Pop3Session::ExtractMailHeader
//	描述:				提取邮件属性
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int Pop3Session::ExtractMailHeader(IMessagePtr& iMsg)
{
	DATE sendon=0;
	CString strfrom		=(char *)iMsg->From;
	m_from = strfrom;
	CString strto		=(char *)iMsg->To;
	m_to = strto;
	CString strcc		=(char *)iMsg->CC;
	CString strbcc		=(char *)iMsg->BCC;
	CString strreplyto	=(char *)iMsg->ReplyTo;
	CString strSubject	=(char *)iMsg->Subject;
	iMsg->get_SentOn(&sendon);
	
	COleDateTime sendtm(sendon);
	if((sendtm.GetStatus() == 0) && (sendon > 30000))
	{
		CString strsendtm = sendtm.Format("%Y-%m-%d %H:%M:%S");
		if(!strsendtm.IsEmpty())
		{
			m_emlfield.Format("sendtm= %s\r\n", strsendtm);	
		}
	}
	
	
	if (!strfrom.IsEmpty())
	{
		if (strfrom.GetLength()>800)
			m_emlfield.Format("%sfrom= %s\r\n", (CString)m_emlfield, strfrom.GetBuffer(800));
		else
			m_emlfield.Format("%sfrom= %s\r\n", (CString)m_emlfield, strfrom);
	}
	
	if (!strto.IsEmpty())
	{
		if (strto.GetLength()>800)
			m_emlfield.Format("%sto= %s\r\n", (CString)m_emlfield, strto.GetBuffer(800));
		else
			m_emlfield.Format("%sto= %s\r\n", (CString)m_emlfield, strto);
	}
	
	if(!strcc.IsEmpty())
	{
		if (strcc.GetLength()>800)
			m_emlfield.Format("%scc= %s\r\n", (CString)m_emlfield, strcc.GetBuffer(800));
		else
			m_emlfield.Format("%scc= %s\r\n", (CString)m_emlfield, strcc);
	}
	
	if(!strbcc.IsEmpty())
	{
		if (strcc.GetLength()>800)
			m_emlfield.Format("%sbcc= %s\r\n", (CString)m_emlfield, strbcc.GetBuffer(800));
		else
			m_emlfield.Format("%sbcc= %s\r\n", (CString)m_emlfield, strbcc);
	}
	
	if(!strreplyto.IsEmpty())
	{
		if (strreplyto.GetLength()>800)
			m_emlfield.Format("%sreplyto= %s\r\n", (CString)m_emlfield, strreplyto.GetBuffer(800));
		else
			m_emlfield.Format("%sreplyto= %s\r\n", (CString)m_emlfield, strreplyto);
	}
	
	if(!strSubject.IsEmpty())
	{
		if (strSubject.GetLength()>800)
			m_emlfield.Format("%ssubject= %s\r\n", (CString)m_emlfield, strSubject.GetBuffer(800));
		else
			m_emlfield.Format("%ssubject= %s\r\n", (CString)m_emlfield, strSubject);
	}
	return 0;
}

//*************************************************
//	函数名称:			Pop3Session::ExtractMailHeader
//	描述:				提取邮件文本并生成文本附加文件
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int Pop3Session::ExtractMailText(IMessagePtr& iMsg)
{
#define 	 SS _T("\r\n\r\n-------------------------------------------------\r\n\r\n")
	try
	{
		// 提取正文
		CString emlText = (char *)iMsg->TextBody;
		if (emlText.GetLength()>0)
		{
			m_Language = CheckSPL(emlText);	// 检查语种
		}
		else
		{
			m_Language = "";
		}

		// 写附加文件
		FILE * fs = 0;
		CString emltxtfile;
		emltxtfile = m_realfname + "@AdditionalTxtFile.txt";
		fs = fopen(emltxtfile, "wb");
		if(fs>0)
		{
			CString str;
			// 写邮件属性
			str = Getemlheader(m_realfname, g_Pop3FILEBUFFER, FILEBUFFERSIZE);
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写正文
			fwrite((LPCTSTR)emlText, 1, emlText.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写HTML格式的正文
			str = (char*)iMsg->HTMLBody;
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fclose(fs);
			m_attfiles.Add(emltxtfile + "*1");
		}
		else
		{
			WriteLog(POP3PRO, "打开文件<%s>失败!");
			return -4;
		}
	}
	catch (...)
	{
		WriteLog(POP3PRO, "ExtractMailText error");
		return -3;
	}
	return 0;	
}

void Pop3Session::DumpEmlFile()
{
	// 首先拆解邮件属性, 正文和附件并生成正文附加文件
	CString tempstr;
	int re = StrippingMail(tempstr);
	m_emlfilecount++;
	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// 需要匹配案件和预警的串
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);


	// 匹配案件和预警
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"pop3");

	tempstr.Empty();

	// 处理IDC数据
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1邮件服务\1%s\1POP3\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
		}
	}
	// 写活动历史

	HistoryRecord hisRec;
	CString from=SplitEmlName(m_to, m_userid, FALSE);//取出收件人，放入from中

	//匹配eml属性里边的匹配上user的to，是带着后缀的。
	SearchCaseEmailIm(this, from, from.GetLength(), d_state);

	//如果是有多个收件人，必须获得登录之后才能把to写上，否则不知道当前是那个人收信
	if (!from.IsEmpty() || !m_userid.IsEmpty())		//pop3无收件人and user为空不写表
	{
		if (m_userid.IsEmpty())
		{
			m_userid.Format("%s",from);
		}
		else if (from.IsEmpty())
		{
			from = m_userid;
		}
		char spytm[40];
		GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 40);
		HistoryRecord hisRec;
		hisRec.SetRecord(IDX_BEGTM,		spytm);
		hisRec.SetRecord(IDX_ENDTM,		spytm);
		hisRec.SetRecord(IDX_FORWTYPE,	"1");
		hisRec.SetRecord(IDX_ID,		from);
 		hisRec.SetRecord(IDX_USER,		from);

		from = SplitEmlName(m_from, "", TRUE);
		hisRec.SetRecord(IDX_CONTACT,	from);
		hisRec.SetRecord(IDX_PASS,		m_PASS);
		hisRec.SetRecord(IDX_STATE,		"收信");
		if (m_ProxyType == 1)
		{
			hisRec.SetRecord(IDX_IFAGENT,		"2");
		}
		WriteHisData("pop3", "pop3", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
	}

	// 写对照文件并输出
	text.Empty();
	if (!m_userid.IsEmpty())
	{
		text.Format("user= %s\r\n", m_userid);
	}
	if (!m_PASS.IsEmpty())
	{
		text.Format("%spass= %s\r\n", (CString)text, m_PASS);
	}
	if (!m_emlfield.IsEmpty())
	{
		// m_emlfield的结尾有"\r\n"
		text.Format("%s%s", (CString)text, m_emlfield);
	}

	// 输出文件
	m_indexfname = m_realfname + ".indexfile.txt";
	WriteIndexFile("pop3", "pop3", text, g_pop3MissObj);
	if (!m_from.IsEmpty() && !m_to.IsEmpty())
				{
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString strtm;
		strtm.Format("%04d%02d%02d%02d%02d%02d", now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond);
		
		CString strfs;
		strfs.Format("%s\t%s\t%s\t%s\r\n",m_from, m_to, "POP3", strtm);
		CString strfp;
		strfp.Format("v:\\netspy\\statdata\\emlfrdshp\\%s-%d.txt",strtm, rand());
		FILE *fp = fopen(strfp,"w+b");
		if (fp)
		{
			fwrite(strfs,1,strfs.GetLength(),fp);
			fclose(fp);
		}
		
		}
	Clear();
	return;
}

CString SplitEmlName(CString emlname,CString username,BOOL isfrom)
{
	int n;
	CString tempeml;
	tempeml=emlname;
	n=tempeml.Find("<");
	if (isfrom)
	{

		if (n>=0 && tempeml.GetLength()>n+1)
		{
			tempeml=tempeml.Mid(n+1);
			n=tempeml.Find(">");
			if (n>=0)
				tempeml=tempeml.Left(n);
		}
		return tempeml;
	}
	else
	{
		if (emlname.Find(",")>0)//多个收件人
		{
			n=emlname.Find(username);
			if (n>=0)
			{
				emlname=emlname.Mid(n);
				n=emlname.Find(">");
				emlname=emlname.Left(n);
				n=emlname.Find("<");
				if (n>=0)
				{
					emlname=emlname.Mid(n+1);
				}
				return emlname;
			}
			else
				return "";

		}
		else
		{
			if (n>=0 && tempeml.GetLength()>n+1)
			{
				tempeml=tempeml.Mid(n+1);
				n=tempeml.Find(">");
				if (n>=0)
					tempeml=tempeml.Left(n);
			}
			return tempeml;
		}
	}

}

int ImapSession::StrippingMail(CString& attfileslst)
{
	int re = -2;
	int error = 0;
//	CString attfileslst;
	CString strNewFileName;
	try
	{
error=2;
		// 解码邮件实体
		IMessagePtr iMsg(__uuidof(Message));
		_StreamPtr  pStm(__uuidof(Stream));	
		IBodyPartPtr iBp;	
		CString strTemp;

//		_bstr_t AttName;
		CString AttName;
		int j= -1;
		CString strFN;
		_bstr_t path;
		path = m_realfname;
		_variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);
error=3;
		try
		{
			pStm->raw_Open( varOptional, 
				adModeUnknown,   
				adOpenStreamUnspecified,
				NULL,
				NULL);
			pStm->LoadFromFile(path);
		}
		catch(_com_error e)
		{
			WriteLog(POP3PRO, "SaveAffixTo() comErr1!!");
			return -2;
		}
error=4;		
		IDataSourcePtr iDsrc;
		iMsg->AutoGenerateTextBody=TRUE;
		iDsrc = iMsg;

		try
		{
			iDsrc->OpenObject(pStm,_bstr_t("_Stream"));
		}
		catch(_com_error e)
		{
			WriteLog(POP3PRO, "SaveAffixTo() comErr2!!");
			return -2;
		}
error=5;

		// 提取邮件属性
		if (0 > ExtractMailHeader(iMsg))
		{
			return -3;
		}

//		attfileslst = m_emlfield;

		// 提取正文 并生成文本附加文件 
		if (0 > ExtractMailText(iMsg))
		{
			return -4;
		}
		m_from	=(char *)iMsg->From;
		m_to	=(char *)iMsg->To;

		// 保存附件
error=6;
		for(int i=1;i<iMsg->Attachments->Count+1;i++)
		{
error=61;
			iBp=iMsg->Attachments->Item[i];
			AttName=(char*)iBp->FileName;
			if (AttName.GetLength() <=0 )
			{
				continue;
			}

			// 转换url编码的文件名
			AttName = ConverHttp2Html(AttName);
			
			// 转换UTF-8编码的文件名
			ConvertUtf8toGbk(AttName, AttName.GetLength(), AttName);

			attfileslst += AttName + ",";
			strNewFileName.Format("%s@%s", m_realfname, AttName);
			strTemp=GetExtName(AttName);
			strTemp.TrimLeft();
			strTemp.TrimRight();

//strNewFileName=m_realfname;
			CString trueFname = strNewFileName;		// 实际的文件名
#ifdef		_DEBUG
			if (strNewFileName.GetLength() >= 50)
#else
			if (strNewFileName.GetLength() >= 200)
#endif
			{
				trueFname.Format("%s§%d.%s", m_realfname, m_attfiles.GetSize(), strTemp);
			}
error=7;
			BOOL suc = FALSE;
			try
			{
				suc = (iBp->SaveToFile(_bstr_t(trueFname)) == 0);
			}
			catch (...)
			{
				suc = FALSE;
			}
//			if(iBp->SaveToFile(_bstr_t(trueFname))==0)
			if(suc)
			{
				if (strTemp == "eml")
				{
					DealEmlFile(trueFname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst,g_Pop3FILEBUFFER);
				}
error=8;
				CString addfname;
				if (trueFname != strNewFileName)
				{
					addfname.Format("%s|%s", trueFname, GetShortName(strNewFileName));
				}
				else
				{
					addfname = strNewFileName;
				}
				char iret=checkfiletype(addfname,strTemp);
				int index = 0;
				if (iret=='5'||iret=='6'||iret=='7')
				{
					index =m_attfiles.Add(addfname+"*1");
					// 解压失败
					CString& tmpstr = m_attfiles.ElementAt(index);
					ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
					tmpstr.SetAt(tmpstr.GetLength()-1, iret);
					continue;
				}
				else
				{
					if(strTemp=="" || strTemp=="exe" || strTemp=="com" || strTemp=="pif" || strTemp =="scr"/*strTemp=="gif" || strTemp=="jpg"*/)
					{
						m_attfiles.Add(strNewFileName + "*2");
						DeleteFile(addfname);
						continue;
					}

				}
				

				// 如果是压缩文件(*.rar/*.zip)就要尝试解压
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// 尝试解压
					if (!DealRarFile(addfname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst))
					{
						// 解压失败
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// 解压成功
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '3');
					}
				}
			}
			else
			{
				// 附件输出失败
				m_attfiles.Add(strNewFileName + "*2");
			}
		}

		// 匹配Mail的屏蔽信息
		m_rubbishflag = SearchBurrishByMail(m_emlfield + attfileslst);

	}
	catch(...)
	{
		WriteLog(POP3PRO, "Mail SaveAffixTo() Error=%d!!", error);
		return -3;
	}
	return 0;
}

//*************************************************
//	函数名称:			ImapSession::ExtractMailHeader
//	描述:				提取邮件属性
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int ImapSession::ExtractMailHeader(IMessagePtr& iMsg)
{
	DATE sendon=0;
	CString strfrom		=(char *)iMsg->From;
	CString strto		=(char *)iMsg->To;
	CString strcc		=(char *)iMsg->CC;
	CString strbcc		=(char *)iMsg->BCC;
	CString strreplyto	=(char *)iMsg->ReplyTo;
	CString strSubject	=(char *)iMsg->Subject;
	iMsg->get_SentOn(&sendon);
	
	COleDateTime sendtm(sendon);
	if((sendtm.GetStatus() == 0) && (sendon > 30000))
	{
		CString strsendtm = sendtm.Format("%Y-%m-%d %H:%M:%S");
		if(!strsendtm.IsEmpty())
		{
			m_emlfield.Format("sendtm= %s\r\n", strsendtm);	
		}
	}
	
	
	if (!strfrom.IsEmpty())
	{
		if (strfrom.GetLength()>800)
			m_emlfield.Format("%sfrom= %s\r\n", (CString)m_emlfield, strfrom.GetBuffer(800));
		else
			m_emlfield.Format("%sfrom= %s\r\n", (CString)m_emlfield, strfrom);
	}
	
	if (!strto.IsEmpty())
	{
		if (strto.GetLength()>800)
			m_emlfield.Format("%sto= %s\r\n", (CString)m_emlfield, strto.GetBuffer(800));
		else
			m_emlfield.Format("%sto= %s\r\n", (CString)m_emlfield, strto);
	}
	
	if(!strcc.IsEmpty())
	{
		if (strcc.GetLength()>800)
			m_emlfield.Format("%scc= %s\r\n", (CString)m_emlfield, strcc.GetBuffer(800));
		else
			m_emlfield.Format("%scc= %s\r\n", (CString)m_emlfield, strcc);
	}
	
	if(!strbcc.IsEmpty())
	{
		if (strcc.GetLength()>800)
			m_emlfield.Format("%sbcc= %s\r\n", (CString)m_emlfield, strbcc.GetBuffer(800));
		else
			m_emlfield.Format("%sbcc= %s\r\n", (CString)m_emlfield, strbcc);
	}
	
	if(!strreplyto.IsEmpty())
	{
		if (strreplyto.GetLength()>800)
			m_emlfield.Format("%sreplyto= %s\r\n", (CString)m_emlfield, strreplyto.GetBuffer(800));
		else
			m_emlfield.Format("%sreplyto= %s\r\n", (CString)m_emlfield, strreplyto);
	}
	
	if(!strSubject.IsEmpty())
	{
		if (strSubject.GetLength()>800)
			m_emlfield.Format("%ssubject= %s\r\n", (CString)m_emlfield, strSubject.GetBuffer(800));
		else
			m_emlfield.Format("%ssubject= %s\r\n", (CString)m_emlfield, strSubject);
	}
	return 0;
}

//*************************************************
//	函数名称:			ImapSession::ExtractMailHeader
//	描述:				提取邮件文本并生成文本附加文件
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int ImapSession::ExtractMailText(IMessagePtr& iMsg)
{
#define 	 SS _T("\r\n\r\n-------------------------------------------------\r\n\r\n")
	try
	{
		// 提取正文
		CString emlText = (char *)iMsg->TextBody;
		if (emlText.GetLength()>0)
		{
			m_Language = CheckSPL(emlText);	// 检查语种
		}
		else
		{
			m_Language = "";
		}

		// 写附加文件
		FILE * fs = 0;
		CString emltxtfile;
		emltxtfile = m_realfname + "@AdditionalTxtFile.txt";
		fs = fopen(emltxtfile, "wb");
		if(fs>0)
		{
			CString str;
			// 写邮件属性
			str = Getemlheader(m_realfname, g_Pop3FILEBUFFER, FILEBUFFERSIZE);
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写正文
			fwrite((LPCTSTR)emlText, 1, emlText.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// 写HTML格式的正文
			str = (char*)iMsg->HTMLBody;
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fclose(fs);
			m_attfiles.Add(emltxtfile + "*1");
		}
		else
		{
			WriteLog(POP3PRO, "打开文件<%s>失败!");
			return -4;
		}
	}
	catch (...)
	{
		WriteLog(POP3PRO, "ExtractMailText error");
		return -3;
	}
	return 0;	
}

void ImapSession::DumpEmlFile()
{
	// 首先拆解邮件属性, 正文和附件并生成正文附加文件
	CString tempstr;
	int re = StrippingMail(tempstr);
	m_emlfilecount++;
	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// 需要匹配案件和预警的串
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);


	// 匹配案件和预警
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"pop3");

	tempstr.Empty();

	// 处理IDC数据
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1邮件服务\1%s\1POP3\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
		}
	}
	// 写活动历史

	HistoryRecord hisRec;
	CString from=SplitEmlName(m_to, m_userid, FALSE);//取出收件人，放入from中

	//匹配eml属性里边的匹配上user的to，是带着后缀的。
	SearchCaseEmailIm(this, from, from.GetLength(), d_state);

	//如果是有多个收件人，必须获得登录之后才能把to写上，否则不知道当前是那个人收信
	if (!from.IsEmpty() || !m_userid.IsEmpty())		//pop3无收件人and user为空不写表
	{
		if (m_userid.IsEmpty())
		{
			m_userid.Format("%s",from);
		}
		else if (from.IsEmpty())
		{
			from = m_userid;
		}
		char spytm[40];
		GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 40);
		HistoryRecord hisRec;
		hisRec.SetRecord(IDX_BEGTM,		spytm);
		hisRec.SetRecord(IDX_ENDTM,		spytm);
		hisRec.SetRecord(IDX_FORWTYPE,	"1");
		hisRec.SetRecord(IDX_ID,		from);
 		hisRec.SetRecord(IDX_USER,		from);

		from = SplitEmlName(m_from, "", TRUE);
		hisRec.SetRecord(IDX_CONTACT,	from);
		hisRec.SetRecord(IDX_PASS,		m_PASS);
		hisRec.SetRecord(IDX_STATE,		"收信");
		if (m_ProxyType == 1)
		{
			hisRec.SetRecord(IDX_IFAGENT,		"2");
		}
		WriteHisData("imap", "imap", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
	}

	// 写对照文件并输出
	text.Empty();
	if (!m_userid.IsEmpty())
	{
		text.Format("user= %s\r\n", m_userid);
	}
	if (!m_PASS.IsEmpty())
	{
		text.Format("%spass= %s\r\n", (CString)text, m_PASS);
	}
	if (!m_emlfield.IsEmpty())
	{
		// m_emlfield的结尾有"\r\n"
		text.Format("%s%s", (CString)text, m_emlfield);
	}

	// 输出文件
	m_indexfname = m_realfname + ".indexfile.txt";
	WriteIndexFile("imap", "imap", text, g_pop3MissObj);
	Clear();
	return;
}

