//***************************************************************
//	Copyright (C) 2008 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	taobao
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		lp
//	��Ҫ����:	taobao����Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 
#include "stdafx.h"
#include "imsg.h"
#include "TaoBao1.h"
#include "TaoBaoDecode.h"
int g_error = 0;
CMapStringToString g_righttaobaomap(8192);
CMapStringToString g_wrongtaobaomap(8192);

TaoBaoSession::TaoBaoSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_direction = 0;
	m_closetm = packh.m_tm + 180;
	m_bIdc = FALSE;
	m_MsgBuflen = 0;
	m_pMsgBuf = NULL;
	m_begfiletm = 0;
	
	g_ImsgSion++;
}

TaoBaoSession::~TaoBaoSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL TaoBaoSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0)
		{
			// �Դ��ϴδ��ļ�����һֱû�а���������ٸûỰ
			return TRUE;
		}
		
		// ����ļ�
		if (m_MsgBuflen > 0)
		{
			// дʵ���ļ�
			WriteRealFile(g_pMsgBuffer, 0);
		}
		if (_access(m_realfname,0) != 0)
		{
			return TRUE;
		}
		// ���ﲻ����IDCServer �� ���ʷ
		// ��������Դ
		if(m_dataSrc == DATASRC_OTHER)
		{
			if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
				|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
			{
				m_dataSrc = DATASRC_IDC;
			}
		}

		// ƥ�䰸����Ԥ��
		m_OpenCaseFlg = v_opencasetaobao;
		m_OpenWarnFlg = v_openwarntaobao;
		m_OpenIndxFlg = v_openflagtaobao;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// ƥ������
		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// д�����ļ�
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_taobao.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}
		CString str;
		str.Format("user= %s\r\nstat= ͨѶ\r\n", m_userid);
		WriteIndexFile("imsg_taobao", "imsg_taobao", str, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
		m_closetm += 180;
		return FALSE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "taobaoSession::CloseSession() Error!!");
		return TRUE;
	}
}

int TaoBaoSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
		if ((memcmp(pPacket,"\x8f\x01\x01\x00",4) == 0 ||
			memcmp(pPacket,"\x88\x06\x00\x00\x00\x00\x00",7) == 0 )&&
			packh.m_datalen > 0x50 &&
			(memcmp(pPacket+24,"\x03\x06\x00\x01",4) == 0||
			memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||	//����
			memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||	//����
			memcmp(pPacket+0x48,"\x01\x2b",2) == 0 ||	//������������İ������ 01 2b
			memcmp(pPacket+0x48,"\x01\x30",2) == 0))	//������������ͻ��˵İ������ 01 30 
		{
			;
		}
		else
		{
			return FALSE;
		}

		if (m_begfiletm == 0)
		{
			m_begfiletm = packh.m_tm;
		}

		if (memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||
			memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||
			(memcmp(pPacket,"\x88\x06\x00\x00\x00\x00\x00",7) == 0 &&
			memcmp(pPacket+24,"\x03\x06\x00\x01",4) == 0 ))
		{
			FormatLoginOrOut(packh,pPacket);
		}
		else
		{
			if (memcmp(pPacket+0x48,"\x01\x2b",2) == 0)//������������İ������ 01 2b
			{
				FormatMsg(packh,pPacket,1);
			}
			else if(memcmp(pPacket+0x48,"\x01\x30",2) == 0)//������������ͻ��˵İ������ 01 30 
			{
				FormatMsg(packh,pPacket,2);
			}

		}


 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "TaoBaoSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}


// дʵ���ļ�
void TaoBaoSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_taobao.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBuflen > 0)
	{
		fwrite(m_pMsgBuf, m_MsgBuflen, 1, fp);
		m_MsgBuflen = 0;
		delete[] m_pMsgBuf;
		m_pMsgBuf = NULL;
	}
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}
//16�����ַ���ת����10���� ��61 61-��aa

