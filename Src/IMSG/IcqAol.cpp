//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	IcqAol
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	ICQ/AOL协议还原
//	模块描述:	
//	主要函数:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "IcqAol.h"


BOOL FormatIcqMessage(char* tp, int len);
BOOL FormatIcqFriend(char* databuf, int len,
					 CStringArray &friends, CStringArray &waitauths);


IcqAolSession::IcqAolSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 180;
	m_ProcessTime = packh.m_tm;
	m_processseq1 = 0;
	m_processseq2 = 0;
	m_bSortList = FALSE;
	m_MsgBuf = NULL;
	m_MsgBufLen = 0;

	
	g_ImsgSion++;
}

IcqAolSession::~IcqAolSession()
{
	if (m_MsgBuf != NULL)
	{
		delete[] m_MsgBuf;
	}

	g_ImsgSion--;
}

static Search	s_fe("纮");	// "\0xC0\0x80"	// 数据边界字符
static char		s_strhead[] = "{\\rtf1\\ansi\\ansicpg936\\deff0\\deflang1033\\deflangfe2052{\\fonttbl{\\f0\\fnil\\fcharset134 Arial;}}\r\n{\\colortbl ;\\red0\\green64\\blue128;}\r\n\viewkind4\\uc1\\pard\\cf1\\lang2052\\f0\\fs20";
static char		s_strtail1[] = "\\f1\\par\r\n}\r\n";
static char		s_strtail2[] = "\\par\r\n}\r\n";
static unsigned char basechar[]="\xF3\x26\x81\xC4\x39\x86\xDB\x92\x71\xA3\xB9\xE6\x53\x7A\x95\x7C";


int IcqAolSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if (packh.m_type==ProxyTCP)
		{
			m_ProxyType = 1;
		}

		DTcpPacket ourgamepack(packh, (CHAR *)pPacket);
		m_bSortList = FALSE;
		m_lastpacktm = packh.m_tm;
		m_PacketList.push_back(ourgamepack);
		ourgamepack.buf = NULL;
		if (abs(packh.m_tm - m_ProcessTime) >= 40)
		{
			// 处理链表中的包
			ProcessIcqList(packh.m_tm);
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "IcqSession::InsertPacket() Error!!");
		return -3;
	}
}

BOOL IcqAolSession::CloseSession(time_t nowtm)
{
	try
	{
// 		if (m_begfiletm == 0)
// 		{
// 			ASSERT(m_ProcessTime);
// 			m_begfiletm = m_ProcessTime;
// 		}
		ProcessIcqList(nowtm);
		if (m_MsgBufLen > 0)
		{
			WriteRealFile(g_pMsgBuffer, 0);
		}
		if (!m_realfname.IsEmpty())
		{
			// 匹配IDC数据源
			if (m_dataSrc == DATASRC_OTHER)
			{
				if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)
					|| v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP))
				{
					m_dataSrc = DATASRC_IDC;
				}
			}

			// 匹配案件、预警和屏蔽信息
			DWORD dwstate = 0;
			CString casetxt;
			casetxt.Format("%s______%s", m_userid,m_pass);
			SearchCaseOrWarn(this, casetxt, casetxt.GetLength(), dwstate);
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

			m_OpenCaseFlg = v_opencaseaol;
			m_OpenWarnFlg = v_openwarnaol;
			m_OpenIndxFlg = v_openflagaol;

			// 匹配语种
			CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

			// 输出文件
			CString text;
			if (!m_userid.IsEmpty())
			{
				if (!m_pass.IsEmpty())
				{
					text.Format("user= %s\r\npass= %s\r\n",
						m_userid, m_pass);
				}
				else
				{
					text.Format("user= %s\r\n", m_userid);
				}
			}
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile("imsg_icqaol", "imsg_icqaol", text, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_begfiletm = m_ProcessTime;
		}

		if ((nowtm - m_lastpacktm) > 360)
		{
			return TRUE;		// 结束会话
		}
		else
		{
			m_closetm += 180;
			return FALSE;		// 保留会话
		}
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "IcqSession::CloseSession() Error!!");
		return TRUE;
	}
}

// 处理链表中的包
void IcqAolSession::ProcessIcqList(time_t now)
{
	if (!m_bSortList)
	{
		m_PacketList.sort();
		m_bSortList = TRUE;
	}
	g_Msglen = 0;
	
 	DTcpPacketListIt item, end;
	item = m_PacketList.begin();
	end = m_PacketList.end();
	while(item != end)
	{
		DTcpPacket* pPacket = &(*item);
		if ((now - pPacket->tm) < 20)		// 保留20秒的数据
		{
			break;
		}
		m_ProcessTime = pPacket->tm;
		int direction = 0;
		if (memcmp(pPacket->sip, m_csip, 4) == 0)
		{
			if ((m_processseq1 == 0)
				|| (0x80000000 > (pPacket->Ssequence - m_processseq1)))
			{
				m_processseq1 = pPacket->Ssequence;
				direction = 1;
			}
		}
		else
		{
			if ((m_processseq2 == 0)
				|| (0x80000000 > (pPacket->Ssequence - m_processseq2)))
			{
				m_processseq2 = pPacket->Ssequence;
				direction = 2;
			}
		}

		if (direction != 0)
		{
			int msglen = ProcessIcqPacket(pPacket->tm, pPacket->buf, pPacket->Slen,
				direction, g_pMsgBuffer + g_Msglen, MSGBUFFERLEN - g_Msglen);
			if (msglen > 0)
			{
				g_Msglen += msglen;
			}
		}
		DTcpPacketListIt tmpitem = item;
		item++;
		m_PacketList.erase(tmpitem);
		if (g_Msglen > MSGBUFFERLEN - 8192)
		{
			break;
		}
	}
	
	if (g_Msglen > 0)
	{
		// 写实体文件
		memset(g_pMsgBuffer + g_Msglen, 0, 4);
		if (g_Msglen + m_MsgBufLen >= MSGBUFLEN)
		{
			// 写文件
			WriteRealFile(g_pMsgBuffer, g_Msglen);
		}
		else
		{
			if (m_MsgBuf == NULL)
			{
				m_MsgBuf = new char[MSGBUFLEN];
			}
			memcpy(m_MsgBuf + m_MsgBufLen, g_pMsgBuffer, g_Msglen+1);
			m_MsgBufLen += g_Msglen;
		}
		g_Msglen = 0;
	}
	return;
}

// 写实体文件
void IcqAolSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_icqaol.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBufLen > 0)
	{
		fwrite(m_MsgBuf, m_MsgBufLen, 1, fp);
		m_MsgBufLen = 0;
		delete[] m_MsgBuf;
		m_MsgBuf = NULL;
	}
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}

