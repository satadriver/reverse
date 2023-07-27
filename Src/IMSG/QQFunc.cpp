// PubFunc.cpp: implementation of the CPubFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "..\\QQ_Crypt.h"
#include "imsg.h"
#include "QQFunc.h"

void WriteQQChatHis(OicqSession *s, CString strtm, DWORD dwRecvQQ, DWORD dwSendQQ);
//////////////////////////////////////////////////////////////////////
// 
BOOL DecryptQQPacket(BYTE* data, int& len, BYTE* key, BYTE* outbuf)
{
	int outlen = len;
	if (qq_crypt(DECRYPT, data, len, key, outbuf, &outlen))
	{
//		memcpy(data, pMsgBuffer, outlen);
		len = outlen;
		return TRUE;
	}
	else
		return FALSE;
}

void OutQQGroupReq(
			OicqSession *s,
			unsigned char *pQQData, 
			const int Datalen,
			char* msgbuf,
			int& msglen)
{
	unsigned char *pData = pQQData;
	switch(pData[0]) {
	case 0x1A:	// ����Ϣ(��չ)
		{
			pData += 1;	// ������0x1A
			pData += 4;	// Ⱥ�ڲ�ID
			pData += 2;	// ����ĳ���
			pData += 2;	// Type, 0x1���ı�, 0x2���Զ������
			pData += 4;	// ��Ƭ�� ��Ƭ��� ��ϢID2
			pData += 4;	// δ֪
			// printf("%s\r\n", pData); ��Ϣ����
		}
		break;
	case 0x01:	// ����Ⱥ
		{
			int Len;
			DWORD PartQQ;
			pData += 1;	// ������0x01
			pData += 1; // Ⱥ����(�̶�/��ʱ)
			pData += 1;	// �Ƿ���Ҫ��֤
			pData += 4;	// 2004����
			pData += 4; // 2005����
			Len = pData[0];
			pData += 1;	// Ⱥ���Ƴ���
			pData += Len;	// ����
			pData += 2;		// δ֪2
			Len = pData[0];
			pData += 1;	// ��������
			pData += Len;	// ����
			pData += 1;	// ��鳤��
			pData += Len;	// ���
			PartQQ = ntohl(*(DWORD*)pData);
			for(;PartQQ != 0;)
			{
				pData += 4;		// ��Ա�б�
				PartQQ = ntohl(*(DWORD*)pData);
			}
		}
		break;
	case 0x0C:	// �õ�Ⱥ��Ա����
	case 0x1D:	// ��ɢȺ
	case 0x09:	// �˳�Ⱥ
		break;
	default:
		break;
	}
	
	return ;
}

void OutUpdateInfo(
				OicqSession *s,
				unsigned char *pQQData, 
				const int Datalen,
				int QQCmd,
				char* msgbuf,
				int& msglen)
{
// 	char chrPart;
// 	CStringArray arrInfo;
// 	if(QQCmd == QQ_CMD_UPDATE_INFO)	
// 	{	// �����û���Ϣ
// 		chrPart = '\x1f';
// 	}
// 	else	
// 	{	// ����û���Ϣ
// 		chrPart = '\x1e';
// 		arrInfo.Add("");
// 	}
// 	char *pData = (char*)pQQData;
// 	char *pChar1F = strchr(pData, chrPart);
// 	while(pChar1F != NULL && ((char*)pQQData-pChar1F) < Datalen)
// 	{
// 		arrInfo.Add(CString(pData, pChar1F-pData));
// 		pData = pChar1F+1;
// 		pChar1F = strchr(pData, chrPart);
// 	}
// 	if(arrInfo.GetSize() < 30)
// 		return ;
// 
// 	int pos = 0;
// 	if (s->m_direction == 1)
// 	{
// 		if(QQCmd == QQ_CMD_UPDATE_INFO)
// 		{
// 			pos+=sprintf(msgbuf, "OICQ   %s �� %s     QQ(%s) �޸��û�����:\r\n",
// 				s->m_sip, s->m_dip, s->m_userid);
// 		}
// 		else
// 		{
// 			pos+=sprintf(msgbuf, "OICQ   %s �� %s     QQ(%s) ����û�����:\r\n",
// 				s->m_sip, s->m_dip, s->m_userid);
// 		}
// 	}
// 	else
// 	{
// 		if(QQCmd == QQ_CMD_UPDATE_INFO)
// 		{
// 			pos+=sprintf(msgbuf, "OICQ   %s �� %s     QQ(%s) �޸��û�����:\r\n",
// 				s->m_dip, s->m_sip, s->m_userid);
// 		}
// 		else
// 		{
// 			pos+=sprintf(msgbuf, "OICQ   %s �� %s     QQ(%s) ����û�����:\r\n",
// 				s->m_dip, s->m_sip, s->m_userid);
// 		}
// 	}
// 
// 	if(QQCmd == QQ_CMD_UPDATE_INFO)
// 	{
// 		if(arrInfo[0].Compare(arrInfo[1]) != 0)
// 			pos+=sprintf(msgbuf+pos, "�������, ������:%s, ������:%s\r\n", arrInfo[0], arrInfo[1]);
// 	}
// 	else
// 	{
// 		if(!arrInfo[1].IsEmpty())
// 			pos+=sprintf(msgbuf+pos, "QQ��:%s\r\n", arrInfo[1]);
// 	}
// 
// 	if(!arrInfo[2].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "�ǳ�:%s\r\n",	arrInfo[2]);
// 	if(!arrInfo[4].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "ʡ:%s\r\n",		arrInfo[4]);
// 	if(!arrInfo[6].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "��ַ:%s\r\n",	arrInfo[6]);
// 	if(!arrInfo[7].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "�绰:%s\r\n",	arrInfo[7]);
// 	if(!arrInfo[10].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "����:%s\r\n",	arrInfo[10]);
// 	if(!arrInfo[11].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "Email:%s\r\n",	arrInfo[11]);
// 	if(!arrInfo[23].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "�ֻ���:%s\r\n", arrInfo[23]);
// 	if(!arrInfo[25].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "����:\r\n%s\r\n",	arrInfo[25]);
// 	if(!arrInfo[26].IsEmpty())
// 		pos+=sprintf(msgbuf+pos, "����:%s\r\n",	arrInfo[26]);
// 	pos += sprintf(msgbuf+pos, "\r\n");
// 
// 	if(pos > 0)
// 	{
// 		msglen = pos;
// #ifdef _DEBUG
// 		printf("%s", CString(msgbuf, msglen));
// #endif
// 	}
	return ;
}