// CString hexstr_to_int10str(const char *s)
// {
// 	const char* p = s;
// 	int len = strlen(s);
// 	CString res;
// 	char* tp = res.GetBufferSetLength(len/4); 
// 	int i = 0;
// 	int hh = 0;
// 	while (i < len)
// 	{
// 		int temp;
// 		char tmpchar[1];
// 		memset(tmpchar, 0, 1);
// 		memcpy(tmpchar, p+i, 2);
// 		sscanf(tmpchar, "%x", &temp);
//  		CString overcver;
//  		overcver.Format("%c", temp);
// 		CString aaa;//������˾仰ʱ����ֵ������
// 		memcpy(tp+hh, (char*)(LPCTSTR)overcver, 1);
// 
// 		i+=4;
// 		hh++;
// 	}
// 	return res;
// }
// 
// //�ַ�ת����16���ƣ��� aa-��61 61
// 
// CString int10str_to_hexstr(const char *s)
// {
// 	const char* p = s;
// 	int len = strlen(s);
// 	CString res;
// 	char* tp = res.GetBufferSetLength(len*2); 
// 	int i = 0;
// 	while (i < len)
// 	{
// 		CString converstr;
// 		//02x��Ϊ�˴���\8֮���ת����ֻ��һ��8
// 		//ֻȡ��λ��Ϊ�˺���%02x֮����8�ֽڣ�ֻȡ����
// 		converstr.Format("%02x",*(p+i));
// 		converstr = converstr.Right(2);
// 		memcpy(tp+2*i,(char*)(LPCTSTR)converstr,2);
// //		memcpy(tp+4*i+2,"  ",2);
// 		i++;
// 	}
// 	return res;
// 
// }