// 处理一个单包
int IcqAolSession::ProcessIcqPacket(time_t optm, char* pdata, int datalen,
	int direction, char* pmsgbuf, int msgbuflen)
{
int err = 0;
	int Msglen = 0;
	try
	{
		char Msgstr[2048];
		memset(Msgstr, 0, sizeof(Msgstr));
		int msgdirection = 0;			// 消息的方向(1表示发 2表示收)
		CString linkman;				// 联系人
		BOOL bSwap = FALSE;		// 是否交换IP
		BOOL bWHis = FALSE;		// 是否写IDCServer和活动历史(只在登录时才写)	
err=1;
		if ((memcmp(pdata, "\x2a\x01", 2) == 0)
			&& (datalen > 50) && (datalen < 150)
			&& (memcmp(pdata + 10, "\x0\x1", 2) == 0))
		{
			int lenth = Swap2Byte(*((WORD*)(pdata + 12)));
			if ((lenth + 14) > datalen)
			{
				return 0;
			}

			if (memcmp(pdata + 14 + lenth, "\x0\x2", 2) != 0)
			{
				return 0;
			}

			memcpy(m_userid.GetBufferSetLength(lenth), pdata + 14, lenth);
			
			int tlenth = Swap2Byte(*((WORD*)(pdata + 16 + lenth)));
			if ((tlenth + 16 + lenth) > datalen)
			{
				return 0;
			}

err=2;
			// Client --> Server
			if (direction == 2)
			{
				bSwap = TRUE;
			}
			bWHis = TRUE;
			linkman = "服务器";
			msgdirection = 1;

			if (tlenth <= sizeof(basechar))
			{
				unsigned char ctmp[24];
				memset(ctmp, 0, sizeof(ctmp));
				memcpy(ctmp, pdata + 18 + lenth, tlenth);
				for(int i=0; i<tlenth; i++)
				{
					ctmp[i] = ctmp[i] ^ basechar[i];
				}
				m_pass.Format("%s", ctmp);
				
				sprintf(Msgstr, "用户%s登录,密码：%s\r\n\r\n", m_userid, ctmp);
			}
			else
			{
				sprintf(Msgstr, "用户%s登录\r\n\r\n", m_userid);
			}
		}
		else if (memcmp(pdata, "\x2a\x02", 2) == 0) 
		{
			int addlenth, templenth, tlvlenth, inlenth, tlvin, lenth = 0;
			char id[32];
err=3;
			while (lenth < datalen)
			{
				addlenth = Swap2Byte(*((WORD*)(pdata + lenth + 4)));
				if (lenth + 6 + addlenth > datalen)
				{
					return Msglen;
				}

err=4;
				if (memcmp(pdata + 6 + lenth, "\x0\x4\x0\x6", 4) == 0)		//outgoing
				{
					// Client --> Server
					if (direction == 2)
					{
						bSwap = TRUE;
					}
					msgdirection = 1;
					templenth = *(pdata+26);
					if ((27 + templenth) > datalen) 
					{
						return Msglen;
					}
					memset(id, 0, sizeof(id));
err=5;
					memcpy(id, pdata + 27, templenth);
					linkman = id;
					if (memcmp(pdata + 27 + templenth, "\x0\x5", 2) == 0)	//rendez vous data
					{
						tlvlenth = Swap2Byte(*((WORD*)(pdata + 29 + templenth)));
						if (templenth + 31 + tlvlenth > datalen)
						{
							return Msglen;
						}
						if (tlvlenth > addlenth)
						{
							return Msglen;
						}
						templenth = 30 + 27 + templenth;
						while (1)
						{
err=6;
							if (memcmp(pdata + templenth, "\x27\x11", 2) == 0)
							{
								inlenth=Swap2Byte(*((WORD*)(pdata + templenth + 2)));
								if (templenth + 4 + inlenth > datalen)
								{
									return Msglen;
								}
								tlvin = *((WORD*)(pdata + templenth + 4));
								if (templenth + 6 + tlvin > datalen) 
								{
									return Msglen;
								}
								templenth = templenth + 4 + tlvin + 2;
								tlvin = *((WORD*)(pdata + templenth));
								if (templenth + tlvin + 2 > datalen)
								{
									return Msglen;
								}
								templenth = templenth + 2 + tlvin;
								if (pdata[templenth] == 1)
								{
									tlvin = *((WORD*)(pdata + templenth + 6));
									if (templenth + 2 + tlvin > datalen) 
									{
										return Msglen;
									}
									memset(Msgstr + (tlvin&~3), 0, 8);
									memcpy(Msgstr, pdata + templenth + 8, tlvin);
									break;
								}
								else
								{
									return Msglen;
								}
							}
							else
							{
err=7;
								tlvlenth = Swap2Byte(*((WORD*)(pdata + templenth + 2)));
								templenth = templenth + tlvlenth + 4;
								if (templenth > addlenth)
								{
									return Msglen;
								}
							}
						}
					}
					else if(memcmp(pdata + 27 + templenth, "\x0\x2", 2) == 0)		//message
					{
err=8;
						tlvlenth = Swap2Byte(*((WORD*)(pdata + 29 + templenth)));
						if (31 + templenth + tlvlenth > datalen)
						{
							return Msglen;
						}
						if (tlvlenth > addlenth)
						{
							return Msglen;
						}
						tlvin = Swap2Byte(*((WORD*)(pdata + 29 + templenth + 4)));
						if (37 + templenth > datalen)
						{
							return Msglen;
						}
						templenth = templenth + 35 + tlvin;
						tlvin = Swap2Byte(*((WORD*)(pdata + templenth + 2)));
						if (4 + templenth + tlvin > datalen)
						{
							return Msglen;
						}
						memset(Msgstr + ((tlvin - 4) & ~3), 0, 8);
err=9;
						memcpy(Msgstr, pdata + templenth + 8, tlvin - 4);
					}
					// Client --> Server
					if (direction == 2)
					{
						bSwap = TRUE;
					}
					int len = strlen(Msgstr);
					if (len > 0)
					{
err=10;
						FormatIcqMessage(Msgstr, len);
					}
				}
				else if (memcmp(pdata+6+lenth,"\x0\x4\x0\x7",4)==0)//incoming
				{
					// Server --> Client
err=11;
					if (direction == 1)
					{
						bSwap = TRUE;
					}
					msgdirection = 2;
					templenth = *(pdata+26);
					if ((templenth > 29) || (templenth + 27 > datalen))
					{
						return Msglen;
					}
					memset(id, 0, sizeof(id));
err=12;
					memcpy(id, pdata + 27, templenth);
					linkman = id;
					int lenth = Swap2Byte(*((WORD*)(pdata + 29 + templenth)));
					if (31 + templenth + lenth > datalen) 
					{
						return Msglen;
					}
					int k = 0;
					templenth = templenth + 29 + 2;
					while (k < lenth + 1)
					{
err=13;
						int num = Swap2Byte(*((WORD*)(pdata + templenth + 2)));
						if (memcmp(pdata + templenth, "\x0\x2", 2) == 0)
						{
							if (4 + templenth + num > datalen) 
							{
								return Msglen;
							}
// 							if (num > 100)
// 							{
// 								return Msglen;
// 							}
							num = Swap2Byte(*((WORD*)(pdata + templenth + 2 + 4)));
							if (8+templenth+num>datalen) 
							{
								return Msglen;
							}
							templenth+=6+num;
							num=Swap2Byte(*((WORD*)(pdata+templenth+4)));
							if (templenth+6+num>datalen) 
							{
								return Msglen;
							}
							memset(Msgstr + ((num - 4) & ~3), 0, 8);
err=14;
							memcpy(Msgstr, pdata+templenth+10, num-4);
							break;
						}
						else if ((num > 30)
							&& (memcmp(pdata + templenth, "\x0\x5", 2) == 0))
						{
err=15;
							if ((num + templenth) > datalen) 
							{
								return Msglen;
							}
							templenth += 30;
							while (1)
							{
err=16;
								if (memcmp(pdata + templenth, "\x27\x11", 2) == 0)
								{
									inlenth=Swap2Byte(*((WORD*)(pdata+templenth+2)));
									if (templenth+4+inlenth>datalen) 
									{
										return Msglen;
									}
									tlvin=*((WORD*)(pdata+templenth+4));
									if (templenth+6+tlvin>datalen) 
									{
										return Msglen;
									}
									templenth=templenth+4+tlvin+2;
									tlvin=*((WORD*)(pdata+templenth));
									if (templenth+tlvin+2>datalen) 
									{
										return Msglen;
									}
									templenth=templenth+2+tlvin;
									if (memcmp(pdata+templenth,"\x1",1)==0)
									{
										tlvin=*((WORD*)(pdata+templenth+6));
										if (templenth+2+tlvin>datalen) 
										{
											return Msglen;
										}
										memset(Msgstr + (tlvin & ~3), 0, 8);
err=17;
										memcpy(Msgstr, pdata+templenth+8, tlvin);
										break;
									}
									else
									{
										return Msglen;
									}
								}
								else
								{
									tlvlenth=Swap2Byte(*((WORD*)(pdata+templenth+2)));
									templenth = templenth + tlvlenth + 4;
									if (templenth > num)
									{
										return Msglen;
									}
								}
							}
						}
						else
						{
err=18;
							int num=Swap2Byte(*((WORD*)(pdata+templenth+2)));
							if (num>100)
							{
								return Msglen;
							}
							templenth=templenth+4+num;
							if (templenth>datalen) 
							{
								return Msglen;
							}
						}
						k++;
					}
					int len = strlen(Msgstr);
					if (len > 0)
					{
err=19;
						FormatIcqMessage(Msgstr, len);
					}
				}
				else if (memcmp(pdata+6+lenth,"\x0\x13\x0\x6",4)==0)//friend
				{
err=20;
					// Server --> Client
					if (direction == 1)
					{
						bSwap = TRUE;
					}
					msgdirection = 2;
					CStringArray friends;
					CStringArray waitauths;
					friends.SetSize(0, 32);
					waitauths.SetSize(0, 8);
err=21;
					if (FormatIcqFriend(pdata+lenth, datalen - lenth, friends, waitauths))
					{
err=22;
						linkman = "服务器";
						int len = sprintf(Msgstr, "获取好友列表\r\n好友: \r\n");
 						for (int i = 0;  i < friends.GetSize(); i++)
						{
							len += sprintf(Msgstr + len, "%s\r\n", friends[i]);
						}
						if (friends.GetSize()>0)
						{
							len += sprintf(Msgstr + len, "待验证用户:\r\n");
							for (int j = 0;  j < waitauths.GetSize(); j++)
							{
								len += sprintf(Msgstr + len, "%s\r\n", waitauths[j]);
							}
						}
						strcpy(Msgstr + len, "\r\n");
					}
				}
				lenth += addlenth + 6;
			}	
		}

err=23;
		if (bSwap)
		{
err=230;
			// 交换ip方向
			CString tstr = m_sip;
			m_sip = m_dip;
			m_dip = tstr;
			tstr = m_dmac;
			m_dmac = m_smac;
			m_smac = tstr;
			DWORD ip;
			memcpy(&ip, m_csip, 4);
			memcpy(m_csip, m_cdip, 4);
			memcpy(m_cdip, &ip, 4);
			m_AuthInfo.SwapAuthInfo();
			ip = m_processseq1;
			m_processseq1 = m_processseq2;
			m_processseq2 = ip;
		}

		if (bWHis)
		{
			if (m_dataSrc == DATASRC_OTHER)
			{
err=24;
				if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)
					|| v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP))
				{
					m_dataSrc = DATASRC_IDC;
				}
			}

			CString isp;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), isp))
			{
err=25;
				// 写IDCServer
				CString idcstr;
				idcstr.Format("%s\1%s\1yahoo\1即时消息\1%s\1icq/aol\1%s\1\1", 
					m_dip.Left(15), isp, m_dip.Right(5), TYPESTATE_DECIDED);
				v_IdcServerStat[IMSG_PROTOCOL].AddServer(idcstr);
			}
			if (v_stat && v_statImsgOther)
			{
err=26;
				CString hisstr;
				HistoryRecord hisRec;
				hisRec.SetRecord(IDX_ID,		m_userid);
				hisRec.SetRecord(IDX_USER,		m_userid);

				hisRec.SetRecord(IDX_FORWTYPE,	"1");
				hisRec.SetRecord(IDX_STATE,	"上线");
				if(m_ProxyType==1)
					hisRec.SetRecord(IDX_IFAGENT, "2");
				WriteHisData("imsg", "imsg_icqaol", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
			}
		}
		
		if (Msgstr[0] != 0)
		{
err=27;
			Msglen += GetTimeString(optm, "ICQ/AOL   %Y-%m-%d %H:%M:%S    ",
				pmsgbuf + Msglen, 128);
			if (msgdirection == 2)
			{
				Msglen += sprintf(pmsgbuf + Msglen, "%s ← %s    %s ← %s :\r\n%s\r\n\r\n",
					m_sip, m_dip, m_userid, linkman, Msgstr);
			}
			else
			{
				Msglen += sprintf(pmsgbuf + Msglen, "%s → %s    %s → %s :\r\n%s\r\n\r\n",
					m_sip, m_dip, m_userid, linkman, Msgstr);
			}
		}
