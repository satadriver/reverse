//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	UC
//	版本:		1.0
//	创建日期:	2008-3-8
//	作者:		lp
//	主要功能:	uc聊天的还原
//	模块描述:	
//	主要函数:	
//
//**************************************************************** 
#include "stdafx.h"
#include "imsg.h"
#include "uccrypt.h"
#include "Uc.h"

UCIMSession::UCIMSession(DATEHEADER& packh, int aT)
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

UCIMSession::~UCIMSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL UCIMSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0)
		{
			// 自从上次存文件以来一直没有包到达就销毁该会话
			return TRUE;
		}
		
		// 输出文件
		if (m_MsgBuflen > 0)
		{
			// 写实体文件
			WriteRealFile(g_pMsgBuffer, 0);
		}
		
		if (!m_realfname.IsEmpty())
		{
			// 这里不处理IDCServer 和 活动历史
			// 处理数据源
			if(m_dataSrc == DATASRC_OTHER)
			{
				if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
					|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
				{
					m_dataSrc = DATASRC_IDC;
				}
			}
	// 		if (GetFileLength(m_realfname) <= 0)
	// 		{
	// 			DeleteFile(m_realfname);
	// 			return false;
	// 		}
			// 匹配案件和预警
			m_OpenCaseFlg = v_opencaseuc;
			m_OpenWarnFlg = v_openwarnuc;
			m_OpenIndxFlg = v_openflaguc;
			DWORD state = 0;
			SearchCaseOrWarn(this, "", 0, state);
		
			// 匹配语种
			CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

			// 写对照文件
			if (m_realfname.IsEmpty())
			{
				m_indexfname = CreateFilename(m_sip, m_dip, "imsg_uc.txt.indexfile.txt");
			}
			else
			{
				m_indexfname = m_realfname + ".indexfile.txt";
			}
			CString str;
			if (m_userid.IsEmpty())
			{
				str.Format("stat= 通讯\r\n");
			}
			else
			{
				str.Format("user= %s\r\nstat= 通讯\r\n", m_userid);
			}
			WriteIndexFile("imsg_uc", "imsg_uc", str, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_realfname.Empty();
			m_begfiletm = 0;
			m_closetm += 180;
			return FALSE;
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
		WriteLog(IMSGPRO, "UCIMSession::CloseSession() Error!!");
		return TRUE;
	}
}

int UCIMSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int error=0;
	try
	{
		if(!IsUcPacket(packh, pPacket))
		{
			return FALSE;
		}
error=1;	
		if (m_begfiletm == 0)
		{
			m_begfiletm = packh.m_tm;
		}
// 		WORD plen = *((WORD*)pPacket);
// 		if ((plen <= 12) || (plen > packh.m_datalen))
// 		{
// 			return 0;
// 		}
		const char *tp = pPacket;
		g_Msglen = 0;


/*		OpenRealFile();
		FILE* fp = fopen(m_realfname, "ab");
		if (fp == NULL)
		{
			return 0;
		}

		fclose(fp);*/
error=2;
//		if (packh.m_type==UDPPacket)
		{
			parse_uc((const unsigned char*)pPacket,packh.m_datalen,NULL,packh);
		}
error=3;		
		if (g_Msglen > 0)
		{
			// 写实体文件
			memset(g_pMsgBuffer + g_Msglen, 0, 4);
			if (g_Msglen + m_MsgBuflen >= MSGBUFLEN)
			{
				// 写文件
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

 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "UcSession::InsertPacket() Error!!=%d",error);
		return -3;
	}
	return 0;
}


// 打开实体文件
void UCIMSession::OpenRealFile()
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_uc.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
}
// 写实体文件
void UCIMSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_uc.txt");
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

#define  PRINT_SECTION  \
	section_len = (u_short)*ptext;\
	ptext += 2;\
	sub_len -= 2;\
	if(ptext+section_len>text+textlen)\
		return 0;\
	for(i=0; i<section_len; i++)\
	{\
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);\
		sub_len--;\
	}
//  to print a section, length as header 2-byte indexed.
// 	for(i=0; i<section_len; i++)\
// 	{\
// 		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);\	
// 		sub_len--;\fprintf(pipe, "%c", *ptext++);\
// 	}			sprintf(g_pMsgBuffer+g_Msglen, "%s",ptext);\	
	


#define	PRINT_UCID \
	ucid = *(u_int*)ptext;\
	g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%d\n", ucid);\
	ptext += 8;\
//  to print a 8-byte ucid.