int TaoBaoSession::FormatMsg(DATEHEADER& packh, const char* pPacket,int Direction)
{
	int error=0;
	g_error=0;
	try
	{
		int datalen;
		datalen=ntohl(*(DWORD*)(pPacket+0x50/*0x24*/));
		if (datalen > 2048)
		{
			return 1;
		}
		CString trimuser;
		char userid[0x50],touser[0x50];
		memcpy(userid,pPacket+15,56);
		trimuser.Format("%s", userid);
		trimuser.TrimRight();
//		CString strtemp = ConverHttp2Html(trimuser);
error=1;
g_error=1;
		if (/*trimuser.IsEmpty() && */m_userid.IsEmpty() && Direction == 1)
		{
			if(memcmp(trimuser, "cntaobao", 8) == 0
				||memcmp(trimuser, "cnalichn", 8) == 0 )
			{
				trimuser = trimuser.Mid(8);
			}
			m_userid.Format("%s",trimuser);
		}
		if (Direction == 2)
		{
			trimuser = m_userid;
		}
	//	if (datalen < g_Msglen)
		{
//			char pass[8];

			//�����������md5����ʼ������
			TAOBAOPWD* pwdlist = 0;
error=2;
g_error=2;

			if (SearchTaobaoPwd(trimuser, pwdlist))
			{
				// ��ԭSessionKey
//				BYTE*   pTmpData;
//				int		ptmplen;
error=3;
g_error=3;
				while (pwdlist!=NULL)
				{			
					char taobaomd[20];
					memset(taobaomd,0,20);
					memcpy(taobaomd,pwdlist->pwd2md5,4);
					CString aa = int10str_to_hexstr(taobaomd);
					sprintf(taobaomd,"%s",aa);
error=4;
g_error=4;
					Bian_Des_SetKey((char*)taobaomd);
					memset(g_pMsgBuffer,0,datalen);
					char* decodebuf = new char[datalen+2];
					memcpy(decodebuf,pPacket+0x54,datalen);
error=5;
g_error=5;
					if (datalen > 136)
					{
						
						CString tell;
						memcpy((char*)userid,decodebuf,64);
						memcpy((char*)touser,decodebuf+64,64);
//	WriteLog(IMSGPRO,"GetTaobao,userid%s,m_usid=%s,taobaomd:%s.neirong:",userid,m_userid,taobaomd);
//	WriteLog(IMSGPRO,"%s",pPacket+0x54);
						CString taobaouser;
						taobaouser.Format("cntaobao%s",trimuser);
						if (taobaouser.CompareNoCase(userid)!=0)
						{
							Bian_Dec_TaoBao((unsigned char*)decodebuf,datalen);
							if (Direction ==1 )
							{
								memcpy((char*)userid,decodebuf,64);
								memcpy((char*)touser,decodebuf+64,64);
							}
							else
							{
								memcpy((char*)touser,decodebuf,64);
								memcpy((char*)userid,decodebuf+64,64);

							}

							if (taobaouser.CompareNoCase(userid)!=0)
							{

								CString tmpmapstr,tmptrimuser;
								tmptrimuser.Format("%s--%s",trimuser,pwdlist->longpwd);
// 								if (!g_wrongtaobaomap.Lookup(tmptrimuser,tmpmapstr))
// 								{
// 									CString tmpstr;
// 									tmpstr.Format("%s\1%s\1%s\1imsg_taobao\1%s",trimuser,pwdlist->longpwd,"0","0");
// 									WriteArrToData(IMSG_PROTOCOL, ERROR_DATA, tmpstr, NULL);
// 									g_wrongtaobaomap.SetAt(tmptrimuser,tmpmapstr);
// 								}
								pwdlist = pwdlist->pNext;

								delete[] decodebuf;
								continue;
							}

						}
						if (Direction == 1)
						{
							tell.Format("%s �� %s:",userid,touser);
						}
						else
						{
							tell.Format("%s �� %s:",userid,touser);

						}
						int telllen = tell.GetLength();
						int num;
						if (Direction == 1)
						{
							num = decodebuf[133];
						}
						else
						{
							num = decodebuf[132];
						}
						if (telllen+num-4 < MSGBUFFERLEN&&
							num > 6)
						{
//							g_Msglen = datalen;
							memcpy(g_pMsgBuffer,(char*)(LPCTSTR)tell,telllen);

error=6;
g_error=6;
							memcpy(g_pMsgBuffer+telllen,decodebuf+136,num-6);
error=611;
g_error=611;

							memcpy(g_pMsgBuffer+telllen+num-6,"\r\n",2);
							g_Msglen = telllen+num-4;	
							WriteLog(IMSGPRO,"%s",g_pMsgBuffer);

						}
error=61;
g_error=61;
						CString tmpmapstr;
// 						if (!g_righttaobaomap.Lookup(trimuser,tmpmapstr))
// 						{
// error=62;
// g_error=62;
// 
// 							CString tmpstr;
// 							tmpstr.Format("%s\1%s\1%s\1imsg_taobao\1%s",trimuser,pwdlist->longpwd,"0","1");
// error=63;
// g_error=63;
// 
// 							WriteArrToData(IMSG_PROTOCOL, ERROR_DATA, tmpstr, NULL);
// 							g_righttaobaomap.SetAt(trimuser,tmpmapstr);
// 						}
						delete[] decodebuf;
						break;
error=7;
g_error=7;
					}
					else
					{
						pwdlist = pwdlist->pNext;

						delete[] decodebuf;
						continue;

					}
// 					CString tmpmapstr;
// 					if (!g_wrongtaobaomap.Lookup(trimuser,tmpmapstr))
// 					{
// 						CString tmpstr;
// 						tmpstr.Format("%s\1%s\1%s\1imsg_taobao\1%s",trimuser,pwdlist->longpwd,"0","0");
// 						WriteArrToData(IMSG_PROTOCOL, ERROR_DATA, tmpstr, NULL);
// 						g_wrongtaobaomap.SetAt(trimuser,tmpmapstr);
// 					}

				}
			}

		}
		if (g_Msglen > 0)
		{
error=8;
g_error=8;
			if (m_begfiletm == 0)
			{
				m_begfiletm = packh.m_tm;
			}

			// дʵ���ļ�
			memset(g_pMsgBuffer + g_Msglen, 0, 4);
			if (g_Msglen + m_MsgBuflen >= MSGBUFLEN)
			{
				// д�ļ�
				WriteRealFile(g_pMsgBuffer, g_Msglen);
			}
			else
			{
				if (m_pMsgBuf == NULL)
				{
					m_pMsgBuf = new char[MSGBUFLEN];
				}
				memcpy(m_pMsgBuf + m_MsgBuflen, g_pMsgBuffer, g_Msglen+1);
				m_MsgBuflen += g_Msglen;
			}
			g_Msglen = 0;
		}
		return 1;
	}
	catch (...)
	{
		WriteLog(IMSGPRO,"FormatMsg error=%d,g_error=%d",error,g_error);
	}
	return 1;
}