err=28;
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "ProcessIcqPacket() Error=%d!!", err);
		return 0;
	}
	return Msglen;
}
// 		if (memcmp(pack.buf,"\x2a\x01",2)==0 &&//登录
// 			pack.Slen>50 && pack.Slen<150&&
// 			memcmp(pack.buf+10,"\x0\x1",2)==0)
// 		{
// 			static unsigned char basechar[]="\xF3\x26\x81\xC4\x39\x86\xDB\x92\x71\xA3\xB9\xE6\x53\x7A\x95\x7C";
// // 			int i/*,j=0*/;
// 			int lenth=Swap2Byte(*((WORD*)(pack.buf+12)));
// 			if (lenth+14>pack.Slen)
// 				return false;
// 			if (memcmp(pack.buf+14+lenth,"\x0\x2",2)!=0)
// 				return false;
// 			memcpy(s->userid.GetBufferSetLength(lenth),pack.buf+14,lenth);
// //			Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录",s->userid);
// 			
// 			int tlenth=Swap2Byte(*((WORD*)(pack.buf+16+lenth)));
// 			if (tlenth+16+lenth>pack.Slen)
// 				return false;
// 			Msglen+=GetTimeString(packtm, "ICQ/AOL   %Y-%m-%d %H:%M:%S    ", pMsgBuffer+Msglen, 128);
// 			s->direction = direction;
// 			if (s->direction==1)
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → 服务器 :\r\n",
// 					s->sip, s->dip, s->userid);
// 			}
// 			else
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → 服务器 :\r\n",
// 					s->dip, s->sip, s->userid);
// 			}
// 			if (tlenth<=sizeof(basechar))
// 			{
// 				unsigned char ctmp[24];
// 				memset(ctmp, 0, sizeof(ctmp));
// 				memcpy(ctmp,pack.buf+18+lenth,tlenth);
// 				for(int i=0; i<tlenth; i++)
// 				{
// 					ctmp[i]=ctmp[i]^basechar[i];
// 				}
// 				s->password.Format("%s",ctmp);
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录,密码：%s\r\n\r\n",s->userid,ctmp);
// 			}
// 			else
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录\r\n\r\n", s->userid);
// 			}
// 			if (v_stat && v_statImsgOther)
// 			{
// 				CString sipaddr, dipaddr;
// 				char strtm[20];
// 				GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", strtm, 20);
// 				HistoryRecord hisRec;
// 				hisRec.SetRecord(IDX_RECTM,		strtm);
// 				if(s->direction ==1)
// 				{
// 					sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 					dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 					
// 					hisRec.SetRecord(IDX_SIP,		s->sip);
// 					hisRec.SetRecord(IDX_DIP,		s->dip);
// 					hisRec.SetRecord(IDX_SPORT,		s->sip.Right(5));
// 					hisRec.SetRecord(IDX_DPORT,		s->dip.Right(5));
// 					
// 					hisRec.SetRecord(IDX_PHONE,		s->Phone1);
// 					hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount1);
// 					hisRec.SetRecord(IDX_DPHONE,	s->Phone2);
// 					hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount2);
// 				}
// 				else
// 				{
// 					sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 					dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 					
// 					hisRec.SetRecord(IDX_SIP,		s->dip);
// 					hisRec.SetRecord(IDX_DIP,		s->sip);
// 					hisRec.SetRecord(IDX_SPORT,		s->dip.Right(5));
// 					hisRec.SetRecord(IDX_DPORT,		s->sip.Right(5));
// 
// 					hisRec.SetRecord(IDX_PHONE,		s->Phone2);
// 					hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount2);
// 					hisRec.SetRecord(IDX_DPHONE,	s->Phone1);
// 					hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount1);
// 				}
// 				hisRec.SetRecord(IDX_SIPADDR,	sipaddr);
// 				hisRec.SetRecord(IDX_DIPADDR,	dipaddr);
// 				hisRec.SetRecord(IDX_DATASRC,	s->m_dataSrc);
// 				hisRec.SetRecord(IDX_PRO,		"imsg_icqaol");
// 				hisRec.SetRecord(IDX_ID,		s->userid);
// 				//hisRec.SetRecord(IDX_USER,		s->userid);
// 				hisRec.SetRecord(IDX_PASS,		s->password);
// 				hisRec.SetRecord(IDX_IMSGSTA,	"登录");
// 				if(s->m_ProxyType==1)
// 					hisRec.SetRecord(IDX_IFAGENT, "2");
// 
// 				CString tempstr;
// 				if(hisRec.GetHisRecord(tempstr))
// 					WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, tempstr);
// 			}
// 		}
// 		else if (memcmp(pack.buf,"\x2a\x02",2)==0) 
// 		{
// 			int addlenth,templenth,tlvlenth,inlenth,tlvin,lenth=0;
// 			char id[30];
// 			CString temtalk;
// 			
// 			while (lenth<pack.Slen)
// 			{
// 				addlenth=Swap2Byte(*((WORD*)(pack.buf+lenth+4)));
// 				if (lenth+6+addlenth>pack.Slen) 
// 					return false;
// 				if (memcmp(pack.buf+6+lenth,"\x0\x4\x0\x6",4)==0)//outgoing
// 				{
// 					templenth=*(pack.buf+26);
// 					if (27+templenth>pack.Slen) 
// 						return false;
// 					memset(id,0,30);
// 					memcpy(id,pack.buf+27,templenth);
// 					
// 					if (memcmp(pack.buf+27+templenth,"\x0\x5",2)==0)//rendez vous data
// 					{
// 						tlvlenth=Swap2Byte(*((WORD*)(pack.buf+29+templenth)));
// 						if (templenth+31+tlvlenth>pack.Slen) 
// 							return false;
// 						if (tlvlenth>addlenth)
// 							return false;
// 						templenth=30+27+templenth;
// 						while (1)
// 						{
// 							if (memcmp(pack.buf+templenth,"\x27\x11",2)==0)
// 							{
// 								inlenth=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 								if (templenth+4+inlenth>pack.Slen) 
// 									return false;
// 								tlvin=*((WORD*)(pack.buf+templenth+4));
// 								if (templenth+6+tlvin>pack.Slen) 
// 									return false;
// 								templenth=templenth+4+tlvin+2;
// 								tlvin=*((WORD*)(pack.buf+templenth));
// 								if (templenth+tlvin+2>pack.Slen) 
// 									return false;
// 								templenth=templenth+2+tlvin;
// 								if (memcmp(pack.buf+templenth,"\x1",1)==0)
// 								{
// 									tlvin=*((WORD*)(pack.buf+templenth+6));
// 									if (templenth+2+tlvin>pack.Slen) 
// 										return false;
// 									memcpy(temtalk.GetBufferSetLength(tlvin),pack.buf+templenth+8,tlvin);
// //									Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s\r\n%s:%s\r\n\r\n",
// //										s->sip, s->dip, id,temtalk);
// 									temtalk.ReleaseBuffer();
// 									break;
// 								}
// 								else
// 									return false;
// 							}
// 							else
// 							{
// 								tlvlenth=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 								templenth=templenth+tlvlenth+4;
// 								if (templenth>addlenth)
// 									return false;
// 							}
// 						}
// 
// 					}
// 					else if(memcmp(pack.buf+27+templenth,"\x0\x2",2)==0)//message
// 					{
// 						tlvlenth=Swap2Byte(*((WORD*)(pack.buf+30+templenth)));
// 						if (32+templenth+tlvlenth>pack.Slen) 
// 							return false;
// 						if (tlvlenth>addlenth)
// 							return false;
// 						tlvin=Swap2Byte(*((WORD*)(pack.buf+30+templenth+4)));
// 						if (36+templenth>pack.Slen) 
// 							return false;
// 						templenth=templenth+34+tlvin;
// 						tlvin=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 						if (4+templenth+tlvin>pack.Slen) 
// 							return false;
// 						memcmp(temtalk.GetBufferSetLength(tlvin-4),pack.buf+templenth+8,tlvin-4);
// //						Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s\r\n%s:%s\r\n\r\n",
// //							s->sip, s->dip, id,temtalk);
// 						temtalk.ReleaseBuffer();
// 					}
// 					if (s->direction==0)
// 						s->direction = direction;
// 					if (!temtalk.IsEmpty())
// 					{
// 						FormatIcqMessage(temtalk);
// 						Msglen+=GetTimeString(packtm,"ICQ/AOL   %Y-%m-%d %H:%M:%S    ",
// 							pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → %s :\r\n%s\r\n",
// 								s->sip, s->dip, s->userid, id, temtalk);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → %s :\r\n%s\r\n",
// 								s->dip, s->sip, s->userid, id, temtalk);
// 						}
// 					}
// 
// 				}
// 				else if (memcmp(pack.buf+6+lenth,"\x0\x4\x0\x7",4)==0)//incoming
// 				{
// 					templenth=*(pack.buf+26);
// 					if (templenth>29 || templenth+27>pack.Slen)
// 						return FALSE;
// 					memcpy(id,pack.buf+27,templenth);
// 
// 					int lenth=Swap2Byte(*((WORD*)(pack.buf+29+templenth)));
// 					if (31+templenth+lenth>pack.Slen) 
// 						return false;
// 					int k=0;
// 					templenth=templenth+29+2;
// 					while (k<lenth+1)
// 					{
// 						if (memcmp(pack.buf+templenth,"\x0\x2",2)==0)
// 						{
// 							int num=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 							if (4+templenth+num>pack.Slen) 
// 								return false;
// 							if (num>100)
// 								return false;
// 							num=Swap2Byte(*((WORD*)(pack.buf+templenth+2+4)));
// 							if (8+templenth+num>pack.Slen) 
// 								return false;
// 							templenth+=6+num;
// 							num=Swap2Byte(*((WORD*)(pack.buf+templenth+4)));
// 							if (templenth+6+num>pack.Slen) 
// 								return false;
// 							memcpy(temtalk.GetBufferSetLength(num-4),pack.buf+templenth+10,num-4);
// 							break;
// 						}
// 						else
// 						{
// 							int num=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 							if (num>100)
// 								return false;
// 							templenth=templenth+4+num;
// 							if (templenth>pack.Slen) 
// 								return false;
// 						}
// 						k++;
// 					}
// //					Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s\r\n%s:%s\r\n\r\n",
// //						s->sip, s->dip, id,temtalk);
// 					if (s->direction==0)
// 						s->direction = (direction==1)? 2 : 1;
// 					if (!temtalk.IsEmpty())
// 					{
// 						FormatIcqMessage(temtalk);
// 						Msglen+=GetTimeString(packtm,"ICQ/AOL   %Y-%m-%d %H:%M:%S    ",
// 							pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← %s :\r\n%s\r\n",
// 								s->sip, s->dip, s->userid, id, temtalk);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← %s :\r\n%s\r\n",
// 								s->dip, s->sip, s->userid, id, temtalk);
// 						}
// 					}
// 				}
// 				else if (memcmp(pack.buf+6+lenth,"\x0\x13\x0\x6",4)==0)//friend
// 				{
// 					if (s->direction==0)
// 						s->direction = (direction==1)? 2 : 1;
// 					CStringArray friends;
// 					CStringArray waitauths;
// 					friends.SetSize(0, 32);
// 					waitauths.SetSize(0, 8);
// 					if (FormatIcqFriend(pack.buf+lenth, pack.Slen-lenth, friends, waitauths))
// 					{
// 						Msglen+=GetTimeString(packtm, "ICQ/AOL   %Y-%m-%d %H:%M:%S    ", pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← 服务器 :\r\n",
// 								s->sip, s->dip, s->userid);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← 服务器 :\r\n",
// 								s->dip, s->sip, s->userid);
// 						}
// 						Msglen+=sprintf(pMsgBuffer+Msglen, "获取好友列表\r\n好友: \r\n");
// 						if (friends.GetSize()>0)
// 						{
// 							for (int i = 0;  i < friends.GetSize(); i++)
// 							{
// 								Msglen+=sprintf(pMsgBuffer+Msglen, "%s\r\n", friends[i]);
// 							}
// 						}
// 						if (friends.GetSize()>0)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "待验证用户:\r\n");
// 							for (int j = 0;  j < waitauths.GetSize(); j++)
// 							{
// 								Msglen+=sprintf(pMsgBuffer+Msglen, "%s\r\n", waitauths[j]);
// 							}
// 						}
// 						strcpy(pMsgBuffer+Msglen, "\r\n");
// 						Msglen+=2;
// 					}
// 						
// //					int num=0;
// //					int ilenth=Swap2Byte(*((WORD*)(pack.buf+lenth+17)));
// //					num+=19;
// //					Msglen+=sprintf(pMsgBuffer+Msglen, "获得好友列表:");
// //					while (ilenth>0)
// //					{
// //						int lsubenth=Swap2Byte(*((WORD*)(pack.buf+lenth+num)));
// //						if (lsubenth>0)
// //						{
// //							CString tempstr;
// //							memcpy(tempstr.GetBufferSetLength(lsubenth),pack.buf+lenth+num+2,lsubenth);
// //							Msglen+=sprintf(pMsgBuffer+Msglen, "%s,",tempstr);
// //							tempstr.ReleaseBuffer();
// //						}
// //						num=num+lsubenth;
// //						lsubenth=Swap2Byte(*((WORD*)(pack.buf+lenth+num+8)));
// //						num=num+lsubenth+10;
// //						ilenth--;
// //					}
// 				}
// 				lenth+=addlenth+6;
// 			}
// 		}
//	return 0;
//}