#define PRINT_UCNICK \
	section_len = *ptext++;\
	if(ptext+section_len>text+textlen)\
		return 0;\
	fprintf(pipe, "UC nick is:\t");\
	for(i=0; i<section_len; i++)\
	{\
		g_Msglen+ = sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);\
	}\
	g_Msglen+ = sprintf(g_pMsgBuffer+g_Msglen, "\n");\
//	to print a uc nickname, one byte indexed.



int UCIMSession::outputtext_tcp(u_char* text, u_short textlen, FILE* pipe)
{
	u_short data_len, sub_len, section_len, count;
	u_short room_id;
	u_int   i;
	u_char* ptext;
	BOOL IsFinishFont = FALSE;

	ptext    = text;
	sub_len  = 0;
	data_len = (u_short)(*ptext);

	if( data_len+2 > textlen )
		return 0;

	ptext += 4;
	room_id = *(u_short*)(ptext);
	ptext += 4;

	data_len = *(u_short*)(ptext);
	if(ptext+data_len>text+textlen)
	{
		return 0;
	}

	ptext += 2;

	switch( *(u_short*)ptext )
	{
	case UC_WEB_CHAT:
		ptext += 0x0E;
		sub_len = *(u_short*)ptext;
		ptext += 2;

//		PRINT_SECTION
	section_len = (u_short)*ptext;
	ptext += 2;
	sub_len -= 2;
	if(ptext+section_len>text+textlen)
		return 0;
	for(i=0; i<section_len; i++)
	{
//		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);
		ptext++;
		sub_len--;
	}
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%d → %d:", 
			*(u_int*)(ptext+sub_len-17), *(u_int*)(ptext+sub_len-9) );


//		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, " (Font:");

		section_len = *(u_short*)ptext;
		ptext += 2;
		sub_len -= 2;
		
		for(i=0; i<section_len; i++)
		{
			if( '|' == *ptext )
			{
//				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, ")\n");
				ptext++;
				sub_len--;
				IsFinishFont = TRUE;
				continue;
			}
			if (IsFinishFont)
			{
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);
			}
			else
			{
				ptext++;
			}
			sub_len--;
			if (g_Msglen  > MSGBUFFERLEN - 500)
			{
				return 1;
			}

		}

		for(i=0; i+0x11 < sub_len; i++)
		{
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%c", *ptext++);

			if (g_Msglen  > MSGBUFFERLEN - 500)
			{
				return 1;
			}
		}
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
// 		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n%%1=%d, %%2=%d\n", 
// 			*(u_int*)ptext, *(u_int*)(ptext+8) );

		return 1;

	case UC_WEB_USERIN:
//		outputBinary(text, textlen, pipe);
/*		ptext += 6;
		g_Msglen+= sprintf(g_pMsgBuffer+g_Msglen, "用户(ucid=%d)(", *(u_int*)ptext);
		ptext += 8;
		PRINT_SECTION
		g_Msglen+= sprintf(g_pMsgBuffer+g_Msglen, ")进入本房间\t");
// 		for(i=0; i<8; i++)
// 		{
// 			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%02x", *ptext++);
// 		}
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");*/
		return 1;

	case UC_WEB_USEROUT:
/*		ptext += 6;
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "用户(ucid=%d)离开本房间\n", *(u_int*)ptext);*/
		return 1;
	case UC_WEB_BROADCAST:
		return 1;
		ptext += 2;
		if( 0 == *(int*)ptext )
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "★系统广播★\n");
		else
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "★房间广播★(ucid=%d)发布\n", *(u_int*)ptext);
		ptext += 8;
		PRINT_SECTION		
		fprintf(pipe, "\n");
		return 1;
	case UC_WEB_ROOMINFO:
		return 1;
		ptext += 0x0B;
		PRINT_SECTION		

		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\nIP Address:");
		PRINT_SECTION		
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\tPort:%d", *(u_short*)ptext);
		ptext += 2;

		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n★公告★本聊天室的主题是：");
		PRINT_SECTION		

		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n★公告★\n");
		PRINT_SECTION		
		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
		return 1;

	case UC_WEB_GETLIST:
		return 1;
		ptext += 6;
		count =  *(u_short*)ptext;
		ptext += 2;

		g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "聊天室中的%d人名单：\n",count);
		for(; count--; )
		{
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "ucid=%d", *(u_int*)ptext );
			ptext += 8;

			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\tnick=");
			PRINT_SECTION		
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\t");
// 			for(i=0; i<8; i++)
// 			{
// 				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%02x", *ptext++);
// 			}
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
		}
		return 1;

	case UC_WEB_CHAT2:
	case UC_WEB_UNK1:
	case UC_WEB_UNK2:
	case UC_WEB_UNK3:
	case UC_WEB_UNK4:
	case UC_WEB_UNK5:
	default:
		break;
	}

	return 0;
}

