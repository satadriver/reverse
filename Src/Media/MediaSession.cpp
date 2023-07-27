//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技科技发展有限公司版权所有
//	文件名: 	MediaSession
//	版本:		1.0
//	创建日期:	2007-8-2
//	作者:		荣强
//	主要功能:	对Media类的会话类进行再一次的封装
//	模块描述:
//	主要函数:	
//**************************************************************** 
#include "stdafx.h"
#include "MediaSession.h"
#include "Media.h"
#include <sys/stat.h>

MediaSession::MediaSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;	// 1分钟调用一次CloseSession
	m_MediaPro = "";			// 协议类型(即对照文件中的协议字段值)
	m_AvType = 3;				// 实体文件类型(音频 / 视频 / 音视频)
	m_MediaDirection = "<<==>>";	// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = NULL;		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = NULL;		// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
	m_processtm = 0;

	// 匹配数据源
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
	m_MediaPro = "";			// 协议类型(即对照文件中的协议字段值)
	m_AvType = 3;				// 实体文件类型(音频 / 视频 / 音视频)
	m_MediaDirection = "<<==>>";	// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = NULL;		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = NULL;		// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer
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
		// 狗正常
		if ( (/*v_programtype==SYSTEM_SLEUTH && */abs(now - m_lastpacktm) >= 5)
			|| (abs(now - m_lastpacktm) >= 200)
			)
		{
			bDrop = TRUE;
			bOutFile = TRUE;
		}
		else
		{
			m_closetm += 60;		// 下一分钟继续调用CloseSession
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
// 		// 狗不正常
// 		bDrop = ((rand() & 7) != 0);
// 		bOutFile = ((rand() & 3) == 0);
// 	}
// VMPEND

	if (bOutFile)
	{
		// 如果需要输出文件
		m_processtm = now - 20;
		ListToFile();		// 处理链表

		//判断FLV是否拥有关键帧，如果没有关键帧，直接删除，只处理有关键帧的文件
		if (m_realfname.Find(".flv") != -1)
		{
			if (!CheckFlvMainFrame(m_realfname))
			{
		//		WriteLog(MEDIAPRO,"文件%s没有关键帧，删除!",m_realfname);
				if (_access(m_realfname,0) ==0)
				{
					remove(m_realfname);
				}
				m_realfname.Empty();
			}
		}

		if(!m_realfname.IsEmpty() && GetFileLength(m_realfname) > 5000)
		{
			//写对照文件
			DWORD d_state = 0;

			CString text;	// 需要匹配案件和预警的串
	 		if(!m_userid.IsEmpty())
	 		{
				// 匹配案件和预警
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	 		}
 			text.Format("__%s\r\n__%s\r\n", m_userid, m_LinkMan);

			// 匹配案件和预警
			SearchCaseOrWarn(this, text, text.GetLength(), d_state);

			if(strcmp(m_MediaPro,"media_QQ")==0)
			{
				if (m_CaseArr.GetSize() > 0)//案件全部处理
				{

				}
				else if (v_qqnum % v_mediasample == 0)//非案件符合抽样的处理
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
				if (m_CaseArr.GetSize() > 0)//案件全部处理
				{

				}
				else if (v_ucnum % v_mediasample == 0)//非案件符合抽样的处理
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
			
			// 处理IDC数据
			WriteIdcStat(m_IDCSvrType, m_IDCSvrSubType);

			// 写活动历史
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE, "2");
			WriteHisData("media", m_MediaPro, &hisRec, g_MediaMissObj,	MEDIA_PROTOCOL);//, HISTORY_DATA);

			// 写对照文件并输出
			text.Format("方向= %s\r\n", m_MediaDirection);
			if (m_AvType == 1)
			{
				text += "filetype= 视频\r\n";
			}
			else if (m_AvType == 2)
			{
				text += "filetype= 音频\r\n";
			}
			else if (m_AvType == 3)
			{
				text += "filetype= 音视频\r\n";
			}
				
	 		if(!m_userid.IsEmpty())
	 		{
				//应该写user，为适应查询的情况，user userid都写上
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
	//	printf("文件类型为:FLV\n");
		ptext += 3;

	//	printf("版本类型为:%d\n",*ptext);
		ptext++;

	//	printf("流信息为:%d\n",*ptext);
		ptext++;

		char tmp[20];
		char dst[20];

		memset(tmp,0,20);
		memset(dst,0,20);
		memcpy(tmp,ptext,4);
		unsigned int headlen = (u_char)ptext[0]*256*256*256 + (u_char)ptext[1]*256*256 +(u_char)ptext[2]*256+(u_char)ptext[3];
		printf("长度为:%d\n",headlen);
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
		 //	printf("音频\n");
			memcpy(datalen,ptext+1,3);


			unsigned int len = (u_char)datalen[0]*256*256 + (u_char)datalen[1]*256 +(u_char)datalen[2];

			//1 + 3 + 3 + 1 + 3
			ptext += 11;
			totallen += 11;

			if (len>0)
			{
				if (*ptext == 0x12)
				{
				//	printf("拥有音频关键帧\n");
					delete[] ptext;
					return TRUE;
				}
				else 
				{
				//	printf("没有有视频关键帧\n");
				}

			}
		
			//tag data
			ptext += len;
			totallen += len;
		}
		else if (*ptext == 0x09)
		{
		//	printf("视频\n");
			memcpy(datalen,ptext+1,3);

			unsigned int len = (u_char)datalen[0]*256*256 + (u_char)datalen[1]*256 +(u_char)datalen[2];

			//1 + 3 + 3 + 1 + 3
			ptext += 11;
			totallen += 11;

			if (len>0)
			{
				if (*ptext == 0x12)
				{
				//	printf("拥有视频关键帧!!!!!!!!!,file=%s\n",realname);
					delete[] ptext;
					return TRUE;
					
				}
				else 
				{
				//	printf("没有视频关键帧\n");
				}
			}
		
			//tag data
			ptext += len;
			totallen += len;

		}
		//我们程序内不会出现这种情况
		else if(*ptext == 0x12)
		{
			delete[] ptext;
			return FALSE;
		//	memcpy(datalen,ptext+1,3);
		//	printf("脚本\n");
		}

		if (totallen >= flen)
		{
		//	printf("分析完毕!\n");
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