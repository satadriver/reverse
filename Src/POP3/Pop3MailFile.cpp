//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ��Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Pop3MailFile
//	�汾:		1.0
//	��������:	2007-7-28
//	����:		��ǿ
//	��Ҫ����:	
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#include "StdAfx.h"
#include <afxdisp.h>
#include "pop3.h"
#include "Pop3Session.h"
#include "ImapSession.h"

//*************************************************
//	��������:			Pop3Session::StrippingMail
//	����:				���*.Eml�ļ� ��ȡ�ʼ�����,
//						���ĺ͸���
//	�������:			
//	�������:			
//	����ֵ:				����ɹ���ⷵ��0 ʧ�ܷ��ظ���
//	����:				
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
		// �����ʼ�ʵ��
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

//strNewFileName=m_realfname;
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
				

				// �����ѹ���ļ�(*.rar/*.zip)��Ҫ���Խ�ѹ
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// ���Խ�ѹ
					if (!DealRarFile(addfname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst))
					{
						// ��ѹʧ��
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// ��ѹ�ɹ�
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

		// ƥ��Mail��������Ϣ
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
//	��������:			Pop3Session::ExtractMailHeader
//	����:				��ȡ�ʼ�����
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
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
//	��������:			Pop3Session::ExtractMailHeader
//	����:				��ȡ�ʼ��ı��������ı������ļ�
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
//*************************************************
int Pop3Session::ExtractMailText(IMessagePtr& iMsg)
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
			str = Getemlheader(m_realfname, g_Pop3FILEBUFFER, FILEBUFFERSIZE);
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
			WriteLog(POP3PRO, "���ļ�<%s>ʧ��!");
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
	// ���Ȳ���ʼ�����, ���ĺ͸������������ĸ����ļ�
	CString tempstr;
	int re = StrippingMail(tempstr);
	m_emlfilecount++;
	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);


	// ƥ�䰸����Ԥ��
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"pop3");

	tempstr.Empty();

	// ����IDC����
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1�ʼ�����\1%s\1POP3\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
		}
	}
	// д���ʷ

	HistoryRecord hisRec;
	CString from=SplitEmlName(m_to, m_userid, FALSE);//ȡ���ռ��ˣ�����from��

	//ƥ��eml������ߵ�ƥ����user��to���Ǵ��ź�׺�ġ�
	SearchCaseEmailIm(this, from, from.GetLength(), d_state);

	//������ж���ռ��ˣ������õ�¼֮����ܰ�toд�ϣ�����֪����ǰ���Ǹ�������
	if (!from.IsEmpty() || !m_userid.IsEmpty())		//pop3���ռ���and userΪ�ղ�д��
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
		hisRec.SetRecord(IDX_STATE,		"����");
		if (m_ProxyType == 1)
		{
			hisRec.SetRecord(IDX_IFAGENT,		"2");
		}
		WriteHisData("pop3", "pop3", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
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
	if (!m_emlfield.IsEmpty())
	{
		// m_emlfield�Ľ�β��"\r\n"
		text.Format("%s%s", (CString)text, m_emlfield);
	}

	// ����ļ�
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
		if (emlname.Find(",")>0)//����ռ���
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
		// �����ʼ�ʵ��
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

//strNewFileName=m_realfname;
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
				

				// �����ѹ���ļ�(*.rar/*.zip)��Ҫ���Խ�ѹ
				if ((strTemp == "rar") || (strTemp == "zip"))
				{
					// ���Խ�ѹ
					if (!DealRarFile(addfname, GetShortName(m_realfname), m_attfiles, "dce7", attfileslst))
					{
						// ��ѹʧ��
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '4');
					}
					else
					{
						// ��ѹ�ɹ�
						CString& tmpstr = m_attfiles.ElementAt(index);
						ASSERT(tmpstr.GetAt(tmpstr.GetLength()-2)=='*');
						tmpstr.SetAt(tmpstr.GetLength()-1, '3');
					}
				}
			}
			else
			{
				// �������ʧ��
				m_attfiles.Add(strNewFileName + "*2");
			}
		}

		// ƥ��Mail��������Ϣ
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
//	��������:			ImapSession::ExtractMailHeader
//	����:				��ȡ�ʼ�����
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
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
//	��������:			ImapSession::ExtractMailHeader
//	����:				��ȡ�ʼ��ı��������ı������ļ�
//	�������:			COM����
//	�������:			
//	����ֵ:				�ɹ�����0 ���򷵻ظ���
//	����:				
//*************************************************
int ImapSession::ExtractMailText(IMessagePtr& iMsg)
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
			str = Getemlheader(m_realfname, g_Pop3FILEBUFFER, FILEBUFFERSIZE);
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
			WriteLog(POP3PRO, "���ļ�<%s>ʧ��!");
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
	// ���Ȳ���ʼ�����, ���ĺ͸������������ĸ����ļ�
	CString tempstr;
	int re = StrippingMail(tempstr);
	m_emlfilecount++;
	m_OpenIndxFlg = v_openflagpop3;
	m_OpenCaseFlg = v_opencasepop3;
	m_OpenWarnFlg = v_openwarnpop3;

	CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
	text.Format("%s\r\n____%s\r\n____%s\r\n___%s", 
		m_userid, m_PASS, m_emlfield, tempstr);


	// ƥ�䰸����Ԥ��
	DWORD d_state = 1;
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 1);
	SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state,"pop3");

	tempstr.Empty();

	// ����IDC����
	if (v_programtype!=SYSTEM_SLEUTH && v_stat)
	{
		CString strISP;
		if (v_statIdc && v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
		{
			CString str;
			str.Format("%s\1%s\1pop3\1�ʼ�����\1%s\1POP3\1%s\1\1",
				m_dip.Left(15), strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[POP3_PROTOCOL].AddServer(str);
		}
	}
	// д���ʷ

	HistoryRecord hisRec;
	CString from=SplitEmlName(m_to, m_userid, FALSE);//ȡ���ռ��ˣ�����from��

	//ƥ��eml������ߵ�ƥ����user��to���Ǵ��ź�׺�ġ�
	SearchCaseEmailIm(this, from, from.GetLength(), d_state);

	//������ж���ռ��ˣ������õ�¼֮����ܰ�toд�ϣ�����֪����ǰ���Ǹ�������
	if (!from.IsEmpty() || !m_userid.IsEmpty())		//pop3���ռ���and userΪ�ղ�д��
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
		hisRec.SetRecord(IDX_STATE,		"����");
		if (m_ProxyType == 1)
		{
			hisRec.SetRecord(IDX_IFAGENT,		"2");
		}
		WriteHisData("imap", "imap", &hisRec, g_pop3MissObj, POP3_PROTOCOL);//, HISTORY_DATA);
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
	if (!m_emlfield.IsEmpty())
	{
		// m_emlfield�Ľ�β��"\r\n"
		text.Format("%s%s", (CString)text, m_emlfield);
	}

	// ����ļ�
	m_indexfname = m_realfname + ".indexfile.txt";
	WriteIndexFile("imap", "imap", text, g_pop3MissObj);
	Clear();
	return;
}