int UCIMSession::outputtext_udp(u_char* text, u_short textlen, FILE* pipe)
{
int error=0;
	try
	{
		u_short data_len, sub_len, section_len, cmd, i;
 		u_int   ucid;
		u_char* ptext, direction;
		BOOL direct = true;//正向
		CString tmpreceiver;
		CString str;
		DWORD dwstate = 0;
		CString ucstate;


		ptext    = text;
		data_len = (u_short)(*ptext);
error=1;
		if(data_len+2>textlen)
			return 0;

		ptext += 2;
		direction = *(++ptext);

	sub1:
		sub_len = (u_short)(*ptext);
		ptext += 2;

		switch( cmd = *(u_short*)ptext )
		{
		case UC_CMD_CHG_STATUS:
		case UC_CMD_CHG_STATUS_68:
/*
error=5;
			ptext += 2;
			ucid   = *(u_int*)ptext;
			ptext += 0x08;

			if(UC_CMD_CHG_STATUS == cmd)
				ptext++;	//should be a byte of 0x01.
			ptext++;	//should be former status.
error=6;
			switch(*ptext++)
			{
			case UC_STATUS_OFFLINE:
				//seems like some time counting data in this packet.
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "ucid %d is logging out!\n", ucid);
				return 1;

			case UC_STATUS_HIDDEN:
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "ucid %d is hidden.\n", ucid);
				return 1;

			case UC_STATUS_ONLINE:
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "ucid %d is online!\t", ucid);
				PRINT_SECTION
				fprintf(pipe, "\n");
				return 1;
			
			case UC_STATUS_GOAWAY:
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "ucid %d is away!\t", ucid);
				PRINT_SECTION
				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
				return 1;
			default:
				return 0;
			}*/
return 0;
		case UC_CMD_RREQ_FRI:
		case UC_CMD_RREQ_FRI+1:
		case UC_CMD_FRI_LIST:
		case UC_CMD_FRI_LIST+1:
		case UC_CMD_SEARCH:
		case UC_CMD_SEARCH+1:
		case UC_CMD_FRI_STATUS:
		case UC_CMD_FRI_STATUS+1:
			break;

		case UC_CMD_TMP_STORE:
			// send to server to store temporarily.
			// get from server the stored info.
			ptext += 2;
error=7;
			ucid = *(u_int*)ptext;
			tmpreceiver.Format(" → %d",ucid);
			ptext += 8;


			goto sub1;
		case UC_CMD_TMP_STORE+1:
			// get from server the stored info.
error=8;
			ptext += 2;
			ucid = *(u_int*)ptext;
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "%d ← ", ucid);
			ptext += 8;

			if (m_userid.IsEmpty())
			{
				m_userid.Format("%u",ucid);
			}


			goto sub1;

		case UC_CMD_BROADCAST:
			ptext += 3;
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "Broadcast content:\n");
			PRINT_SECTION
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
			return 1;

			break;
		case UC_CMD_REDIR:
			// redirected packet, need decipher again.
			ptext += 2;

			//data is meaningful for us.
			ptext += 0x10;
error=9;
			ptext += 5;
			section_len = (u_short)*ptext;
			if(section_len%8) return 0;
			ptext += 2;

			for(i=0; i<section_len/8; i++)
			{
				UC_decipher( (u_long*)(ptext+8*i), (u_long*)(ptext+4+8*i) );
			}
error=9;
			return outputtext_udp(ptext, section_len, pipe);

		case 0x00E7:
		case 0x00E8:	//older version?
		case 0x00EA:
		case 0x00EB:	// get friend info details.
			break;

		case 0x03E9: //4.01.920, 4.10.625
		case 0x0487: //2.40 ~ 4.00.200
			ptext += 2;

error=10;
			char username[MAX_PATH];
			memset(username,0,MAX_PATH);
			section_len = (u_short)*ptext;
			ptext += 2;
			sub_len -= 2;
			if(ptext+section_len>text+textlen)
				return 0;

			for(i=0; i<section_len; i++)
			{
				sprintf(username+i, "%c", *ptext++);
				sub_len--;
			}

			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen,username);
	error=11;
// 				if (m_userid.IsEmpty() && 
// 					!tmpreceiver.IsEmpty())
// 				{
// 					m_userid.Format("%s", username);
// 				}

			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "[昵称:\t");
			PRINT_SECTION
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "]");
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, tmpreceiver);