//  ����QQ�ķ�����Ϣ�İ�
void OutQQSendChatIM(
				OicqSession *s,
				unsigned char *pQQData,
				const int Datalen,
				char* msgbuf, 
				int& msglen)
{
	msglen = 0;
	if(Datalen <= 50)
	{
		return;
	}

	unsigned char *pData = pQQData;
	DWORD dwSendQQ, dwRecvQQ;
	DWORD msgType, senType;
	dwSendQQ = Swap4Byte(*((DWORD*)pData));			// ������Ϣ��QQ
	dwRecvQQ = Swap4Byte(*((DWORD*)(pData+4)));		// ������Ϣ��QQ

	DWORD msgID = 0;
	int lowID = 0;

	// srcQQ_4 dstQQ_4 srcVer_2 srcQQ_4 dstQQ_4
	pData += 18;
	// md5(srcQQ+SessionKey)_16
	pData += 16;
	// MsgType_2 ID_2 
	msgType = Swap2Byte(*((unsigned short*)pData));
	pData += 4;

	char sendtm[40];
	time_t tmSend = Swap4Byte(*((time_t*)pData));
	GetTimeString(tmSend, "%Y-%m-%d %H:%M:%S", sendtm, 40);
	
	// Sendtm_4 SendFace_2 unknow_4
	pData += 10;
	// ��Ƭ_1, Ƭ���_1 ��ϢID_2
	msgID = Swap4Byte(*((DWORD*)pData));
	if((msgID == 0) || (msgType != 0xB))
	{
		return ;
	}

	if (SearchSeq(s->m_SendmsgID, msgID))
	{
		return ;	// ����Ϣ�Ѵ���
	}

	pData += 4;
	// ��Ϣ��ʽ
	senType = pData[0];
	pData += 1;

	if(senType==1 || senType==2)
	{
		int pos = 0;
// 		pos+=sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� %u :\r\n",
// 			sendtm, s->m_sip, s->m_dip, dwSendQQ, dwRecvQQ);

		WriteQQChatHis(s, sendtm, dwRecvQQ, dwSendQQ);
		//msgbuf += pos;
		int freeData = Datalen - (pData - pQQData);
		int imlen = pos;
		char* tp1 = (char*)pData;
		char* tp2 = NULL;
		int len = strlen(tp1);

// 		 char *tp3 = NULL;
		// �˳�����
		while(1)
		{
			tp2 = strchr(tp1, '\x14');
			if (tp2==NULL)
			{
				if(*tp1 != 0x20 || len > 1)
				{
// 					tp3 = strchr(tp1, '\x15');
// 					if(tp3 != NULL)
// 						printf("pause]");

					if(len > freeData)
						break;
					memcpy(msgbuf+imlen, tp1, len);
					imlen += len;
					memcpy(msgbuf+imlen, "\r\n\r\n", 4);
					imlen += 4;
				}
				break;
			}
			if (tp2==tp1)
			{
				len -= 2;
				freeData -= 2;
			}
			else
			{
				int l = tp2-tp1;
				if(l > freeData)
					break;
				memcpy(msgbuf+imlen, tp1, l);
				imlen += l;
				//msgbuf += l;
				len -= l+2;
				freeData -= l+2;
			}
			tp1 = tp2+2;
		}
		if (imlen>pos)
		{
			msglen = imlen;
			s->m_from.Format("%u",dwSendQQ);
			s->m_to.Format("%u",dwRecvQQ);
#ifdef _DEBUG
			printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
		}
	}
	return ;
}

void OutQQSendSMSIM(
				OicqSession *s,
				unsigned char *pQQData, 
				const int Datalen,
				char* msgbuf,
				int& msglen)
{
	if(Datalen <= 32)
		return ;
	
	int pos = 0;
	pos = sprintf(msgbuf, "OICQ   %s �� %s",	s->m_sip, s->m_dip);

	int loop;
	DWORD smsNo;
	unsigned char *pData = pQQData;
	pData += 2;		// ��Ϣ���
	pData += 6;		// δ֪2 δ֪4

	char nicename[16];
	memset(nicename, 0, sizeof(nicename));
	memcpy(nicename, pData, 13);
	pos += sprintf(msgbuf+pos, "     %s �����ֻ�����:\r\n", nicename);

	pData += 13;	// �������ǳ�
	pData += 2;		// δ֪1 �������
	pData += 1;		// ����
	smsNo = *(DWORD*)pData;
	pData += 4;		// ���ͱ��
	pData += 1;		// δ֪1
	int recPhone = pData[0];
	pData += 1;		// �����ֻ���
	for(loop = 0; loop< recPhone; loop++)
	{
		if(recPhone == 1)
		{
			pos += sprintf(msgbuf+pos, "�����ֻ�:%s\t", pData);
		}
		else
		{
			pos += sprintf(msgbuf+pos, "�����ֻ�%d:%s\t", loop+1, pData);
		}
		pData += 18;	// �ֻ�n
		pData += 3;		// δ֪2, δ֪1
	}
	int recQQ = pData[0];
	pData += 1;		// ����QQ��
	for(loop = 0; loop < recQQ; loop++ )
	{
		if(recQQ == 1)
		{
			pos += sprintf(msgbuf+pos, "����QQ��:%d\t", *(DWORD*)pData);
		}
		else
		{
			pos += sprintf(msgbuf+pos, "����QQ��%d:%d\t", loop+1, *(DWORD*)pData);
		}
		pData += 4;		// QQn
	}
	pData += 1;		// δ֪1
	int smsLen = ntohs(*(unsigned short*)pData);
	pData += 2;		// ����
	if(smsLen == 0) 
	{
		pos += sprintf(msgbuf+pos, "\r\n���ű��:%d", smsNo);
	}
	else
	{
//		�������ͨ����Ϣ�������ƽ�����ֽ��������
//		�����Щ�ַ�����˸������Щ�ֽ�Ҫ��0x01������
//		���������Ϣ��һ������Ϣ��ֶ��ɵĲ��֣�������Ϣ�ֽ�����ǰ��Ҫ��һ�������ݣ��ⲿ��������
//		[��Ϣ��ŵ��ַ�����ʽ����1��ʼ] [0x2F] [����Ϣ�������ַ�����ʽ] [0x0A]
		pos += sprintf(msgbuf+pos, "\r\n��������: %s\r\n\r\n", CString((char*)pData, smsLen));
	}
	if(pos > 0)
	{
		msglen = pos;
#ifdef _DEBUG
		printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
	}

	return ;
}


CString ConvertIPToString(DWORD u_ip)//192.168.1.29
{
    CString sip;
	char str[20];
	memset(str,0,20);
	BYTE *tp;
	tp = (BYTE*)&u_ip;
	sprintf((char*)str, "%u.%u.%u.%u", *(tp+3), *(tp+2), *(tp+1), *tp);

    sip.Format("%s",str);
	return sip;
}


#define _DEBUG_UNIQE_
void OutTMChatIM(
				OicqSession *s,
				unsigned char *pQQData,
				const int Datalen,
				char* msgbuf, 
				int& msglen,
				DWORD cmdID)