int TaoBaoSession::FormatLoginOrOut(DATEHEADER& packh, const char* pPacket)
{
	CString str;
	CString taobaostate;
	
	char tempuserid[50] = {0};
	char userid[0x50];
	memset(userid,0,0x50);
	int len;
	if(memcmp(pPacket,"\x88\x06\x00\x00\x00\x00\x00",7) == 0 &&
		memcmp(pPacket+24,"\x03\x06\x00\x01",4) == 0 &&
		packh.m_datalen > (195+50))
	{
		len = pPacket[195];
		if (len <= 0)
		{
			return 0;
		}
		memcpy(userid,pPacket+196,min(len,50));
		//����
		str.Format("stat= ����\r\n");
		taobaostate.Format("����");

	}
	else
	{
		if (packh.m_datalen >= 15+56)
		{
			memcpy(userid,pPacket+15,56);
			memcpy(tempuserid,pPacket+68,56);
			
		}
		if (memcmp(pPacket+0x48,"\x01\xf5",2) == 0)
		{
			//����
			str.Format("stat= ����\r\n");
			taobaostate.Format("����");

		}
		else if (memcmp(pPacket+0x48,"\x01\x04",2) == 0)
		{
			//����
			str.Format("stat= ����\r\n");
			taobaostate.Format("����");

		}
	}

	CString ttype ;
	if (m_userid.IsEmpty())
	{
		if(memcmp(userid, "cntaobao", 8) == 0
		 ||memcmp(userid, "cnalichn", 8) == 0 )
		{
			ttype = userid;
			strcpy(userid, ttype.Mid(8));
			
			ttype = ttype.Left(8);
		}
		CString strtemp = ConverHttp2Html(tempuserid);
		CString strtemp2;
		int temp22 =strlen(tempuserid);
		ConvertUtf8toGbk(tempuserid,56,strtemp2);
		m_userid.Format("%s", userid);
		m_userid.TrimLeft();
		if (m_userid.IsEmpty())
		{
			return 0;
		}
	}
		// ƥ�䰸����Ԥ����������Ϣ
	m_OpenCaseFlg = v_opencasetaobao;
	m_OpenWarnFlg = v_openwarntaobao;
	m_OpenIndxFlg = v_openflagtaobao;

		// д�����ļ�
	if (m_realfname.IsEmpty())
	{
		m_indexfname = CreateFilename(m_sip, m_dip, "imsg_taobao.txt.indexfile.txt");
	}
	else
	{
		m_indexfname = m_realfname + ".indexfile.txt";
	}
	WriteRealFile((char*)(LPCTSTR)taobaostate, taobaostate.GetLength());
	//Ҫд��ʵ�壬�����ռ�ʱ����ʵ�嶼��浽һ���ļ��У�����ʵ���ļ�������ղ���Ӧ������������
//	str.Format("%s", m_userid);
	DWORD dwstate = 0;
	SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwstate,"imsg_taobao");
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	// ����ļ�
	if(ttype.IsEmpty())
		str.Format("%suser= %s\r\n", (CString)str, m_userid);
	else
		str.Format("%stype= %s\r\nuser= %s\r\n", (CString)str, ttype, m_userid);


//	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_taobao.indexfile.txt");
	ASSERT(!m_indexfname.IsEmpty());
	WriteIndexFile("imsg_taobao", "imsg_taobao", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	return 1;
}