// 			if (!tmpreceiver.IsEmpty())
// 			{
// 				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen,username);
// 	error=11;
// 				if (m_userid.IsEmpty() && 
// 					!tmpreceiver.IsEmpty())
// 				{
// 					m_userid.Format("%s", username);
// 				}
// 
// 				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "[昵称:\t");
// 				PRINT_SECTION
// 				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "]");
// 				g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, tmpreceiver);
// 			}
// 			else
// 			{
// 				if (direction==60)
// 				{
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen,username);
// 					if (m_userid.IsEmpty() && 
// 						!tmpreceiver.IsEmpty())
// 					{
// 						m_userid.Format("%s", username);
// 					}
// 
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "[昵称:\t");
// 					PRINT_SECTION
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "] → ");
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, tmpreceiver);
// 				}
// 				else
// 				{
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen," ← ");
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen,username);
// // 					if (m_userid.IsEmpty() && 
// // 						!tmpreceiver.IsEmpty())
// // 					{
// // 						m_userid.Format("%s", username);
// // 					}
// // 
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "[昵称:\t");
// 					PRINT_SECTION
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "]");
// 					g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, tmpreceiver);
// 
// 				}
// 			}
error=12;
	//		fprintf(pipe, "Receiver:\t(see Destination IP and port)\n");
	//		if you don't parse friend status packet and keep their status, you
	//		won't know who is the Receiver. While the real Receiver, the one
	//		listening on the correct ip and port knows that it is for him or her.

			ptext += 6;
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, ":\t");
			PRINT_SECTION
			g_Msglen += sprintf(g_pMsgBuffer+g_Msglen, "\n");
			// following are other information such as font. 
error=13;
			return 1;

		case UC_CMD_ADD_FRI:
			// add friend request.
			break;

		case UC_CMD_KEEP_ALIVE_C8:
		case UC_CMD_KEEP_ALIVE_C8+1:
		case UC_CMD_KEEP_ALIVE_CB:
		case UC_CMD_KEEP_ALIVE_CB+1:
			break;

		case 0x6365:	//ack of comman packet.
			// infact "echo" here(65 63 68 6F)
			break;

		default:
			{
			error=2;
			// 0559F682  A1 01 08 00 36 2E 30 31 2E 36 30 30 AB FC 6B 66  ....6.01.600kf
			// 0559F692  00 00 00 00 00 00 0B 00 64 6B 73 74 61 72 5F 73  ........dkstar_s
			// 0559F6A2  69 6E 61 00 00 00 00 00 00 00 00 00 00 00 00 00  ina.............
			// 0559F6B2  00 00 00 00 0D D8 01 00 00 01 0C 00 31 39 32 2E  ............192.
			// 0559F6C2  31 36 39 2E 33 2E 32 32 87 60 00 01 00 00 00 00  169.3.22嘸......
			// 0559F6D2  00 00 00 00 01 00 00 00 00 00 00 00 AB FC 6B 66  ............kf
			// 0559F6E2  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

			// CHECK IS LOGIN
			ptext += 2;	// SKIP 2CMDBYTE

			section_len = (u_short)*ptext;
			ptext += 2;
			sub_len -= 2;
			if(ptext+section_len>text+textlen)
				return 0;
			if(section_len != 8)
				return 0;
			if(ptext[1] != 0x2e || ptext[4] != 0x2e)
				return 0;
			ptext += 8;
			sub_len -= 8;
// 			for(i=0; i<section_len; i++)
// 			{
// 				ptext++;
// 				sub_len--;
// 			}
			ucid = *(u_int*)ptext;
			ptext += 10;

			m_userid.Format("%u",ucid);
			SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwstate);
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
			
			error=4;
			// 输出文件
			str.Format("stat=上线\r\nuser= %s\r\n", m_userid);
			ucstate.Format("上线");

			char pass[32];
			memset(pass, 0, 32);
			section_len = (u_short)*ptext;
			ptext += 2;
			if(section_len > 4 && section_len < 32)
			{
				for (int ii = 0; ii < section_len && ptext[ii] != 0; ii++)
				{
					pass[ii] = ptext[ii];
				}
				if(strlen(pass) == section_len)
				{
					SearchCaseOrWarn(this, pass, section_len, dwstate);
	// 				SearchCaseEmailIm(this, pass, section_len, 0);
					
					error=4;
					// 输出文件
					str.Format("%spass= %s\r\n", CString(str), pass);
				}
			}
			
			if (m_realfname.IsEmpty())
			{
				m_indexfname = CreateFilename(m_sip, m_dip, "imsg_uc.txt.indexfile.txt");
			}
			else
			{
				m_indexfname = m_realfname + ".indexfile.txt";
			}
			
			WriteRealFile((char*)(LPCTSTR)ucstate, ucstate.GetLength());
			//要写上实体，否则收集时有无实体都会存到一个文件中，出现实体文件段与对照不对应，索引出问题
			
			m_OpenCaseFlg = v_opencaseuc;
			m_OpenWarnFlg = v_openwarnuc;
			m_OpenIndxFlg = v_openflaguc;
			//			m_realfname.Empty();
			//			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_uc.indexfile.txt");
			ASSERT(!m_indexfname.IsEmpty());
			WriteIndexFile("imsg_uc", "imsg_uc", str, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			return 1;
			
			if( UC_CMD_LOGIN_54 == cmd )
				//	此时 *ptext == x （x=1代表通行证方式，x=2手机号，x=3邮件） 
				ptext += 2;
			if( UC_CMD_LOGIN_58 == cmd )
				ptext ++;
			ptext += 0x0E;
			
			ptext += 20;
			m_closetm = time(0);
			return 1;
			}
			break;
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "outputtext_udp Error!!=%d",error);
	}
	return 0;
}