{
	if(Datalen <= 0x28)
		return ;

	Search bmMSG;
	bmMSG.InitSearch("MSG");
	int iFind = bmMSG.find((char*)pQQData, Datalen);
	if(iFind == -1)
	{
		if(Datalen <= 0xAB)
			return ;
		if(cmdID == TM_CMD_RECV_IM) //ֻ���� 00 81
		{
			DWORD dwSendQQ, dwRecvQQ;
			dwSendQQ = ntohl(*((DWORD*)pQQData));
			dwRecvQQ = ntohl(*((DWORD*)(pQQData+4)));
			int offset=0;
			if(memcmp(pQQData,pQQData+0x27,8)==0)
			{
				offset=0x27;
			}
			else if(memcmp(pQQData,pQQData+0x21,8)==0)
			{
				offset=0x21;
			}
			else 
				return ;
			offset+=8+0x10;
			WORD imtype=*(WORD*)(pQQData+offset);
			if(imtype==0x8100)//QQ_NORMAL_IM_FILE_EX_REQUEST_UDP 00 81
			{
				offset+=0x2a;
				DWORD remoteip=htonl(*(DWORD*)(pQQData+offset));
				offset+=4;
				offset+=0x3a;
				DWORD remotesubip=htonl(*(DWORD*)(pQQData+offset));
				CString s_remoteip=ConvertIPToString(remoteip);
				CString s_remotesubip=ConvertIPToString(remotesubip);
				CString tmpstr;

				if(remoteip==remotesubip)
				{
					tmpstr.Format("%s",s_remoteip);
				}
				else 
					tmpstr.Format("%s(����IP:%s)",s_remoteip,s_remotesubip);
				s->m_mapQQFriendIP.SetAt(dwSendQQ, tmpstr);

			}
			
		}

		return ;
	}
#ifdef _DEBUG_UNIQE_
	BOOL bRepeat = FALSE;
#endif
	unsigned char *pData = pQQData;
	DWORD dwSendQQ, dwRecvQQ;
	DWORD msgID;
	if(cmdID == TM_CMD_SEND_IM)
	{
//		DWORD dwSendQQ, dwRecvQQ;
		dwSendQQ = Swap4Byte(*((DWORD*)pData));			// ������Ϣ��QQ
		dwRecvQQ = Swap4Byte(*((DWORD*)(pData+4)));		// ������Ϣ��QQ
	//	if(iFind > 12)
	//		msgID = *((DWORD*)(pData+iFind-12));
	//	else
	//		return ;		// ��ƫ��12Ϊ���, ����������
// 		if (SearchSeq(s->m_SendmsgID, msgID))
// 		{
// 			printf("SendIM Find Repeat %u\n", msgID);
// #ifdef _DEBUG_UNIQE_
// 			bRepeat = TRUE;
// #else
// 			return ;	// ����Ϣ�Ѵ���
// #endif
// 		}
	}
	else
	{
	// 	DWORD msgType, senType, msgID;
		dwSendQQ = ntohl(*((DWORD*)pData));
		dwRecvQQ = ntohl(*((DWORD*)(pData+4)));
	
// 		if(SearchSeq(s->m_RecvmsgID, msgID))
// 		{
// 			printf("RecvIM Find Repeat %u\n", msgID);
// 			// �ظ���
// #ifdef _DEBUG_UNIQE_
// 			bRepeat = TRUE;
// #else
// 			return ;	// ����Ϣ�Ѵ���
// #endif
// 		}
	}

/*


  put_int( buf, 0x4D534700 ); //"MSG"
  put_int( buf, 0x00000000 );
  put_int( buf, p->time_create );
  put_int( buf, (msg_id<<16)|msg_id );	//maybe a random interger
  put_int( buf, 0x00000000 );
  put_int( buf, 0x09008600 );
  char font_name[] = "�?�?";	//must be UTF8
  put_word( buf, strlen(font_name) );
  put_data( buf, (void*)font_name, strlen( font_name) );
  put_word( buf, 0x0000 );
  put_byte( buf, 0x01 );
  put_word( buf, len+3 );
  put_byte( buf, 1 );
  put_word( buf, len );
  //	put_word( buf, p->seqno );
  put_data( buf, (uchar*)msg, len );
	post_packet( qq, p, SESSION_KEY );

*/


	if(iFind != -1 && iFind < Datalen-0x28)
	{
		int msgoffset=0;
		char sendtm[40];
		time_t tmSend;
	
		pData += iFind; //ָ��msg
		ASSERT(stricmp((char*)pData, "MSG")==0);

		tmSend = Swap4Byte(*((time_t*)(pData+8)));
		GetTimeString(tmSend, "%Y-%m-%d %H:%M:%S", sendtm, 40);


		msgID = *((DWORD*)(pData+12)); //randomid

		if(cmdID == TM_CMD_SEND_IM)
		{
			if (SearchSeq(s->m_SendmsgID, msgID))
		 	{
				printf("SendIM Find Repeat %u\n", msgID);
			 //#ifdef _DEBUG_UNIQE_
				//		bRepeat = TRUE;
			 //#else
						return ;	// ����Ϣ�Ѵ���
			 // #endif
			}
		}
		else 
		{
			if (SearchSeq(s->m_RecvmsgID, msgID))
			{
				printf("recvIM Find Repeat %u\n", msgID);
				//#ifdef _DEBUG_UNIQE_
				//	bRepeat = TRUE;
			//	#else
					return ;	// ����Ϣ�Ѵ���
			//	#endif
			}
		}


		msgoffset+=0x4+0x4+0x4+0x4+0x4+0x4; //ָ�򳤶� word 

		WORD fontnamelen=htons(*(WORD*)(pData+msgoffset));
		msgoffset+=2+fontnamelen;
		msgoffset+=2+1+2+1;

		WORD  buflen=htons(*(WORD*)(pData+msgoffset));
		msgoffset+=2;
		//pData += 0x28;
		int freeData = Datalen-(iFind+msgoffset);
		
		if(buflen!=freeData)
		{
			return ;
		}

		pData+=msgoffset; //ָ����������

		char msgBody[1600];
		int offMsg = 0;
		for(int l=0;l < freeData; l++)
		{
			if(pData[l] == 0x14)
			{
				offMsg += sprintf(msgBody+offMsg, "[%d]", pData[l+1]);
				pData ++;
			}
			else
			{
				offMsg += sprintf(msgBody+offMsg, "%c", pData[l]);
			}
		}
		if(offMsg > 0)
		{
			CString strMsg = NetBytestoAnsi(msgBody, CP_ACP);
			if(cmdID == TM_CMD_SEND_IM)
			{
//				msglen = sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� %u :\r\n%s\r\n\r\n",
//					sendtm, s->m_sip, s->m_dip, dwSendQQ, dwRecvQQ, strMsg);
				msglen = sprintf(msgbuf, "%s",strMsg);

				s->m_from.Format("%u",dwSendQQ);
				s->m_to.Format("%u",dwRecvQQ);

			}
			else
			{

//				msglen = sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� %u:\r\n%s\r\n\r\n",
//					sendtm, s->m_sip, s->m_dip, dwRecvQQ, dwSendQQ, strMsg);
				msglen = sprintf(msgbuf, "%s",strMsg);
				s->m_from.Format("%u",dwSendQQ);
				s->m_to.Format("%u",dwRecvQQ);

			}
#ifdef _DEBUG
			printf("%08x   %s --> %s\r\n%s\r\n",msgID, s->m_from, s->m_to, CString(msgbuf, msglen));
#endif

#ifdef _DEBUG_UNIQE_
			if(bRepeat)
				msglen = 0;
#endif

		}
	}
}

