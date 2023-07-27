//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	SmtpMailFile
//	�汾:		1.0
//	��������:	2007-7-26
//	����:		��ǿ
//	��Ҫ����:	Smtp�ʼ���ԭ��Ĵ���(��⣬�ȶԺ����)
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#include "stdafx.h"
#include <afxdisp.h>
#include "smtp.h"
#include "SmtpSession.h"

//*************************************************
//	��������:			SmtpSession::StrippingMail
//	����:				���*.Eml�ļ� ��ȡ�ʼ�����,
//						���ĺ͸���
//	�������:			
//	�������:			
//	����ֵ:				����ɹ���ⷵ��0 ʧ�ܷ��ظ���
//	����:				
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
		// �����ʼ�ʵ��
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

		// ��ȡ�ʼ�����
		if (0 > ExtractMailHeader(iMsg))
		{
			return -3;
		}

//		attfileslst = m_emlfield;

		// ��ȡ���� �������ı������ļ� 
		if (0 > ExtractMailText(iMsg))
		{
			return -4;
		}
		m_from	=(char *)iMsg->From;
		m_to	=(char *)iMsg->To;

		// ���渽��
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

			// ת��url������ļ���
			AttName = ConverHttp2Html(AttName);
			
			// ת��UTF-8������ļ���
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
			CString trueFname = strNewFileName;		// ʵ�ʵ��ļ���
#ifdef		_DEBUG
			if (strNewFileName.GetLength() >= 50)
#else
			if (strNewFileName.GetLength() >= 200)
#endif
			{
				trueFname.Format("%s��%d.%s", m_realfname, m_attfiles.GetSize(), strTemp);
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
					// ��ѹʧ��
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
				// �����ѹ���ļ�(*.rar/*.zip)��Ҫ���Խ�ѹ
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// ���Խ�ѹ
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
				// �������ʧ��
				m_attfiles.Add(strNewFileName + "*2");
			}
		}
		
		// ƥ��Mail��������Ϣ(�ʼ�����)
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
//	��������:			SmtpSession::ExtractMailHeader
//	����:				��ȡ�ʼ�����
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
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
//	��������:			SmtpSession::ExtractMailHeader
//	����:				��ȡ�ʼ��ı��������ı������ļ�
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
//*************************************************
int SmtpSession::ExtractMailText(IMessagePtr& iMsg)
{
#define 	 SS _T("\r\n\r\n-------------------------------------------------\r\n\r\n")
	try
	{
		// ��ȡ����
		CString emlText = (char *)iMsg->TextBody;
		if (emlText.GetLength()>0)
		{
			m_Language = CheckSPL(emlText);	// �������
		}
		else
		{
			m_Language = "";
		}

		// д�����ļ�
		FILE * fs = 0;
		CString emltxtfile;
		emltxtfile = m_realfname + "@AdditionalTxtFile.txt";
		fs = fopen(emltxtfile, "wb");
		if(fs>0)
		{
			CString str;
			// д�ʼ�����
			str = Getemlheader(m_realfname, g_SmtpFileBuffer, FILEBUFFERSIZE);
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// д����
			fwrite((LPCTSTR)emlText, 1, emlText.GetLength(), fs);
			fwrite(SS, 1, sizeof(SS)-1, fs);
			// дHTML��ʽ������
			str = (char*)iMsg->HTMLBody;
			fwrite((LPCTSTR)str, 1, str.GetLength(), fs);
			fclose(fs);
			m_attfiles.Add(emltxtfile + "*1");
		}
		else
		{
			WriteLog(SMTPPRO, "���ļ�<%s>ʧ��!");
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
//	��������:			SmtpSession::DumpEmlFile
//	����:				����ʼ�
//	�������:			
//	�������:			
//	����ֵ:				
//	����:				
//*************************************************
void SmtpSession::DumpEmlFile()
{
	// ���ȴ����ʼ�������ƥ���ʼ�ʵ��
	CString tempstr;
	int re = StrippingMail(tempstr);

	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);

	// ƥ�䰸����Ԥ��
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);

	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"smtp");

	tempstr.Empty();
	// ����IDC����
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1�ʼ�����\1%s\1SMTP\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[SMTP_PROTOCOL].AddServer(str);
		}
	}

	// д���ʷ
	HistoryRecord hisRec;
	CStringArray arrto;
	SplitEmlName(m_to, &arrto, FALSE);//��ֳ������ռ���
	CString from=SplitEmlName(m_from, NULL, TRUE);//ȡ�������ˣ����m_user��û�з����ˣ�from����id��

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

			if (from.Find(m_userid)>=0)//��from��useridֵһ��ʱ����fromֵ���������@������
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
			hisRec.SetRecord(IDX_STATE,		"����");
			if (m_ProxyType == 1)
				hisRec.SetRecord(IDX_IFAGENT,		"2");
			WriteHisData("smtp", "smtp", &hisRec, g_smtpMissObj, SMTP_PROTOCOL);//, HISTORY_DATA);
		}
		
	}

	// д�����ļ������
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


//�����������ռ��ˣ�ȡ��<>�е�����
CString SplitEmlName(CString emlname, CStringArray* arrto, BOOL isfrom)
{
	int n,hh=0;
	CString tempeml;
	tempeml=emlname;
	n=tempeml.Find("<");
	if (isfrom)//��������
	{
		if (n>=0 && tempeml.GetLength()>n+1)
		{
			tempeml=tempeml.Mid(n+1);
			n=tempeml.Find(">");
			if (n>=0)
				tempeml=tempeml.Left(n);
		}
	}
	else//�ռ��ˣ��п����Ƕ��
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