u_short UCIMSession::parse_uc( const u_char * byteArray, const size_t byteArrayLen, FILE* pipe ,DATEHEADER& packh)
{
	int error=0;
	try
	{
		u_short  i;
		u_char*  pdata;
		u_int    datalen;
		uc_header           *puh;
		uc_webchat_header   *puwch;
	//	ip_info				*pi;
		if (packh.m_datalen<sizeof(uc_header))
		{
			return 0;
		}
		if (packh.m_type == UDPPacket)
		{
			pdata = (u_char*)(byteArray);

			puh = (uc_header*)pdata;
			datalen = packh.m_datalen;
error=1;			
			if (puh->begin_tag != UC_BEGIN_TAG)
				return 0;
			
			if(datalen - puh->uc_cipher_len	- UC_HEAD_LEN != 0 )
				return 0;
			
			if(puh->uc_unknown2 != UC_UNKNOWN2 )
				return 0;
error=2;			
			for(i=0; i < puh->uc_cipher_len/8; i++)
			{
				UC_decipher((u_long*)&puh->data[8*i],(u_long*)&puh->data[4+8*i]);
			}
error=3;
			outputtext_udp(puh->data, puh->uc_cipher_len, pipe);
			return 1;
		}
		else if (packh.m_type == TCPPacket)
		{
			pdata = (u_char*)(byteArray); 
			//maybe multi-message in one packet.
			datalen = packh.m_datalen;
error=4;
			do
			{
				if (datalen < sizeof(uc_webchat_header))
				{
					break;
				}
				puwch = (uc_webchat_header*)pdata;
				
				if (puwch->begin_tag != UC_BEGIN_TAG)
					return 0;
				if (puwch->begin_tag2 != UC_BEGIN_TAG2)
					return 0;
				if( puwch->uc_cipher_len + UC_HEAD_LEN > datalen)
					return 0;
				

error=5;				
				if (*((u_short*)(&puwch->data[puwch->uc_cipher_len])) != UC_END_TAG2)
					return 0;

				if (*(u_char*)(&puwch->data[puwch->uc_cipher_len+2]) != UC_END_TAG)
					return 0;
error=6;

				if (*((u_short*)(&puwch->data[puwch->uc_cipher_len+3])) != puwch->uc_cipher_len)
					return 0;
error=7;
				for(i=0; i < puwch->uc_cipher_len/8; i++)
				{
					UC_decipher((u_long*)&puwch->data[8*i],(u_long*)&puwch->data[4+8*i]);
				}
error=8;				
	//			outputBinary(puwch->data, puwch->uc_cipher_len, pipe);
	// 			if(outputtext_tcp(puwch->data, puwch->uc_cipher_len, pipe))
	// 				printip(pi, pipe);
				outputtext_tcp(puwch->data, puwch->uc_cipher_len, pipe);
				//one subpacket parsed!

				datalen -= (puwch->uc_cipher_len + UC_HEAD_LEN);
				pdata += (puwch->uc_cipher_len + UC_HEAD_LEN);

			}
			while(datalen > 0);

			if(datalen == 0)
				return 1;
			else
				return 0;
		}
error=9;

		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "pare uc () Error!!=%d",error);
	}
	return 0;
}