void OutQQRecvChatIM(
				OicqSession *s,
				unsigned char *pQQData,
				const int Datalen,
				time_t tmGet,
				char* msgbuf,
				int& msglen)
{
	if(Datalen <= 70)
		return ;
	
	unsigned char *pData = pQQData;
	DWORD dwSendQQ, dwRecvQQ;
	DWORD msgType, senType, msgID;
	dwSendQQ = ntohl(*((DWORD*)pData));
	dwRecvQQ = ntohl(*((DWORD*)(pData+4)));
	
	ReceiveIMHeader rechdr;
	memcpy(&rechdr, pData, sizeof(rechdr));
	// srcQQ_4 dstQQ_4 totSeq_4 srcIP_4 srcPort_2 msgType_2
	pData += 20;

	char sendtm[40];
	time_t tmSend;

	char msgFrom[10];
	memset(msgFrom, 0, sizeof(msgFrom));

	int ii = ntohs(rechdr.type);
	switch(ntohs(rechdr.type)) {
	case 0x9:	// ����
	case 0xA:	// İ����
	case 0x84:	// ��չ�����Ժ��ѵ���Ϣ�������������ֻ�����Ϣ
		if(ntohs(rechdr.type) == 0xA)
			strcpy(msgFrom, "İ����");
		else
			strcpy(msgFrom, "����");

		pData += 10; // srcQQVer_2 srcQQ_4 dstQQ_4
		pData += 16; // md5(srcQQ+SessionKey)_16
		msgType = ntohs(*((unsigned short*)pData));
		pData += 2;	// ��Ϣ����, ��ͨ��Ϣ?ϵͳ��Ϣ?...
		pData += 2;	// ID_2
		tmSend = Swap4Byte(*((time_t*)pData));
		GetTimeString(tmSend, "%Y-%m-%d %H:%M:%S", sendtm, 40);
		pData += 4;	// ����ʱ�� 
		pData += 6;	// SendFace_2 unknow_4
		if(0x84 == ntohs(rechdr.type))
			pData += 8;		// unknow_8
		msgID = *((DWORD*)pData);
		pData += 4;	// ��Ƭ? ��ƬID ���2
		senType = pData[0];	
		pData += 1;	// ��Ϣ��ʽ
		if(	msgType != 0xB )
		//	return;		// ����ͨ��Ϣ
		break;
	case 0x52:	/// Ⱥ��Ϣ
		{
			bytebuffer pbuf;
			pbuf.pos = sizeof(rechdr);
			pbuf.len = Datalen;
			pbuf.size = Datalen;
			memcpy(pbuf.data, pQQData, Datalen);

			CString strTo;
			CQunINfo *pOb = NULL;
			if(FALSE == s->m_mapQun.Lookup(CStringEx::GetStringEx("%d", dwSendQQ), (CObject*&)pOb))
				return ;
			strTo = pOb->strQun;
			pbuf.pos += 9;	/// ʵ�ʷ�����
			dwSendQQ = get_int(&pbuf);
			if(dwSendQQ == dwRecvQQ)
				return ;	/// �Լ����͵���Ϣ
				
			Search bmMSG;
			bmMSG.InitSearch("MSG");
			int iFind = bmMSG.find((char*)pQQData, Datalen);
			if(iFind == -1)
				return ;

			//////////////////////////////////////////////////////////////////////////
			int msgoffset=0;
			char sendtm[40];
			time_t tmSend;
		 
			pQQData += iFind; //ָ��msg
			ASSERT(stricmp((char*)pQQData, "MSG")==0);


			tmSend = Swap4Byte(*((DWORD*)(pQQData+8)));
			GetTimeString(tmSend, "%Y-%m-%d %H:%M:%S", sendtm, 40);
			
			
			msgID = *((DWORD*)(pQQData+12)); //randomid
			
// 			if(cmdID == TM_CMD_SEND_IM)
// 			{
// 				if (SearchSeq(s->m_SendmsgID, msgID))
// 				{
// 					printf("SendIM Find Repeat %u\n", msgID);
// 					//#ifdef _DEBUG_UNIQE_
// 					//		bRepeat = TRUE;
// 					//#else
// 					return ;	// ����Ϣ�Ѵ���
// 					// #endif
// 				}
// 			}
// 			else 
// 			{
// 				if (SearchSeq(s->m_RecvmsgID, msgID))
// 				{
// 					printf("recvIM Find Repeat %u\n", msgID);
// 					//#ifdef _DEBUG_UNIQE_
// 					//	bRepeat = TRUE;
// 					//	#else
// 					return ;	// ����Ϣ�Ѵ���
// 					//	#endif
// 				}
// 			}
			
			
			msgoffset+=0x4+0x4+0x4+0x4+0x4+0x4; //ָ�򳤶� word 
			
			WORD fontnamelen=htons(*(WORD*)(pQQData+msgoffset));
			msgoffset+=2+fontnamelen;
			msgoffset+=2+1+2+1;
			
			WORD  buflen=htons(*(WORD*)(pQQData+msgoffset));
			msgoffset+=2;
			//pData += 0x28;
			int freeData = Datalen-(iFind+msgoffset);
			
			if(buflen!=freeData)
			{
				return ;
			}
			
			pQQData+=msgoffset; //ָ����������
			char msgBody[1600];
			int offMsg = 0;
			for(int l=0;l < freeData; l++)
			{
				if(pQQData[l] == 0x14)
				{
					offMsg += sprintf(msgBody+offMsg, "[%d]", pQQData[l+1]);
					pQQData ++;
				}
				else
				{
					offMsg += sprintf(msgBody+offMsg, "%c", pQQData[l]);
				}
			}
			if(offMsg > 0)
			{
				CString strMsg = NetBytestoAnsi(msgBody, CP_ACP);
			 	 
					
					//				msglen = sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� %u:\r\n%s\r\n\r\n",
					//					sendtm, s->m_sip, s->m_dip, dwRecvQQ, dwSendQQ, strMsg);
				msglen = sprintf(msgbuf, "%s",strMsg);
				s->m_from.Format("%u",dwSendQQ);
				 
				s->m_to.Format("%s",strTo);
				s->m_qqgoupid.Format("%s",strTo);
	#ifdef _DEBUG
				printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
	#endif

				if(v_programtype!=SYSTEM_SLEUTH)
				{
					char sendtmaa[40];
					GetTimeString(tmSend, "%Y%m%d%H%M%S", sendtmaa, 40);
					
					CString tmpbcp, ttmsg;
					ttmsg = strMsg;
					ttmsg.Replace("\t", " ");
					ttmsg.Replace("\r\n", "<br>");
					ttmsg.Replace("\r", "<br>");
					ttmsg.Replace("\n", "<br>");
					tmpbcp.Format("qqgroupchat\t%u\t%s\t%u\t%s",
						pOb->dwQNo, sendtmaa, dwSendQQ, ttmsg);
					WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, tmpbcp, NULL, NULL);
				}
			}
		}
		return ;		/// �������
// 	case 0x2A:	// ������/���˻Ự
	case 0x2B:	// �̶�Ⱥ
		if(s->m_qqver >= 0x1759)//QQ2009SP5)
		{
			bytebuffer pbuf;
			pbuf.pos = sizeof(rechdr);
			pbuf.len = Datalen;
			pbuf.size = Datalen;
			memcpy(pbuf.data, pQQData, Datalen);
			
			DWORD dw ;
			char tmp[MSG_CONTENT_LEN];

			dw = get_int(&pbuf);
			if(dw != 0)
				return ;
			dw = get_int(&pbuf);
			CString strTo;
			CQunINfo *pOb = NULL;
			if(FALSE == s->m_mapQun.Lookup(CStringEx::GetStringEx("%d", dwSendQQ), (CObject*&)pOb))
				return ;
			strTo = pOb->strQun;
			if(dw != atoi(strTo))
				return ;
			get_byte(&pbuf);
			dwSendQQ = get_int(&pbuf);
			if(dwSendQQ == dwRecvQQ)
				return ;	/// �Լ����͵���Ϣ
			get_word(&pbuf);
			get_word(&pbuf);	/// msg_id

			DWORD tmsend = get_int(&pbuf);		/// tm
			char sendtm[40];
			GetTimeString(tmsend, "%Y-%m-%d %H:%M:%S", sendtm, 40);

			pbuf.pos += 16;
			dw = get_string(&pbuf, tmp, MSG_CONTENT_LEN);
			if(dw > 2)
			{
// 				int pos =  _snprintf(msgbuf, 2048, "OICQ    %s    %s �� %s    %s �� Ⱥ:%s,QQ:%u :\r\n%s\r\n",
// 					sendtm, s->m_sip, s->m_dip, s->m_userid, strTo, dwSendQQ, tmp);
				int pos =  _snprintf(msgbuf, 2048, "%s",tmp);

				s->m_from.Format("%u",dwSendQQ);
				s->m_to.Format("%s",strTo);
				s->m_qqgoupid.Format("%s",strTo);
				msglen = pos;
#ifdef _DEBUG
				printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
			}	
			return ;
		}
		else
		{
			if(ntohs(rechdr.type) == 0x2B)
				strcpy(msgFrom, "�̶�Ⱥ��Ա");
			else
				strcpy(msgFrom, "�������Ա");

			pData += 5;	// Ⱥ�ⲿID+����
			if(ntohs(rechdr.type) == 0x2A)
				pData += 4;	// �������ڲ�ID
			
			dwSendQQ = ntohl(*((DWORD*)pData));
			pData += 4;	// ������QQ��
			pData += 4;	// δ֪2, ��Ϣ���2
			tmSend = Swap4Byte(*((time_t*)pData));
			GetTimeString(tmSend, "%Y-%m-%d %H:%M:%S", sendtm, 40);
			pData += 4;	// ����ʱ��
			pData += 4;	// VersionID(Ⱥ�汾, �иı伴+1)
			pData += 2; // �������ݳ���
			pData += 2; // ContentType 1���ı�2�б���
			msgID = *((DWORD*)pData);
			pData += 4; // ��Ƭ? ��ƬID ���2
			pData += 4; // δ֪4
		}
		break;
	case 0xC:	// ������ͨ�ֻ�����Ϣ
		GetTimeString(tmGet, "%Y-%m-%d %H:%M:%S", sendtm, 40);
		pData += 1;	// δ֪1�ֽ�
		break;
	case 0x14:	// �����ƶ�QQ�û����ֻ�����Ϣ(ʹ�õ����ֻ�����)
	case 0x13:	// �����ƶ�QQ�û����ֻ�����Ϣ
	case 0xB:	// ���԰��ֻ����ֻ�����Ϣ
	default:
		return ;
	}
	
	u_short aaa = ntohs(rechdr.type);
	
	if(ntohs(rechdr.type) == 0xC)
	{
		char chrPhone[20];
		memcpy(chrPhone, pData, 20);
		pData += 20;
		
		int pos = 0;
// 		pos+=sprintf(msgbuf, "OICQ    %s    %s �� %s    Phone:%s �� %u :\r\n%s\r\n\r\n",
// 			sendtm, s->m_sip, s->m_dip, chrPhone, dwRecvQQ, pData);
		pos+=sprintf(msgbuf, "%s",pData);

		s->m_from.Format("%s",chrPhone);
		s->m_to.Format("%u",dwRecvQQ);
		if(pos > 0)
		{
			msglen = pos;
#ifdef _DEBUG
			printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
		}
	}
	else
	{
		if(SearchSeq(s->m_SendmsgID, rechdr.sequence))
		{
			// �ظ���
			return ;
		}
		// 0x84�����Ϊ0�����. ��Ƭ�����, �����в����.
// 		if(msgID == 0)		
// 			msgID = rechdr.sequence;
// 		if( msgID > 0 && SearchSeq(s->m_SendmsgID, msgID))
// 			return ;	// ����Ϣ�Ѵ���
		
		int pos = 0;
		if (dwRecvQQ==dwSendQQ && rechdr.type>0x20)
		{
// 			pos+=sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� (%s):\r\n",
// 				sendtm, s->m_sip, s->m_dip, dwSendQQ, msgFrom);
			pos+=sprintf(msgbuf, "%s",msgFrom);

			s->m_from.Format("%u",dwSendQQ);
//			s->m_to.Format("%u",msgFrom);

		}
		else
		{
// 			pos+=sprintf(msgbuf, "OICQ    %s    %s �� %s    %u �� %u(%s):\r\n",
// 				sendtm, s->m_sip, s->m_dip, dwRecvQQ, dwSendQQ, msgFrom);
			pos+=sprintf(msgbuf, "%s",msgFrom);

			WriteQQChatHis(s, sendtm, dwRecvQQ, dwSendQQ);
			s->m_from.Format("%u",dwSendQQ);
			s->m_to.Format("%u",dwRecvQQ);

		}
		//msgbuf += pos;

		int freeData = Datalen - (pData - pQQData);
		int imlen = pos;
		char* tp1 = (char*)pData;
		char* tp2 = NULL;
		int len = strlen(tp1);
		// �˳�����

//		char *tp3 = NULL;
		while(1)
		{
			tp2 = strchr(tp1, 14);
			if (tp2==NULL)
			{
// 				tp3 = strchr(tp1, '\x15');
// 				if(tp3 != NULL)
// 					printf("pause]");

				if(len > freeData)
					break;
				memcpy(msgbuf+imlen, tp1, len);
				imlen += len;
				memcpy(msgbuf+imlen, "\r\n\r\n", 4);
				imlen += 4;
				break;
			}
			if (tp2==tp1)
			{
				len -= 2;
				freeData -= 2;
			}
			else
			{
				int l = tp2-tp1;
				if(l > freeData)
					break;

				memcpy(msgbuf+imlen, tp1, l);
				imlen += l;
				len -= l+2;
				freeData -= l+2;
			}
			tp1 = tp2+2;
		}
		if (imlen>pos)
		{
			msglen = imlen;
#ifdef _DEBUG
			printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
		}
	}


	return ;	
}

