//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	SmtpMailFile
//	版本:		1.0
//	创建日期:	2007-7-26
//	作者:		荣强
//	主要功能:	Smtp邮件还原后的处理(拆解，比对和输出)
//	模块描述:
//	主要函数:	
//**************************************************************** 

#include "stdafx.h"
#include <afxdisp.h>
#include "smtp.h"
#include "SmtpSession.h"

//*************************************************
//	函数名称:			SmtpSession::StrippingMail
//	描述:				拆解*.Eml文件 提取邮件属性,
//						正文和附件
//	输入参数:			
//	输出参数:			
//	返回值:				如果成功拆解返回0 失败返回负数
//	其它:				
//*************************************************
int SmtpSession::StrippingMail(CString& attfileslst)
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
			WriteLog(SMTPPRO, "SaveAffixTo() comErr1!!");
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
			WriteLog(SMTPPRO, "SaveAffixTo() comErr2!!");
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
// 			if(strTemp=="" || strTemp=="exe" || strTemp=="com" || strTemp=="pif" || strTemp =="scr"/*strTemp=="gif" || strTemp=="jpg"*/)
// 			{
// 				m_attfiles.Add(strNewFileName + "*2");
// 				continue;
// 			}
// 
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
			if(suc)
			{
				if (strTemp == "eml")
				{
					DealEmlFile(trueFname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst,g_SmtpFileBuffer);
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


			//	int index = m_attfiles.Add(addfname+"*1");
				// 如果是压缩文件(*.rar/*.zip)就要尝试解压
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// 尝试解压
					if (!DealRarFile(addfname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst))
					{
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
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
		
		// 匹配Mail的屏蔽信息(邮件属性)
		m_rubbishflag = SearchBurrishByMail(m_emlfield + attfileslst);
	}
	catch(...)
	{
		WriteLog(SMTPPRO, "Mail SaveAffixTo() Error=%d!!", error);
		return -3;
	}
	return 0;
}

//*************************************************
//	函数名称:			SmtpSession::ExtractMailHeader
//	描述:				提取邮件属性
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int SmtpSession::ExtractMailHeader(IMessagePtr& iMsg)
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
//	函数名称:			SmtpSession::ExtractMailHeader
//	描述:				提取邮件文本并生成文本附加文件
//	输入参数:			COM对象
//	输出参数:			
//	返回值:				成功返回0 否则返回负数
//	其它:				
//*************************************************
int SmtpSession::ExtractMailText(IMessagePtr& iMsg)
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
			str = Getemlheader(m_realfname, g_SmtpFileBuffer, FILEBUFFERSIZE);
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
			WriteLog(SMTPPRO, "打开文件<%s>失败!");
			return -4;
		}
	}
	catch (...)
	{
		WriteLog(SMTPPRO, "ExtractMailText error");
		return -3;
	}
	return 0;	
}

//*************************************************
//	函数名称:			SmtpSession::DumpEmlFile
//	描述:				输出邮件
//	输入参数:			
//	输出参数:			
//	返回值:				
//	其它:				
//*************************************************
void SmtpSession::DumpEmlFile()
{
	// 首先处理邮件附件并匹配邮件实体
	CString tempstr;
	int re = StrippingMail(tempstr);

	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// 需要匹配案件和预警的串
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);

	// 匹配案件和预警
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);

	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"smtp");

	tempstr.Empty();
	// 处理IDC数据
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1邮件服务\1%s\1SMTP\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[SMTP_PROTOCOL].AddServer(str);
		}
	}

	// 写活动历史
	HistoryRecord hisRec;
	CStringArray arrto;
	SplitEmlName(m_to, &arrto, FALSE);//拆分出所有收件人
	CString from=SplitEmlName(m_from, NULL, TRUE);//取出发件人，如果m_user中没有发件人，from放入id中

	SearchCaseEmailIm(this, from, from.GetLength(), 1);
	
	if (!m_userid.IsEmpty())
	{
		char spytm[40];
		GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 40);
		for(int tarr=0; tarr<arrto.GetSize() && tarr < 5; tarr++)
		{
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_BEGTM,		spytm);
			hisRec.SetRecord(IDX_ENDTM,		spytm);
			hisRec.SetRecord(IDX_FORWTYPE,	"1");

			if (from.Find(m_userid)>=0)//当from与userid值一致时，用from值，因其包含@后内容
			{
				hisRec.SetRecord(IDX_ID,		from);
				hisRec.SetRecord(IDX_USER,		from);
			}
			else if (!m_userid.IsEmpty())
			{
				hisRec.SetRecord(IDX_ID,		m_userid);
				hisRec.SetRecord(IDX_USER,		m_userid);
			}
			else
			{
				hisRec.SetRecord(IDX_ID,		from);
				hisRec.SetRecord(IDX_USER,		from);
			}
			hisRec.SetRecord(IDX_CONTACT,	arrto.GetAt(tarr));
			hisRec.SetRecord(IDX_PASS,		m_PASS);
			hisRec.SetRecord(IDX_STATE,		"发信");
			if (m_ProxyType == 1)
				hisRec.SetRecord(IDX_IFAGENT,		"2");
			WriteHisData("smtp", "smtp", &hisRec, g_smtpMissObj, SMTP_PROTOCOL);//, HISTORY_DATA);
		}
		
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
	if (m_emlfield)
	{
		text.Format("%s%s", (CString)text, m_emlfield);
	}

	m_indexfname = m_realfname + ".indexfile.txt";
	WriteIndexFile("smtp", "smtp", text, g_smtpMissObj);

	if (!m_from.IsEmpty() && !m_to.IsEmpty())
	{
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString strtm;
		strtm.Format("%04d%02d%02d%02d%02d%02d", now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond);

		CString strfs;
		strfs.Format("%s\t%s\t%s\t%s\r\n",m_from, m_to, "SMTP", strtm);
		CString strfp;
		strfp.Format("v:\\netspy\\statdata\\emlfrdshp\\%s-%d.txt",strtm, rand());
		FILE *fp = fopen(strfp,"w+b");
		if (fp)
		{
			fwrite(strfs,1,strfs.GetLength(),fp);
			fclose(fp);
		}

	}
	return;
}


//处理单个或多个收件人，取出<>中的内容
CString SplitEmlName(CString emlname, CStringArray* arrto, BOOL isfrom)
{
	int n,hh=0;
	CString tempeml;
	tempeml=emlname;
	n=tempeml.Find("<");
	if (isfrom)//处理发件人
	{
		if (n>=0 && tempeml.GetLength()>n+1)
		{
			tempeml=tempeml.Mid(n+1);
			n=tempeml.Find(">");
			if (n>=0)
				tempeml=tempeml.Left(n);
		}
	}
	else//收件人，有可能是多个
	{
		while (n>=0 && hh<500)
		{
			if (n>=0 && emlname.GetLength()>n+1)
			{
				emlname=emlname.Mid(n+1);
				n=emlname.Find(">");
				if (n>=0)
				{
					tempeml=emlname.Left(n);
					arrto->Add(tempeml);
				}
				else
				{
					return "";
				}
				emlname=emlname.Mid(n+1);
				n=emlname.Find("<");
			}
			hh++;
		}
	}
	return tempeml;
}