//int ProcessYmsgPacket(MsnSession *s, TcpPacket& pack, int direction);

// BOOL ProcessIcqPacket(ImsgSession *s, time_t packtm, TcpPacket& pack, int direction);
// 
// BOOL FormatIcqFriend(char* databuf);
// 
// BOOL FormatIcqFriend(char* databuf, int len, CStringArray &friends, CStringArray &waitauths);

BOOL FormatIcqMessage(char* tp, int len)
{
	// 去除尾部附加信息
	if (memcmp(tp + len - sizeof(s_strtail1) + 1, s_strtail1, sizeof(s_strtail1)-1) == 0)
	{
		len -= sizeof(s_strtail1)-1;
		tp[len] = 0;
	}
	else if (memcmp(tp + len - sizeof(s_strtail2) + 1, s_strtail2, sizeof(s_strtail2) - 1) == 0)
	{
		len -= sizeof(s_strtail2)-1;
		tp[len] = 0;
	}
	// 去除头部附加信息
	char *p = strstr(tp, "\\fs20");
	if (p == NULL)
	{
		p = strstr(tp, "\\fs36");
	}
	if ((memcmp(tp, "{\\rtf1\\", 7) == 0) && p!=NULL)
	{
		p+=5;
		memmove(tp, p, len-(p-tp)+1);
		len -= (p-tp);
	}
	int pos1=0;
	int pos2=-1;
	BYTE m;
	char *p1 = tp;
	while(len>0)
	{
		if (*tp=='\\')
		{
			if (tp[1]=='\'')
			{
				if (pos1+4<=len)
				{
					if (tp[2]>='0'&&tp[2]<='9')
						m=(tp[2]-'0')<<4;
					else if (tp[2]>='a'&&tp[2]<='f')
						m=(tp[2]-'a'+10)<<4;
					else if (tp[2]>='A'&&tp[2]<='F')
						m=(tp[2]-'A'+10)<<4;
					else
						m = 0;
					if (tp[3]>='0'&&tp[3]<='9')
						m+=(tp[3]-'0');
					else if (tp[3]>='a'&&tp[3]<='f')
						m+=(tp[3]-'a'+10);
					else if (tp[3]>='A'&&tp[3]<='F')
						m+=(tp[3]-'A'+10)<<4;
					else
						m = 0;
				}
				if (m!=0)
					*p1++=m;
				tp+=4;
				len-=4;
			}
			else if (tp[1]=='\\')
			{
				*p1++='\\';
				tp+=2;
				len-=2;
			}
			else
			{
				*p1++ = *tp++;
				len--;
			}
		}
		else
		{
			*p1++ = *tp++;
			len--;
		}
	}
	if (p1!=tp)
	{
		*p1=0;
	}
	return TRUE;
}
// 
// int ProcessIcqList(MsnSession* s, time_t now)
// {
// #ifdef ADD_RUNSTATE_INFO
// 	v_imsgrunstate.AddState(5);
// #endif
// 	
// //	int buflen = MSGBUFFERLEN;
// 	Msglen = 0;
// 	if (s->sortlist1==0)
// 	{
// 		s->packetlist1.sort();
// 		s->sortlist1 = 1;
// 	}
// 	if (s->sortlist2==0)
// 	{
// 		s->packetlist2.sort();
// 		s->sortlist2 = 1;
// 	}
// 	TcpPacketListIt nowitem, item1, item2, end1, end2, tmpitem;//start,last,itl,tmpitl;
// 	item1 = s->packetlist1.begin();
// 	item2 = s->packetlist2.begin();
// 	end1 = s->packetlist1.end();
// 	end2 = s->packetlist2.end();
// 	int curpack = 0;
// 	BOOL pack1=FALSE, pack2=FALSE;
// //	time_t packtm = 0;
// #ifdef		_DEBUG
// 	TcpPacket	*p1, *p2;
// #endif
// 	do
// 	{
// 		while (!pack1 && item1!=end1)
// 		{
// #ifdef		_DEBUG
// 			p1 = &(*item1);
// #endif
// 			if (item1->Ssequence <= s->processseq1)
// 			{
// 				tmpitem = item1;
// 				item1++;
// 				s->packetlist1.erase(tmpitem);
// 				s->packcount--;
// 			}
// 			else
// 			{
// 				pack1 = TRUE;
// 				break;
// 			}
// 		}
// 		while (!pack2 && item2!=end2)
// 		{
// #ifdef		_DEBUG
// 			p2 = &(*item2);
// #endif
// 			if (item2->Ssequence <= s->processseq2)
// 			{
// 				tmpitem = item2;
// 				item2++;
// 				s->packetlist2.erase(tmpitem);
// 				s->packcount--;
// 			}
// 			else
// 			{
// 				pack2 = TRUE;
// 				break;
// 			}
// 		}
// 		if (pack1 && pack2)
// 		{
// 			if (item1->Nextsequence <= item2->Sack)
// 			{
// #ifdef ADD_RUNSTATE_INFO
// 				v_imsgrunstate.AddState(51);
// #endif
// 				s->lastdattm = item1->tm;
// 				curpack=1;
// 			}
// 			else
// 			{
// #ifdef ADD_RUNSTATE_INFO
// 				v_imsgrunstate.AddState(52);
// #endif
// 				s->lastdattm = item2->tm;
// 				curpack=2;
// 			}
// 		}
// 		else if (pack1)
// 		{
// #ifdef ADD_RUNSTATE_INFO
// 			v_imsgrunstate.AddState(53);
// #endif
// 			s->lastdattm = item1->tm;
// 			curpack=1;
// 		}
// 		else if (pack2)
// 		{
// #ifdef ADD_RUNSTATE_INFO
// 			v_imsgrunstate.AddState(54);
// #endif
// 			curpack=2;
// 			s->lastdattm = item2->tm;
// 		}
// 		else
// 			break;
// 		if (abs(now-s->lastdattm)<20)
// 			break;
// 		ASSERT(curpack==1 || curpack==2);
// 		if (curpack == 1)
// 		{
// 			tmpitem = item1;
// 			item1++;
// 			s->processseq1 = tmpitem->Ssequence;
// //			packtm = tmpitem->tm;
// #ifdef ADD_RUNSTATE_INFO
// 			v_imsgrunstate.AddState(55);
// #endif
// 			ProcessIcqPacket(s, tmpitem->tm, *tmpitem, 1);
// 			s->packetlist1.erase(tmpitem);
// 			s->packcount--;
// 			pack1=FALSE;
// 		}
// 		else
// 		{
// 			tmpitem = item2;
// 			item2++;
// 			s->processseq2 = tmpitem->Ssequence;
// //			packtm = tmpitem->tm;
// #ifdef ADD_RUNSTATE_INFO
// 			v_imsgrunstate.AddState(56);
// #endif
// 			ProcessIcqPacket(s, tmpitem->tm, *tmpitem, 2);
// 			s->packetlist2.erase(tmpitem);
// 			s->packcount--;
// 			pack2=FALSE;
// 		}
// 	}while(s->packcount>0 && Msglen<MSGBUFFERLEN-8192);
// 	
// #ifdef ADD_RUNSTATE_INFO
// 	v_imsgrunstate.AddState(57);
// #endif
// 	if (Msglen > 0)
// 	{
// 		if (Msglen+s->msglen >= MSGBUFLEN)
// 		{
// 			// 写文件
// 			WriteMsgTextFile(s, pMsgBuffer, Msglen, v_opencaseaol);
// 		}
// 		else
// 		{
// 			if (s->msgbuf==NULL)
// 				s->msgbuf = new char[MSGBUFLEN];
// 			memcpy(s->msgbuf+s->msglen, pMsgBuffer, Msglen+1);
// 			s->msglen += Msglen;
// 		}
// 		Msglen=0;
// 	}
// 	return 0;
// }
// 
// BOOL ProcessIcqPacket(ImsgSession *s, time_t packtm, TcpPacket& pack, int direction)
// {
// 	try{
// 		int blen = Msglen;
// 		if (memcmp(pack.buf,"\x2a\x01",2)==0 &&//登录
// 			pack.Slen>50 && pack.Slen<150&&
// 			memcmp(pack.buf+10,"\x0\x1",2)==0)
// 		{
// 			static unsigned char basechar[]="\xF3\x26\x81\xC4\x39\x86\xDB\x92\x71\xA3\xB9\xE6\x53\x7A\x95\x7C";
// // 			int i/*,j=0*/;
// 			int lenth=Swap2Byte(*((WORD*)(pack.buf+12)));
// 			if (lenth+14>pack.Slen)
// 				return false;
// 			if (memcmp(pack.buf+14+lenth,"\x0\x2",2)!=0)
// 				return false;
// 			memcpy(s->userid.GetBufferSetLength(lenth),pack.buf+14,lenth);
// //			Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录",s->userid);
// 			
// 			int tlenth=Swap2Byte(*((WORD*)(pack.buf+16+lenth)));
// 			if (tlenth+16+lenth>pack.Slen)
// 				return false;
// 			Msglen+=GetTimeString(packtm, "ICQ/AOL   %Y-%m-%d %H:%M:%S    ", pMsgBuffer+Msglen, 128);
// 			s->direction = direction;
// 			if (s->direction==1)
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → 服务器 :\r\n",
// 					s->sip, s->dip, s->userid);
// 			}
// 			else
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → 服务器 :\r\n",
// 					s->dip, s->sip, s->userid);
// 			}
// 			if (tlenth<=sizeof(basechar))
// 			{
// 				unsigned char ctmp[24];
// 				memset(ctmp, 0, sizeof(ctmp));
// 				memcpy(ctmp,pack.buf+18+lenth,tlenth);
// 				for(int i=0; i<tlenth; i++)
// 				{
// 					ctmp[i]=ctmp[i]^basechar[i];
// 				}
// 				s->password.Format("%s",ctmp);
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录,密码：%s\r\n\r\n",s->userid,ctmp);
// 			}
// 			else
// 			{
// 				Msglen+=sprintf(pMsgBuffer+Msglen, "用户%s登录\r\n\r\n", s->userid);
// 			}
// 			if (v_stat && v_statImsgOther)
// 			{
// 				CString sipaddr, dipaddr;
// 				char strtm[20];
// 				GetTimeString(packtm, "%Y-%m-%d %H:%M:%S", strtm, 20);
// 				HistoryRecord hisRec;
// 				hisRec.SetRecord(IDX_RECTM,		strtm);
// 				if(s->direction ==1)
// 				{
// 					sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 					dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 					
// 					hisRec.SetRecord(IDX_SIP,		s->sip);
// 					hisRec.SetRecord(IDX_DIP,		s->dip);
// 					hisRec.SetRecord(IDX_SPORT,		s->sip.Right(5));
// 					hisRec.SetRecord(IDX_DPORT,		s->dip.Right(5));
// 					
// 					hisRec.SetRecord(IDX_PHONE,		s->Phone1);
// 					hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount1);
// 					hisRec.SetRecord(IDX_DPHONE,	s->Phone2);
// 					hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount2);
// 				}
// 				else
// 				{
// 					sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 					dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 					
// 					hisRec.SetRecord(IDX_SIP,		s->dip);
// 					hisRec.SetRecord(IDX_DIP,		s->sip);
// 					hisRec.SetRecord(IDX_SPORT,		s->dip.Right(5));
// 					hisRec.SetRecord(IDX_DPORT,		s->sip.Right(5));
// 
// 					hisRec.SetRecord(IDX_PHONE,		s->Phone2);
// 					hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount2);
// 					hisRec.SetRecord(IDX_DPHONE,	s->Phone1);
// 					hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount1);
// 				}
// 				hisRec.SetRecord(IDX_SIPADDR,	sipaddr);
// 				hisRec.SetRecord(IDX_DIPADDR,	dipaddr);
// 				hisRec.SetRecord(IDX_DATASRC,	s->m_dataSrc);
// 				hisRec.SetRecord(IDX_PRO,		"imsg_icqaol");
// 				hisRec.SetRecord(IDX_ID,		s->userid);
// 				//hisRec.SetRecord(IDX_USER,		s->userid);
// 				hisRec.SetRecord(IDX_PASS,		s->password);
// 				hisRec.SetRecord(IDX_IMSGSTA,	"登录");
// 				if(s->m_ProxyType==1)
// 					hisRec.SetRecord(IDX_IFAGENT, "2");
// 
// 				CString tempstr;
// 				if(hisRec.GetHisRecord(tempstr))
// 					WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, tempstr);
// 			}
// 		}
// 		else if (memcmp(pack.buf,"\x2a\x02",2)==0) 
// 		{
// 			int addlenth,templenth,tlvlenth,inlenth,tlvin,lenth=0;
// 			char id[30];
// 			CString temtalk;
// 			
// 			while (lenth<pack.Slen)
// 			{
// 				addlenth=Swap2Byte(*((WORD*)(pack.buf+lenth+4)));
// 				if (lenth+6+addlenth>pack.Slen) 
// 					return false;
// 				if (memcmp(pack.buf+6+lenth,"\x0\x4\x0\x6",4)==0)//outgoing
// 				{
// 					templenth=*(pack.buf+26);
// 					if (27+templenth>pack.Slen) 
// 						return false;
// 					memset(id,0,30);
// 					memcpy(id,pack.buf+27,templenth);
// 					
// 					if (memcmp(pack.buf+27+templenth,"\x0\x5",2)==0)//rendez vous data
// 					{
// 						tlvlenth=Swap2Byte(*((WORD*)(pack.buf+29+templenth)));
// 						if (templenth+31+tlvlenth>pack.Slen) 
// 							return false;
// 						if (tlvlenth>addlenth)
// 							return false;
// 						templenth=30+27+templenth;
// 						while (1)
// 						{
// 							if (memcmp(pack.buf+templenth,"\x27\x11",2)==0)
// 							{
// 								inlenth=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 								if (templenth+4+inlenth>pack.Slen) 
// 									return false;
// 								tlvin=*((WORD*)(pack.buf+templenth+4));
// 								if (templenth+6+tlvin>pack.Slen) 
// 									return false;
// 								templenth=templenth+4+tlvin+2;
// 								tlvin=*((WORD*)(pack.buf+templenth));
// 								if (templenth+tlvin+2>pack.Slen) 
// 									return false;
// 								templenth=templenth+2+tlvin;
// 								if (memcmp(pack.buf+templenth,"\x1",1)==0)
// 								{
// 									tlvin=*((WORD*)(pack.buf+templenth+6));
// 									if (templenth+2+tlvin>pack.Slen) 
// 										return false;
// 									memcpy(temtalk.GetBufferSetLength(tlvin),pack.buf+templenth+8,tlvin);
// //									Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s\r\n%s:%s\r\n\r\n",
// //										s->sip, s->dip, id,temtalk);
// 									temtalk.ReleaseBuffer();
// 									break;
// 								}
// 								else
// 									return false;
// 							}
// 							else
// 							{
// 								tlvlenth=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 								templenth=templenth+tlvlenth+4;
// 								if (templenth>addlenth)
// 									return false;
// 							}
// 						}
// 
// 					}
// 					else if(memcmp(pack.buf+27+templenth,"\x0\x2",2)==0)//message
// 					{
// 						tlvlenth=Swap2Byte(*((WORD*)(pack.buf+30+templenth)));
// 						if (32+templenth+tlvlenth>pack.Slen) 
// 							return false;
// 						if (tlvlenth>addlenth)
// 							return false;
// 						tlvin=Swap2Byte(*((WORD*)(pack.buf+30+templenth+4)));
// 						if (36+templenth>pack.Slen) 
// 							return false;
// 						templenth=templenth+34+tlvin;
// 						tlvin=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 						if (4+templenth+tlvin>pack.Slen) 
// 							return false;
// 						memcmp(temtalk.GetBufferSetLength(tlvin-4),pack.buf+templenth+8,tlvin-4);
// //						Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s\r\n%s:%s\r\n\r\n",
// //							s->sip, s->dip, id,temtalk);
// 						temtalk.ReleaseBuffer();
// 					}
// 					if (s->direction==0)
// 						s->direction = direction;
// 					if (!temtalk.IsEmpty())
// 					{
// 						FormatIcqMessage(temtalk);
// 						Msglen+=GetTimeString(packtm,"ICQ/AOL   %Y-%m-%d %H:%M:%S    ",
// 							pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → %s :\r\n%s\r\n",
// 								s->sip, s->dip, s->userid, id, temtalk);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s → %s    %s → %s :\r\n%s\r\n",
// 								s->dip, s->sip, s->userid, id, temtalk);
// 						}
// 					}
// 
// 				}
// 				else if (memcmp(pack.buf+6+lenth,"\x0\x4\x0\x7",4)==0)//incoming
// 				{
// 					templenth=*(pack.buf+26);
// 					if (templenth>29 || templenth+27>pack.Slen)
// 						return FALSE;
// 					memcpy(id,pack.buf+27,templenth);
// 
// 					int lenth=Swap2Byte(*((WORD*)(pack.buf+29+templenth)));
// 					if (31+templenth+lenth>pack.Slen) 
// 						return false;
// 					int k=0;
// 					templenth=templenth+29+2;
// 					while (k<lenth+1)
// 					{
// 						if (memcmp(pack.buf+templenth,"\x0\x2",2)==0)
// 						{
// 							int num=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 							if (4+templenth+num>pack.Slen) 
// 								return false;
// 							if (num>100)
// 								return false;
// 							num=Swap2Byte(*((WORD*)(pack.buf+templenth+2+4)));
// 							if (8+templenth+num>pack.Slen) 
// 								return false;
// 							templenth+=6+num;
// 							num=Swap2Byte(*((WORD*)(pack.buf+templenth+4)));
// 							if (templenth+6+num>pack.Slen) 
// 								return false;
// 							memcpy(temtalk.GetBufferSetLength(num-4),pack.buf+templenth+10,num-4);
// 							break;
// 						}
// 						else
// 						{
// 							int num=Swap2Byte(*((WORD*)(pack.buf+templenth+2)));
// 							if (num>100)
// 								return false;
// 							templenth=templenth+4+num;
// 							if (templenth>pack.Slen) 
// 								return false;
// 						}
// 						k++;
// 					}
// //					Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s\r\n%s:%s\r\n\r\n",
// //						s->sip, s->dip, id,temtalk);
// 					if (s->direction==0)
// 						s->direction = (direction==1)? 2 : 1;
// 					if (!temtalk.IsEmpty())
// 					{
// 						FormatIcqMessage(temtalk);
// 						Msglen+=GetTimeString(packtm,"ICQ/AOL   %Y-%m-%d %H:%M:%S    ",
// 							pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← %s :\r\n%s\r\n",
// 								s->sip, s->dip, s->userid, id, temtalk);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← %s :\r\n%s\r\n",
// 								s->dip, s->sip, s->userid, id, temtalk);
// 						}
// 					}
// 				}
// 				else if (memcmp(pack.buf+6+lenth,"\x0\x13\x0\x6",4)==0)//friend
// 				{
// 					if (s->direction==0)
// 						s->direction = (direction==1)? 2 : 1;
// 					CStringArray friends;
// 					CStringArray waitauths;
// 					friends.SetSize(0, 32);
// 					waitauths.SetSize(0, 8);
// 					if (FormatIcqFriend(pack.buf+lenth, pack.Slen-lenth, friends, waitauths))
// 					{
// 						Msglen+=GetTimeString(packtm, "ICQ/AOL   %Y-%m-%d %H:%M:%S    ", pMsgBuffer+Msglen, 128);
// 						if (s->direction==1)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← 服务器 :\r\n",
// 								s->sip, s->dip, s->userid);
// 						}
// 						else
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "%s ← %s    %s ← 服务器 :\r\n",
// 								s->dip, s->sip, s->userid);
// 						}
// 						Msglen+=sprintf(pMsgBuffer+Msglen, "获取好友列表\r\n好友: \r\n");
// 						if (friends.GetSize()>0)
// 						{
// 							for (int i = 0;  i < friends.GetSize(); i++)
// 							{
// 								Msglen+=sprintf(pMsgBuffer+Msglen, "%s\r\n", friends[i]);
// 							}
// 						}
// 						if (friends.GetSize()>0)
// 						{
// 							Msglen+=sprintf(pMsgBuffer+Msglen, "待验证用户:\r\n");
// 							for (int j = 0;  j < waitauths.GetSize(); j++)
// 							{
// 								Msglen+=sprintf(pMsgBuffer+Msglen, "%s\r\n", waitauths[j]);
// 							}
// 						}
// 						strcpy(pMsgBuffer+Msglen, "\r\n");
// 						Msglen+=2;
// 					}
// 						
// //					int num=0;
// //					int ilenth=Swap2Byte(*((WORD*)(pack.buf+lenth+17)));
// //					num+=19;
// //					Msglen+=sprintf(pMsgBuffer+Msglen, "获得好友列表:");
// //					while (ilenth>0)
// //					{
// //						int lsubenth=Swap2Byte(*((WORD*)(pack.buf+lenth+num)));
// //						if (lsubenth>0)
// //						{
// //							CString tempstr;
// //							memcpy(tempstr.GetBufferSetLength(lsubenth),pack.buf+lenth+num+2,lsubenth);
// //							Msglen+=sprintf(pMsgBuffer+Msglen, "%s,",tempstr);
// //							tempstr.ReleaseBuffer();
// //						}
// //						num=num+lsubenth;
// //						lsubenth=Swap2Byte(*((WORD*)(pack.buf+lenth+num+8)));
// //						num=num+lsubenth+10;
// //						ilenth--;
// //					}
// 				}
// 				lenth+=addlenth+6;
// 			}
// 		}
// 	}
// 	catch (...)
// 	{
// 		WriteLog(strImsg,"error");
// 	}
// 
// 
// 	return 0;
// }
// 
BOOL FormatIcqFriend(char* databuf, int len, CStringArray &friends, CStringArray &waitauths)
{
	int tmplen = len;
	WORD friendcount = Swap2Byte(*((WORD*)(databuf+17)));
	databuf += 19;
	len -= 19;
	char strname[128];
	if (friendcount > 0 && friendcount<len/6)
	{
		while (friendcount-- > 0)
		{
			WORD tlen1 =Swap2Byte(*((WORD*)(databuf)));	// userid len
			WORD tlen2 = Swap2Byte(*((WORD*)(databuf+tlen1+8)));	// TLV len
			if (tlen1>0 && tlen1<16)
			{
				WORD *tp = (WORD*)(databuf+2+tlen1);
				if (*tp!=0 && tp[1]!=0 && tp[2]==0)
				{
					memset(strname, 0, sizeof(strname));
					memcpy(strname, databuf+2, tlen1);
					tp += 4;
					BOOL iswait = FALSE;
					for (WORD i = 0; i < tlen2; )
					{
						WORD tlen3 = Swap2Byte(tp[1]);
						if (i+tlen3+4 > tlen2)
							break;
						switch(*tp)
						{
						case 0x3101:	// 昵称
							if (tlen3>0 && tlen3<100)
							{
								memcpy(strname+tlen1, "[昵称:", 6);
								memcpy(strname+tlen1+6, (char*)(tp+2), tlen3);
								strname[tlen1+6+tlen3] = ']';
							}
							break;
						case 0x6600:	// 待验证
							iswait = TRUE;
							break;
						}
						i += tlen3+4;
						tp = (WORD*)((char*)tp+tlen3+4);
					}
					if (iswait)
						waitauths.Add(strname);
					else
						friends.Add(strname);
				}
			}
			len -= tlen1+tlen2+10;
			databuf += tlen1+tlen2+10;
			if (len<=20)
				break;
		}
		return (friends.GetSize()>0 || waitauths.GetSize()>0);
	}
	return FALSE;
}