void Out0x126QQFirend(
			OicqSession *s,
			unsigned char *pQQData, 
			const int Datalen,
			char* msgbuf,
			int& msglen)
{
	/// 2010beta��2009, �����в�һ�����
// 	if(Datalen < 20)
// 		return ;
// 
// 	bytebuffer pbuf;
// 	pbuf.pos = 0;
// 	pbuf.len = Datalen;
// 	pbuf.size = Datalen;
// 	memcpy(pbuf.data, pQQData, Datalen);
// 
// 	int pos = 0;
// 
// 	DWORD tw;
// 	
// 	pbuf.pos += 10;
// 	unsigned short next_pos = get_word( &pbuf );
// 	pbuf.pos += 5;
// 	while( pbuf.pos < pbuf.len-5 )
// 	{	
// 		tw = get_int( &pbuf );
// // 		get_word( &pbuf );	// face
// // 		get_byte( &pbuf );		// age
// // 		get_byte( &pbuf );		// sex
// // 		tb = get_byte( &pbuf );
// // 		get_data(&pbuf, nicename, tb);	/// nicename
// // 		nicename[tb] = 0;
// 		pbuf.pos += 3;
// 		if(pos == 0)
// 		{
// 			pos = sprintf(msgbuf, "OICQ    %s �� %s    QQ(%s)��ú����б�:\r\n",
// 				s->m_sip, s->m_dip, s->m_userid);
// 		}
// 		pos += sprintf(msgbuf+pos, "QQ: %u\r\n", tw);
// 	}
// 	if( next_pos != 0xffff ){
//  		pos += sprintf(msgbuf+pos, "���и����Ա.\r\n");
// 	}
// 
// 	if(pos > 0)
// 	{
// 		msglen = pos;
// #ifdef _DEBUG
// 		printf("%s\n", CString(msgbuf, msglen));
// #endif
// 	}
}

