//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ��Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	MediaSession
//	�汾:		1.0
//	��������:	2007-8-2
//	����:		��ǿ
//	��Ҫ����:	��Media��ĻỰ�������һ�εķ�װ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 
#include "stdafx.h"
#include "MediaSession.h"
#include "Media.h"
#include <sys/stat.h>

MediaSession::MediaSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;	// 1���ӵ���һ��CloseSession
	m_MediaPro = "";			// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_AvType = 3;				// ʵ���ļ�����(��Ƶ / ��Ƶ / ����Ƶ)
	m_MediaDirection = "<<==>>";	// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
	m_IDCSvrType = NULL;		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_IDCSvrSubType = NULL;		// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_processtm = 0;

	// ƥ������Դ
	if (m_dataSrc == DATASRC_OTHER)
	{
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
		{
			m_dataSrc = DATASRC_IDC;
		}
	}
}

MediaSession::MediaSession()
{
	m_MediaPro = "";			// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_AvType = 3;				// ʵ���ļ�����(��Ƶ / ��Ƶ / ����Ƶ)
	m_MediaDirection = "<<==>>";	// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
	m_IDCSvrType = NULL;		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_IDCSvrSubType = NULL;		// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_processtm = 0;
}

BOOL MediaSession::CloseSession(time_t now)
{
	BOOL bDrop = FALSE;
	BOOL bOutFile = FALSE;

// VMPBEGIN
// 	DWORD flg = v_flag;
// 	flg = Swap4Byte(flg ^ 0x5A5A5A5A);
// 	DWORD nowms = GetTickCount();
// 	if (abs(int(nowms - flg)) < 20*60*1000)
// 	{
		// ������
		if ( (/*v_programtype==SYSTEM_SLEUTH && */abs(now - m_lastpacktm) >= 5)
			|| (abs(now - m_lastpacktm) >= 200)
			)
		{
			bDrop = TRUE;
			bOutFile = TRUE;
		}
		else
		{
			m_closetm += 60;		// ��һ���Ӽ�������CloseSession
			if (abs(now - m_begfiletm) >= v_mediainterval)
			{
				
				bOutFile = TRUE;
			}
			else
			{
				return FALSE;
			}
		}
// 	}
// 	else
// 	{
// 		// ��������
// 		bDrop = ((rand() & 7) != 0);
// 		bOutFile = ((rand() & 3) == 0);
// 	}
// VMPEND

	if (bOutFile)
	{
		// �����Ҫ����ļ�
		m_processtm = now - 20;
		ListToFile();		// ��������

		//�ж�FLV�Ƿ�ӵ�йؼ�֡�����û�йؼ�֡��ֱ��ɾ����ֻ�����йؼ�֡���ļ�
		if (m_realfname.Find(".flv") != -1)
		{
			if (!CheckFlvMainFrame(m_realfname))
			{
		//		WriteLog(MEDIAPRO,"�ļ�%sû�йؼ�֡��ɾ��!",m_realfname);
				if (_access(m_realfname,0) ==0)
				{
					remove(m_realfname);
				}
				m_realfname.Empty();
			}
		}

		if(!m_realfname.IsEmpty() && GetFileLength(m_realfname) > 5000)
		{
			//д�����ļ�
			DWORD d_state = 0;

			CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
	 		if(!m_userid.IsEmpty())
	 		{
				// ƥ�䰸����Ԥ��
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	 		}
 			text.Format("__%s\r\n__%s\r\n", m_userid, m_LinkMan);

			// ƥ�䰸����Ԥ��
			SearchCaseOrWarn(this, text, text.GetLength(), d_state);

			if(strcmp(m_MediaPro,"media_QQ")==0)
			{
				if (m_CaseArr.GetSize() > 0)//����ȫ������
				{

				}
				else if (v_qqnum % v_mediasample == 0)//�ǰ������ϳ����Ĵ���
				{
					v_qqnum = 1;
				}
				else
				{
					v_qqnum++;
					if (_access(m_realfname, 0) == 0)
					{
						DeleteFile(m_realfname);
					}
					m_realfname.Empty();
					return bDrop;

				}
			}
			else if(strcmp(m_MediaPro,"media_UC")==0)
			{
				if (m_CaseArr.GetSize() > 0)//����ȫ������
				{

				}
				else if (v_ucnum % v_mediasample == 0)//�ǰ������ϳ����Ĵ���
				{
					v_ucnum = 1;
				}
				else
				{
					v_ucnum++;
					if (_access(m_realfname, 0) == 0)
					{
						DeleteFile(m_realfname);
					}
					m_realfname.Empty();
					return bDrop;
				}
			}
			
			// ����IDC����
			WriteIdcStat(m_IDCSvrType, m_IDCSvrSubType);

			// д���ʷ
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE, "2");
			WriteHisData("media", m_MediaPro, &hisRec, g_MediaMissObj,	MEDIA_PROTOCOL);//, HISTORY_DATA);

			// д�����ļ������
			text.Format("����= %s\r\n", m_MediaDirection);
			if (m_AvType == 1)
			{
				text += "filetype= ��Ƶ\r\n";
			}
			else if (m_AvType == 2)
			{
				text += "filetype= ��Ƶ\r\n";
			}
			else if (m_AvType == 3)
			{
				text += "filetype= ����Ƶ\r\n";
			}
				
	 		if(!m_userid.IsEmpty())
	 		{
				//Ӧ��дuser��Ϊ��Ӧ��ѯ�������user userid��д��
	 			text.Format("%suserid= %s\r\nuser= %s\r\ncontact=%s\r\ntpye=%s\r\n",
					(CString)text, m_userid, m_userid,m_LinkMan,m_type);
	 		}
			/*
			else
			{
				text = text + "type=" + m_type + "\r\n";
			}*/

			m_indexfname.Format("%s.indexfile.txt", m_realfname);
			if (m_type == "voip_media")
			{
				WriteIndexFile("media", m_MediaPro, m_strText, g_MediaMissObj);
			}
			else
			{
				WriteIndexFile("media", m_MediaPro, text, g_MediaMissObj);
			}
			
			
		}
		if (m_realfname.IsEmpty())
		{
			if (_access(m_realfname, 0) == 0)
			{
				DeleteFile(m_realfname);
			}
			m_realfname.Empty();
		}
		if (m_indexfname.IsEmpty())
		{
			if (_access(m_indexfname, 0) == 0)
			{
				DeleteFile(m_indexfname);
			}
			m_indexfname.Empty();
		}
		m_begfiletm = m_endfiletm;
		m_endfiletm = 0;
	}

	return bDrop;
}