void OutQQFirend(
			OicqSession *s,
			unsigned char *pQQData, 
			const int Datalen,
			char* msgbuf,
			int& msglen)
{
	if(Datalen < 6)
		return ;

	qq_friend_data qqfriend;

	int pos = 0;
	pos+=sprintf(msgbuf, "OICQ    %s �� %s    QQ(%s)��ú����б�:\r\n",
		s->m_sip, s->m_dip, s->m_userid);

	int offData = 2;
	unsigned char *pData = pQQData+2;
	while(pData != NULL && offData < Datalen)
	{
		memset(&qqfriend, 0, sizeof(qq_friend_data));
		memcpy(&qqfriend, pData, 9);
		offData += 9;
		pData += 9;
		memcpy(qqfriend.nicename, pData, qqfriend.lenNicename);
		offData += qqfriend.lenNicename;
		pData += qqfriend.lenNicename;
		memcpy(&qqfriend.flg, pData, 4);
		offData += 4;
		pData += 4;

		pos += sprintf(msgbuf+pos, "QQ: %d, �ǳ�:%s\r\n",
			Swap4Byte(qqfriend.qq_no), qqfriend.nicename);
	}
	pos += sprintf(msgbuf+pos, "\r\n");
	if(pos > 0)
	{
		msglen = pos;
#ifdef _DEBUG
		printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
	}

	return ;
}

//void OutQQOnLineFirend(
//			OicqSession *s, 
//			unsigned char *pQQData,
//			const int Datalen)
//{
//	FriendStatus qqfriend;
//	if(memcmp(pQQData, "\xFF", 1) == 0)
//	{
//		printf("You Have Get ALL Online Firend!\n");
//	}
//	int offData = 1;
//	unsigned char *pData = pQQData+1;
//	while(pData != NULL && offData < Datalen)
//	{
//		memset(&qqfriend, 0, sizeof(FriendStatus));
//		memcpy(&qqfriend, pData, sizeof(FriendStatus));
//		offData += sizeof(FriendStatus);
//		pData += sizeof(FriendStatus);
//
//		offData += 7;
//		pData += 7;
//		
////#define QQ_STATUS_HIDDEN	0x28	// ����
////#define QQ_STATUS_AWAY	0x1E	// �뿪
////#define QQ_STATUS_OFFLINE	0x14	// ����
////#define QQ_STATUS_ONLINE	0x0A	// ����
//		printf("QQ: %d Status:0x%X\n", ntohl(qqfriend.qq_no), qqfriend.status);
//	}
//}
//

void WriteQQChatHis(OicqSession *s, CString strtm, DWORD dwRecvQQ, DWORD dwSendQQ)
{
//  	HistoryRecord hisRec;
// 	CString strID;
// 	hisRec.SetRecord(IDX_RECTM,		strtm);
//  	hisRec.SetRecord(IDX_FORWTYPE,	"2");
// 	
// 	CString sipaddr, dipaddr;
// 	if(s->m_direction ==1)
//  	{
// 		sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 		dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 
// 		hisRec.SetRecord(IDX_SIP,		s->sip);
//  		hisRec.SetRecord(IDX_DIP,		s->dip);
//  		hisRec.SetRecord(IDX_SPORT,		s->sip.Right(5));
//  		hisRec.SetRecord(IDX_DPORT,		s->dip.Right(5));
//  		hisRec.SetRecord(IDX_SIPADDR,	sipaddr);
//  		hisRec.SetRecord(IDX_DIPADDR,	dipaddr);
//  		hisRec.SetRecord(IDX_PHONE,		s->Phone1);
//  		hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount1);
//  		hisRec.SetRecord(IDX_DPHONE,	s->Phone2);
//  		hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount2);
//  	}
//  	else
//  	{
// 		sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
// 		dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
// 
//  		hisRec.SetRecord(IDX_SIP,		s->dip);
//  		hisRec.SetRecord(IDX_DIP,		s->sip);
//  		hisRec.SetRecord(IDX_SPORT,		s->sip.Right(5));
//  		hisRec.SetRecord(IDX_DPORT,		s->dip.Right(5));
//  		hisRec.SetRecord(IDX_SIPADDR,	sipaddr);
//  		hisRec.SetRecord(IDX_DIPADDR,	dipaddr);
//  		hisRec.SetRecord(IDX_PHONE,		s->Phone2);
//  		hisRec.SetRecord(IDX_ACCOUNT,	s->UserAccount2);
//  		hisRec.SetRecord(IDX_DPHONE,	s->Phone1);
//  		hisRec.SetRecord(IDX_DACCOUNT,	s->UserAccount1);
//  	}
// 	hisRec.SetRecord(IDX_DATASRC,	s->m_dataSrc);
//  	hisRec.SetRecord(IDX_PRO,		"imsg_oicq");
// 	strID.Format("%u", dwSendQQ);
//  	hisRec.SetRecord(IDX_ID,		strID);
// 	strID.Format("%u", dwRecvQQ);
//  	hisRec.SetRecord(IDX_CONTENT,	strID);
//  
//  	hisRec.SetRecord(IDX_IMSGSTA, "����");
//  	if(s->m_ProxyType==1)
//  		hisRec.SetRecord(IDX_IFAGENT, "2");
//  	
// 	CString tempstr;
//  	if(hisRec.GetHisRecord(tempstr))
//  		WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, tempstr);
}
void OutQunSendIM( OicqSession *s, unsigned char *pQQData, const int Datalen, char* msgbuf, int& msglen)
{
	bytebuffer pbuf;
	pbuf.pos = 0;
	pbuf.len = Datalen;
	pbuf.size = Datalen;
	memcpy(pbuf.data, pQQData, Datalen);

	CString strTo, strMsg;
	CQunINfo *pOb = NULL;
	DWORD intQQ;
	unsigned char tb;
	DWORD tmsend;
	token tt;
	tb = get_byte(&pbuf);
	switch(tb)
	{
	case 0x2a:	/// ���Ⱥ��Ϣ
		intQQ = get_int(&pbuf);	/// �ڲ�Ⱥ��
		if(FALSE == s->m_mapQun.Lookup(CStringEx::GetStringEx("%d", intQQ), (CObject*&)pOb))
		{
#ifdef _DEBUG
			printf("fails ȺIDδת��Ϊ�ⲿID\r\n");
#endif
			return ;
		}
		strTo = pOb->strQun;

		Search bmMSG;
		bmMSG.InitSearch("MSG");
		int iFind = bmMSG.find((char*)pQQData, Datalen);
		if(iFind == -1)
			return ;

		/////////////////////////////////////////////////////
		pbuf.pos = iFind+8;	/// ֱ������MSGλ��
		tmsend = get_int(&pbuf);		/// tm
		char sendtm[40];
		GetTimeString(tmsend, "%Y-%m-%d %H:%M:%S", sendtm, 40);

		pbuf.pos += 12;
		get_token(&pbuf, &tt);
		pbuf.pos += 6;
		memset(tt.data, 0, sizeof(tt.data));
		get_token(&pbuf, &tt);
		ConvertUtf8toGbk((const char*)tt.data, tt.len, strMsg);
// 		int pos =  sprintf(msgbuf, "OICQ    %s    %s �� %s    %s �� Ⱥ:%s :\r\n%s\r\n",
// 			sendtm, s->m_sip, s->m_dip, s->m_userid, strTo, strMsg);
		int pos =  sprintf(msgbuf, "%s",strMsg);

		s->m_from.Format("%s",s->m_userid);
		s->m_to.Format("%s",strTo);
		s->m_qqgoupid.Format("%s",strTo);
		if(strMsg.GetLength() > 0)
		{
			msglen = pos;
#ifdef _DEBUG
			printf("%s --> %s\r\n%s\r\n", s->m_from, s->m_to, CString(msgbuf, msglen));
#endif
			//qqgroupchat\tȺ��\tʱ��(14λ)\t������ID\t��Ϣ�ı�\r\n
			if(v_programtype!=SYSTEM_SLEUTH)
			{
				char sendtmaa[40];
				GetTimeString(tmsend, "%Y%m%d%H%M%S", sendtmaa, 40);

				CString tmpbcp, ttmsg;
				ttmsg = strMsg;
				ttmsg.Replace("\t", " ");
				ttmsg.Replace("\r\n", "<br>");
				ttmsg.Replace("\r", "<br>");
				ttmsg.Replace("\n", "<br>");
				tmpbcp.Format("qqgroupchat\t%u\t%s\t%s\t%s",
					pOb->dwQNo, sendtmaa, s->m_userid, ttmsg);

				WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, tmpbcp, NULL, NULL);
			}
		}

		break;
	}

}

void OutQunInfo( OicqSession *s, unsigned char *pQQData, const int Datalen, char* msgbuf, int& msglen)
{
	bytebuffer pbuf;
	pbuf.pos = 0;
	pbuf.len = Datalen;
	pbuf.size = Datalen;
	memcpy(pbuf.data, pQQData, Datalen);

	DWORD extQQ, intQQ, ownQQ, maxNum;
	unsigned char qunName[64], qunAnn[256], qunIntro[256];
	CString strName, strAnn, strIntro, strQun;
	CQunINfo *pOb = NULL;

	int err = 0;
	try{

//	int off = 0;
#ifdef _DEBUG
	unsigned int last2;
#endif // _DEBUG
	int iia;
	unsigned int last_number;
	unsigned char more, status;
	unsigned char tb;
	token tt;
	CString resultbcp, tta;
	SYSTEMTIME st;
	CString tm, qunQQParter;

err =1 ;
	tb = get_byte(&pbuf);
	switch(tb)
	{
	case 0x72:	/// ���Ⱥ��Ϣ
err = 2;
		tb = get_byte(&pbuf);
		if(tb != 0)
			return ;
err = 3;
		intQQ = get_int(&pbuf);	/// �ڲ�Ⱥ��
		extQQ = get_int(&pbuf);	/// �ⲿȺ��(�ɲ���)
		get_word(&pbuf);
		get_byte(&pbuf);
		status = get_byte(&pbuf);
err =4 ;
		if( status == 3 )
		{	// Ⱥ��Ա�װ�
err = 5;
			get_byte( &pbuf );
			get_int( &pbuf );	//unknown
		//	get_int( buf );	//(???)unknown in 1205
			ownQQ = get_int( &pbuf );
			get_byte( &pbuf );
			pbuf.pos += 6;
			get_int( &pbuf );
			maxNum = get_word( &pbuf );
			pbuf.pos += 9;
			//name
			tb = get_byte( &pbuf );
			tb = min( 64-1, tb );
			get_data( &pbuf,  qunName, tb );
			qunName[tb] = 0;
err = 6;
			ConvertUtf8toGbk((const char*)qunName, tb, strName);
		//	DBG("qun: %s", q->name );
			get_byte( &pbuf );
			get_byte( &pbuf );	//separator
			//ann
			tb = get_byte( &pbuf );
			
			get_data( &pbuf,  qunAnn, tb );
			qunAnn[tb] = 0;
err = 7;
			ConvertUtf8toGbk((const char*)qunAnn, tb, strAnn);
			//intro
			tb = get_byte( &pbuf );
			get_data( &pbuf,  qunIntro, tb );
			qunIntro[tb] = 0;
			ConvertUtf8toGbk((const char*)qunIntro, tb, strIntro);
			//token data
			get_token( &pbuf, &tt );
// #ifdef _DEBUG
// 			printf("Ⱥ:%s\r\nȺ��:%u\r\n������:%u\r\n����: %s\r\n����: %s\r\n��Ա����:\r\n",
// 				strName, extQQ, ownQQ, strIntro, strAnn);
// #endif
err= 8;
			if(FALSE == s->m_mapQun.Lookup(CStringEx::GetStringEx("%d", intQQ), (CObject*&)pOb))
			{
err = 9;
				pOb = new CQunINfo;
				pOb->strQun.Format("%u(%s)", extQQ, strName);
				pOb->dwQNo = extQQ;
				pOb->ownQQ = ownQQ;
				pOb->lastQQ = 0;
				s->m_mapQun.SetAt(CStringEx::GetStringEx("%d", intQQ), (CObject*&)pOb);
			}
		}
		else
		{	// Ⱥ��Ա����
// #ifdef _DEBUG
// 			printf("Ⱥ��:%u\r\n�����Ա����:\r\n", extQQ);
// #endif
err = 10;
			if(s->m_mapQun.Lookup(CStringEx::GetStringEx("%d", intQQ), (CObject*&)pOb))
			{
err = 11;
				int a = pOb->strQun.Find('(');
				if(a > 0)
				{
					strName = pOb->strQun.Mid(a+1);
					strName.TrimRight(")");
				}
			}
			else
			{
				ASSERT(FALSE);
				WriteLog(IMSGPRO, "OutQunInfo srhMap intQQ %u no exist, status:%d, extQQ %u", intQQ, status, extQQ);
				return ;
			}
		}

err = 12;
		for (iia = 0; iia < 10 && pbuf.pos+6 < pbuf.len ; iia++)
		{
err = 13;
			if(memcmp(pbuf.data+pbuf.pos, pbuf.data+pbuf.len-6, 4) == 0)
				break;
			else
				tb = get_byte(&pbuf);	// ����һ�ֽ�, ����δ֪
		}

		last_number = get_int( &pbuf );	//member last came in
err = 14;
		if(pOb->lastQQ < last_number)
		{
err = 15;
			resultbcp.Empty();
			more = get_byte( &pbuf );	//more member data
			GetLocalTime(&st);
			tm.Format("%04u-%02u-%02u %02u:%02u:%02u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);	
			while( pbuf.pos < pbuf.len )
			{
err = 16;
				DWORD  qunQQ = get_int( &pbuf );
				tb = get_byte( &pbuf );
				tb = get_byte( &pbuf );
				if(qunQQ == pOb->ownQQ)
					qunQQParter = "������";
				else
					qunQQParter = ((tb&1)?"����Ա":"Ⱥ��Ա");
	// #ifdef _DEBUG
	// 			printf("%u %s,", qunQQ, ((tb&1)?"����Ա":"��Ա"));
	// #endif
				tta.Format("%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\t"
						  "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
						  "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
						  "qqgroup",
						  extQQ,//It->second->groupid,//Ⱥid
						  "imsg_oicq",
						  tm,
						  "",
						  "",
						  "",
						  "",
						  "",
						  qunQQ,//Ita->first,//��qq��
						  "",
						  "",
						  "",
						  "",
						  "",
						  "",
						  "",
						  "",
						  "",
						  "",
						  "fyy",
						  v_ddelist.spyaddr,
						  "",
						  "",
						  "",
						  strName,//It->second->groupname,//Ⱥ����
						  "",//ttb,//�ǳ�
						  qunQQParter//tta//QQ���
						  );
				resultbcp += tta;

	#ifdef _DEBUG
				last2 = qunQQ;
	#endif // _DEBUG
			}
			if(more)
			{
	// #ifdef _DEBUG
	// 			printf("Ⱥ�ڻ��и����Ա\r\n");
	// #endif
			}
			ASSERT(last_number == last2);

// 			if(FALSE == resultbcp.IsEmpty())
// 				WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, resultbcp);

err = 17;
			pOb->lastQQ = last_number;
			resultbcp.TrimRight();
			pOb->strQunNo.Add(resultbcp);
err = 18;
			s->m_mapQun.SetAt(CStringEx::GetStringEx("%d", intQQ), (CObject*&)pOb);
		}
		break;
	default:
		break;
	}

err = 19;
	if(FALSE == resultbcp.IsEmpty())//off > 0)
	{
// 		msglen = off;
#ifdef _DEBUG
		printf("out qqgroup %d bcp, len %d, lastqq %d\r\n", extQQ, resultbcp.GetLength(), last2);
#endif
	}

	}catch(...){
		WriteLog(IMSGPRO, "OutQunInfo Catch error, no:%d", err);
	}
}
//////////////////////////////////////////////////////////////////////////
// �����Ϣ��ں���
void OutMessage(OicqSession *s,	DWORD qqcmd,
				unsigned char *pQQData, const int Datalen,
				char* msgbuf,	int& msglen,
				BOOL bC2S, time_t tmPacket)
{
	int err = 0;
	try{

	switch(qqcmd)
	{
	case QQ_CMD_SEND_IM:
// 	case TM_CMD_SEND_IM:
		if(bC2S)
		{
err = 1;
			OutQQSendChatIM(s, pQQData, Datalen, msgbuf, msglen);
		}
		break;
	case TM_CMD_SEND_IM:			// TM������Ϣ
		if(bC2S)
		{
err = 2;
			OutTMChatIM(s, pQQData, Datalen, msgbuf, msglen, TM_CMD_SEND_IM);
		}
		break;
	case TM_CMD_RECV_IM:			// TM������Ϣ
		if(FALSE == bC2S)
		{
err = 3;
			OutTMChatIM(s, pQQData, Datalen, msgbuf, msglen, TM_CMD_RECV_IM);
		}
		break;
 	case QQ_CMD_RECV_IM:			// ������Ϣ
		if(FALSE == bC2S)
		{
err = 4;
			OutQQRecvChatIM(s, pQQData, Datalen, tmPacket, msgbuf, msglen);
		}
 		break;
	case QQ_CMD_SEND_SMS:			// ������
		if(bC2S)
		{
err = 5;
			OutQQSendSMSIM(s, pQQData, Datalen, msgbuf, msglen);
		}
		break;
	case QQ_CMD_GET_FRIENDS_LIST:	// ��ú����б�(ֻ���״�ʹ��qqʱ����)
		if(FALSE == bC2S)
		{
err = 6;
			OutQQFirend(s, pQQData, Datalen, msgbuf, msglen);
		}
		break;
	case QQ_CMD_GET_BUDDY_LIST:	// �����б�
		if(FALSE == bC2S)
		{
err = 7;
			Out0x126QQFirend(s, pQQData, Datalen, msgbuf, msglen);
		}
		break;
	case QQ_CMD_UPDATE_INFO:
	case QQ_CMD_GET_USER_INFO:
		if(FALSE == bC2S)
		{
err = 8;
			OutUpdateInfo(s, (BYTE*)pQQData, Datalen, qqcmd, msgbuf, msglen);
		}
		break;
	case QQ_CMD_QUN:
		if(FALSE == bC2S)
		{
err = 9;
			OutQunInfo(s, (BYTE*)pQQData, Datalen, msgbuf, msglen);
		}
		else
		{
err = 10;
			OutQunSendIM(s, (BYTE*)pQQData, Datalen, msgbuf, msglen);
		}
		break;
// 	case QQ_CMD_GET_FRIENDS_ONLINE:	// ������ߺ����б�
// 		break;
	}

	}catch(...){
		WriteLog(IMSGPRO, "OutMessage() Error!!=%d;", err);
	}
}


unsigned char get_byte( bytebuffer* p )
{
	unsigned char b = 0;
	if( p->pos<p->len ){
		b = p->data[p->pos++];
	}else{
//		DBG("packet p->pos(%d) >= p->len(%d)", p->pos, p->len );
	}
	return b;
}

unsigned short get_word( bytebuffer* p )
{
	unsigned short b = 0;
	if( p->pos+1<p->len ){
		b = *(unsigned short*)(&p->data[p->pos]); p->pos+=2;
	}else{
//		DBG("packet p->pos(%d)+1 >= p->len(%d)", p->pos, p->len );
	}
	return ntohs(b);
}

unsigned int get_int( bytebuffer* p )
{
	unsigned int b = 0;
	if( p->pos+3<p->len ){
		b = *(unsigned int*)(&p->data[p->pos]); p->pos+=4;\
	}else{ 
//		DBG("packet p->pos(%d)+3 >= p->len(%d)", p->pos, p->len );
	}
	return ntohl(b);
}

int get_string( bytebuffer* p, char* str, int len )
{
	int j = 0;
	while( p->pos<p->len && j+1<len ){
		str[j++] = p->data[p->pos++];
		if( str[j-1] == '\0' )
			return j;
	}
	str[j] = 0;
	return j;
}

int get_data( bytebuffer* p, unsigned char* data, int len )
{
	if( p->pos+len<=p->len ){
		memcpy( data, &p->data[p->pos], len );
		p->pos += len;
	}else{ 
//		DBG("packet p->pos(%d)+%d > p->len(%d)", p->pos, len, p->len );
		len = 0;
	}
	
	return len;
}

int get_token( bytebuffer* p, token* tok )
{
	unsigned short len = get_word( p );
	if (len > TOKEN_LEN)
	{
		tok->len = 0;
		return 0 ;
	}
	get_data( p, (unsigned char*)tok->data, len );
	tok->len = len;
	return len;
}


int get_token2( bytebuffer* p, token* tok )
{
	unsigned char len = get_byte( p );
	get_data( p, (unsigned char*)tok->data, len );
	tok->len = len;
	return len;
}