BOOL CheckFlvMainFrame(CString realname)
{
	FILE * fs=0;
	int   flen=0;
	char	*ptext=0;
	int     totallen=0;

	struct stat fst;
	if (stat(realname, &fst) == 0)
		flen=fst.st_size;

	ptext = new char[flen+2];
	memset(ptext,0,flen+2);

	fs=fopen(realname, "rb");
	if(fs > 0)
	{
		UINT readsize=0;
		readsize = fread(ptext, 1, flen, fs);
		fclose(fs);
	}

	//FLV HEADER
	if(memcmp(ptext,"FLV",3) ==0)
	{
	//	printf("�ļ�����Ϊ:FLV\n");
		ptext += 3;

	//	printf("�汾����Ϊ:%d\n",*ptext);
		ptext++;

	//	printf("����ϢΪ:%d\n",*ptext);
		ptext++;

		char tmp[20];
		char dst[20];

		memset(tmp,0,20);
		memset(dst,0,20);
		memcpy(tmp,ptext,4);
		unsigned int headlen = (u_char)ptext[0]*256*256*256 + (u_char)ptext[1]*256*256 +(u_char)ptext[2]*256+(u_char)ptext[3];
		printf("����Ϊ:%d\n",headlen);
		ptext+=4;

		totallen+=9;
	}
	else
	{
		delete[] ptext;
		return FALSE;
	}

	//FLV BODY
	while(ptext)
	{
// 		char *datalen=0;
// 
// 		datalen = new char[10];
		char datalen[10];
		memset(datalen,0,10);
		//pre tag length
		ptext +=4;
		totallen+=4;
		
		if (*ptext == 0x08)
		{
		 //	printf("��Ƶ\n");
			memcpy(datalen,ptext+1,3);


			unsigned int len = (u_char)datalen[0]*256*256 + (u_char)datalen[1]*256 +(u_char)datalen[2];

			//1 + 3 + 3 + 1 + 3
			ptext += 11;
			totallen += 11;

			if (len>0)
			{
				if (*ptext == 0x12)
				{
				//	printf("ӵ����Ƶ�ؼ�֡\n");
					delete[] ptext;
					return TRUE;
				}
				else 
				{
				//	printf("û������Ƶ�ؼ�֡\n");
				}

			}
		
			//tag data
			ptext += len;
			totallen += len;
		}
		else if (*ptext == 0x09)
		{
		//	printf("��Ƶ\n");
			memcpy(datalen,ptext+1,3);

			unsigned int len = (u_char)datalen[0]*256*256 + (u_char)datalen[1]*256 +(u_char)datalen[2];

			//1 + 3 + 3 + 1 + 3
			ptext += 11;
			totallen += 11;

			if (len>0)
			{
				if (*ptext == 0x12)
				{
				//	printf("ӵ����Ƶ�ؼ�֡!!!!!!!!!,file=%s\n",realname);
					delete[] ptext;
					return TRUE;
					
				}
				else 
				{
				//	printf("û����Ƶ�ؼ�֡\n");
				}
			}
		
			//tag data
			ptext += len;
			totallen += len;

		}
		//���ǳ����ڲ�������������
		else if(*ptext == 0x12)
		{
			delete[] ptext;
			return FALSE;
		//	memcpy(datalen,ptext+1,3);
		//	printf("�ű�\n");
		}

		if (totallen >= flen)
		{
		//	printf("�������!\n");
			delete[] ptext;
			break;
		}
	}

	fclose(fs);
	if (ptext != NULL)
	{
		delete[] ptext;
		ptext = NULL;
	}
	return FALSE;
}