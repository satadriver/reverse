#include "stdafx.h"
#include "mobileqq.h"
#include "../qq_crypt.h"
#include "imsg.h"
#include "..\\src\\http\\VoipUrl.h"
#include "..\\Include\\openssl\\md5.h"

//#define DEBUG
int GetQQKeyFromClient(char * data,int datalen, BOOL bc2s,mobileqq * mqq)
{
	qqpwd * myqqpwd = 0;
	int result = g_mobileqqpwdkeymap.Lookup(mqq->state.qqnum, myqqpwd);
	if (result == 0)
	{
		return FALSE;
	}
	unsigned char password[16] = {0};
	unsigned char firstmd5[32] = {0};
	memmove(firstmd5, myqqpwd->pwdmd5,16);
	memmove(password,myqqpwd->password,16);

	unsigned int qq = ntohl(mqq->state.qqnum);
	unsigned char key[32] = {0};
	unsigned char secondmd5[32] = {0};
	memmove(key,firstmd5,16);
	memmove(key + 16,"\x0\x0\x0\x0",4);
	memmove(key + 16 + 4, &qq,4);
	MD5(key,24,secondmd5);

	unsigned char decodebuf[4096];
	int decodebuflen = 4096;
	char rawkey[16] = {0};
	memmove(rawkey,"\x4d\xa0\xf6\x14\xfc\x9f\x29\xc2\x05\x4c\x77\x04\x8a\x65\x66\xd7",16);
	result = qq_decrypt((unsigned char*)data,datalen ,(unsigned char*)rawkey,decodebuf, &decodebuflen);		//03 02
	if (result == 0)
	{
		return FALSE;
	}

	char * pack1 = mystrstr("\x00\x00\x01\x06",4,(char*)decodebuf,decodebuflen);
	if (pack1 == 0)
	{
		return FALSE;
	}

	int pack1len = ntohs(*(unsigned short*)pack1);
	pack1 += 2;
	char pack1result[4096];
	int pack1resultlen = 4096;
	result = qq_decrypt((unsigned char*)pack1,pack1len,secondmd5,(unsigned char*)pack1result,&pack1resultlen);
 	if (result == 0)
	{
		return FALSE;
	}

	char * keytmp = mystrstr((char*)firstmd5,16,pack1result,pack1resultlen);
	if (keytmp == 0)
	{
		return FALSE;
	}
	memmove(mqq->state.mainkey, keytmp,16);		//md5 password

	char * pack2 = mystrstr("\x00\x01\x01\x44",4,(char*)decodebuf,decodebuflen);
	if (pack2 == 0)
	{
		return FALSE;
	}
	
	int pack2len = ntohs(*(unsigned short*)pack2);
	pack2 += 2;
	char pack2result[4096];
	int pack2resultlen = 4096;
	result = qq_decrypt((unsigned char*)pack2,pack2len,(unsigned char*)mqq->state.mainkey,(unsigned char*)pack2result,&pack2resultlen);
	if (result == 0)
	{
		return FALSE;
	}
	return TRUE;
}



int GetQQKeyFromServer(char * data,int datalen, BOOL bc2s,mobileqq * mqq)
{
	qqpwd * myqqpwd = 0;
	int result = g_mobileqqpwdkeymap.Lookup(mqq->state.qqnum, myqqpwd);
	if (result == 0)
	{
		return FALSE;
	}
	unsigned char password[16] = {0};
	unsigned char firstmd5[32] = {0};
	memmove(firstmd5, myqqpwd->pwdmd5,16);
	memmove(password,myqqpwd->password,16);

	unsigned int qq = ntohl(mqq->state.qqnum);
	unsigned char key[32] = {0};
	unsigned char secondmd5[32] = {0};
	memmove(key,firstmd5,16);
	memmove(key + 16,"\x0\x0\x0\x0",4);
	memmove(key + 16 + 4, &qq,4);
	MD5(key,24,secondmd5);

	unsigned char decodebuf[4096];
	int decodebuflen = 4096;
	char rawkey[16] = {0};
	
	memmove(rawkey,"\x4d\xa0\xf6\x14\xfc\x9f\x29\xc2\x05\x4c\x77\x04\x8a\x65\x66\xd7",16);
	result = qq_decrypt((unsigned char*)data + 16,datalen - 1 - 16 ,(unsigned char*)rawkey,decodebuf, &decodebuflen);		//03 02
	if (result == 0)
	{
		return FALSE;
	}
	
	char * pack1 = (char*)decodebuf;
	int pack1len = ntohs(*(unsigned short*)pack1);
	pack1 += pack1len;
	pack1len = ntohs(*(unsigned short*)(pack1 - 2));
	char pack1result[4096];
	int pack1resultlen = 4096;
	result = qq_decrypt((unsigned char*)pack1,pack1len,mqq->state.mainkey,(unsigned char*)pack1result,&pack1resultlen);
	if (result == 0)
	{
		return FALSE;
	}

	char * lastkey = mystrstr("\x03\x05\x00\x10",4,(char*)pack1result,pack1resultlen);
	if (lastkey == 0)
	{
		return FALSE;
	}
	
	memmove(myqqpwd->conststr, lastkey,16);
	g_mobileqqpwdkeymap.SetAt(mqq->state.qqnum, myqqpwd);
	memmove(mqq->state.sessionkey, lastkey,16);
	savepwdkey(mqq->state.qqnum, lastkey);
	return TRUE;
}





static char * mystrstr(char Flag[], int FlagLen, char Address[], int TotlLen)
{
	for (int Cnt = 0; Cnt < TotlLen - FlagLen + 1; Cnt ++)
	{
		if (memcmp( Flag,Address + Cnt, FlagLen) == 0)
		{
			return Address + Cnt + FlagLen;
		}
	}
	return FALSE;
}





BOOL getqqstr(char * pdata, int datalen,char * qq)
{
	int i = 0;
	for (i = 0; i < datalen - strlen(qq) + 1; i ++)
	{
		int n = 0;
		for (n = 0; n < 16; n ++)
		{
			if (!isdigit(*(pdata + i + n)) )
			{
				break;
			}
		}
		
		if (n >= 6 && n < NAME_LEN)
		{
			memmove(qq, pdata + i, n);
			return n;	
		}
	}
	return FALSE;
}


BOOL getcmdstr(char * pdata, int datalen,char * cmd)
{
	int i = 0;
	for (i = 0; i < datalen - strlen(cmd) + 1; i ++)
	{
		int n = 0;
		for (n = 0; n < 64; n ++)
		{
			if (!isalpha(*(pdata + i + n)) )
			{
				if (*(pdata + i + n) != '.' && *(pdata + i + n) != '_') 
				{
					break;
				}
			}
		}
		
		if (n >= 4 && n < NAME_LEN)
		{
			memmove(cmd, pdata + i, n);
			return n;	
		}
	}
	return FALSE;
}



mobileqq::mobileqq(DATEHEADER& packh, int aT, int flag): SocketSession(packh, aT)
{
	if (flag == MOBILEQQ_PROTOCOLIMSG /*|| MOBILEQQ_PROTOCOLHTTP*/)		
	{
		//clientbuf = new char[MOBILEQQ_BUFFER_SIZE];
		//serverbuf = new char[MOBILEQQ_BUFFER_SIZE];
	}
	else
	{
		//clientbuf = 0;
		//serverbuf = 0;
	}
	memset((CHAR*)&state,0,sizeof(MOBILEQQSTATE));

	//src = client dst = server,this use to check out the serverbuf and clientbuf
	if(packh.m_sport == 443 || packh.m_sport == 80 || packh.m_sport == 8080 || packh.m_sport == 14000)
	{
		srcip = *(unsigned int*)packh.m_dip;
		dstip = *(unsigned int*)packh.m_sip;
		srcport = packh.m_dport;
		dstport = packh.m_sport;	
	}
	else if (packh.m_dport == 443 || packh.m_dport == 80 || packh.m_dport == 8080 || packh.m_dport == 14000)
	{
		srcip = *(unsigned int*)packh.m_sip;
		dstip = *(unsigned int*)packh.m_dip;
		srcport = packh.m_sport;
		dstport = packh.m_dport;
	}
	memset(filterflag, 0, PACKET_FILTER_BUFFER_SIZE*sizeof(MobileQQFilter));
	filterflagptr = filterflag;
	clientbufptr = 0;
	serverbufptr = 0;
	state.onlineflag = 0;
}


mobileqq::~mobileqq()
{
	//delete []clientbuf;
	//delete []serverbuf;
}

void InitState(mobileqq * mqq)
{
	memset(mqq->state.from, 0, QQ_IMEI_IMSI_LEN);
	memset(mqq->state.to, 0, QQ_IMEI_IMSI_LEN);
	memset(mqq->state.groupname,0,NAME_LEN);
	memset(mqq->state.groupid,0,QQ_IMEI_IMSI_LEN);
	memmove(mqq->state.ostype,"安卓",strlen("安卓"));
	memset(mqq->state.msg,0,MSG_LEN);
}




BOOL mobileqq::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	this->processpacket(packh,pPacket,this);
	m_lastpacktm = packh.m_tm;
	return TRUE;
}




BOOL mobileqq::PacketFilter(DATEHEADER& packh,const char *pPacket)
{
	MobileQQFilter * InfoPtr = filterflag; 
	for( int Count = 0; Count < PACKET_FILTER_BUFFER_SIZE; Count ++ )
	{
		if( (InfoPtr->lastack == packh.m_ack) && (InfoPtr->lastseq == packh.m_sequence) && (InfoPtr->lastpacksize == packh.m_datalen) )
		{
			return FALSE;
		}
		InfoPtr ++ ;
	}

	if( (unsigned int)filterflagptr >= (unsigned int)filterflag + PACKET_FILTER_BUFFER_SIZE * sizeof(MobileQQFilter) )
	{
		filterflagptr = filterflag;
	}
	filterflagptr->lastack	= packh.m_ack;
	filterflagptr->lastseq	= packh.m_sequence;
	filterflagptr->lastpacksize	= packh.m_datalen;
	filterflagptr->unuse = 0;
	filterflagptr ++;
	return TRUE;
}






BOOL mobileqq::processpacket(DATEHEADER& packh, const char * pPacket,mobileqq * mqq)
{
	try
	{
		if (packh.m_datalen <= 0 || packh.m_datalen >= MAX_TRANSPORT_UNIT)
		{
			return FALSE;
		}

		int result = PacketFilter(packh,pPacket);
		if (result == FALSE)
		{
			return FALSE;
		}

		if (mqq->srcip == *(unsigned int*)packh.m_sip && mqq->dstip == *(unsigned int*)packh.m_dip && 
			mqq->srcport == packh.m_sport && mqq->dstport == packh.m_dport)
		{
			if (packh.m_datalen + mqq->clientbufptr > MOBILEQQ_CLIENT_BUFFER_SIZE)
			{
				mqq->clientbufptr = 0;
				return FALSE;
			}
			memmove(mqq->clientbuf + mqq->clientbufptr, pPacket, packh.m_datalen);
			mqq->clientbufptr += packh.m_datalen;
			
			packh.m_datalen = mqq->clientbufptr;
			pPacket = mqq->clientbuf;
			int bDecodeSuc = ParseQQPacketLen(packh,pPacket);
			if (bDecodeSuc ==0 )
			{
				return FALSE;		//just to return to wait for next packet
			}
			mqq->clientbufptr = 0;
			bDecodeSuc = analyse(packh, pPacket,mqq);
		}
		else if(mqq->srcip == *(unsigned int*)packh.m_dip && mqq->dstip == *(unsigned int*)packh.m_sip && 
			mqq->srcport == packh.m_dport && mqq->dstport == packh.m_sport)
		{		
			if ( (packh.m_datalen + mqq->serverbufptr) > MOBILEQQ_SERVER_BUFFER_SIZE)
			{
				mqq->serverbufptr = 0;	//clear buffer pointer to 0
				return FALSE;
			}
			memmove(mqq->serverbuf + mqq->serverbufptr, pPacket, packh.m_datalen);
			mqq->serverbufptr += packh.m_datalen;

			packh.m_datalen = mqq->serverbufptr;
			pPacket = mqq->serverbuf;
			int bDecodeSuc = ParseQQPacketLen(packh,pPacket);
			if (bDecodeSuc == 0)
			{
				return FALSE;		//just to return to wait for next packet
			}
			mqq->serverbufptr = 0;
			bDecodeSuc = analyse(packh, pPacket,mqq);
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO,"mobileqq processpacket error\r\n");
		return FALSE;
	}
}








BOOL analyse(DATEHEADER& packh, const char * Packet,mobileqq * mqq)
{
	try
	{
		int bRet = 0;
		BOOL bc2s  = FALSE;
		if(packh.m_dport==80 || packh.m_dport==443 || packh.m_dport==8080 || packh.m_dport==14000)
		{
			bc2s = TRUE;
		}
		else
		{
			bc2s = FALSE;
		}

		BYTE decodebuf[MOBILEQQ_SERVER_BUFFER_SIZE];			
		int decodelen = MOBILEQQ_SERVER_BUFFER_SIZE;
		const char * pPacket = Packet;
		while (pPacket < Packet + packh.m_datalen)
		{
			int iversion = ntohl(*(unsigned int*)(pPacket + 4));
			int TotalLen = ntohl(*(unsigned int*)pPacket);	

			if (TotalLen <= 0)
			{
				return FALSE;
			}
			else if (TotalLen >= MOBILEQQ_CLIENT_BUFFER_SIZE && bc2s)
			{
				return FALSE;
			}
			else if ((TotalLen >= MOBILEQQ_SERVER_BUFFER_SIZE) && (bc2s == FALSE))
			{
				return FALSE;
			}

			int offset = 8;
			BYTE crypttype = pPacket[offset];
			offset += 1;

			int tmplen = ntohl(*(unsigned int*)(pPacket+offset));	
			offset += (tmplen + 4);
			if (offset >= TotalLen || offset <= 0)
			{
				//this value of some version = 0x0b packet is very large,do not know how to processi it
				//need to check it ok!
				return FALSE;
			}

			tmplen = *(pPacket + offset)  - 4;	
			offset += 1;
			
			if (offset >= TotalLen || offset <= 0)
			{
				return FALSE;
			}
			if (tmplen >=6 && tmplen <= 12)
			{
				memset(mqq->state.qq,0,QQ_IMEI_IMSI_LEN);
				memmove(mqq->state.qq, pPacket + offset, tmplen);
				mqq->state.qqnum = strtoul(mqq->state.qq, 0,10);
			}
			offset += tmplen;
			
			unsigned char key[16];
			memset((char*)key, 0, 16);	//fix_crypt will use 16 0 as key
			if(crypttype==MOBILE_QQ_CRYPT_AUTO)
			{
				//31 48 ff 44
				/*
				char * fn = "mobile_qq_pwdkey.mqqkey";
				FILE * fp = fopen(fn,"a");
				if (fp == 0)
				{
					fp = fopen(fn,"w");
					return FALSE;
				}

				qqpwd stqqpwd;
				memset((char*)&stqqpwd,0,sizeof(qqpwd));
				stqqpwd.qq = 2210853762;
				memmove(stqqpwd.password , "sata19821117",strlen("sata19821117"));
				MD5((const unsigned char*)stqqpwd.password,strlen((char*)stqqpwd.password),stqqpwd.pwdmd5);
				int cnt = fwrite((char*)&stqqpwd, 1, sizeof(qqpwd), fp);
				if (cnt != sizeof(qqpwd))
				{
					return FALSE;
				}

				memset((char*)&stqqpwd,0,sizeof(qqpwd));
				stqqpwd.qq = 1957001146;
				memmove(stqqpwd.password , "sata19820521",strlen("sata19820521"));
				MD5((const unsigned char*)stqqpwd.password,strlen((char*)stqqpwd.password),stqqpwd.pwdmd5);
				cnt = fwrite((char*)&stqqpwd, 1, sizeof(qqpwd), fp);
				if (cnt != sizeof(qqpwd))
				{
					return FALSE;
				}

				memset((char*)&stqqpwd,0,sizeof(qqpwd));
				stqqpwd.qq = 1307017307;
				memmove(stqqpwd.password , "sata19820521",strlen("sata19820521"));
				MD5((const unsigned char*)stqqpwd.password,strlen((char*)stqqpwd.password),stqqpwd.pwdmd5);
				cnt = fwrite((char*)&stqqpwd, 1, sizeof(qqpwd), fp);
				if (cnt != sizeof(qqpwd))
				{
					return FALSE;
				}
				fclose(fp);
				*/
				

				qqpwd * myqqpwd = 0;
				if (memcmp(mqq->state.version,"A4.7.0",6) >= 0)
				{
					int iret = g_mobileqqpwdkeymap.Lookup(mqq->state.qqnum, myqqpwd);
					if (iret == 0)
					{
						return FALSE;
					}
					memmove(key, myqqpwd->conststr,16);
				}
				else
				{
					mqq->state.qqnum = strtoul(mqq->state.qq,0,10);	
					int iret = g_mobileqqkeymap.Lookup(mqq->state.qqnum,*(unsigned int*)mqq->state.key);
					if (iret == 0)
					{
						return FALSE;
					}
					memmove(key, (char*)mqq->state.key,4);
				}
			}
			else if(crypttype==MOBILE_QQ_CRYPT_FIXED)
			{
				//F0441F5FF42DA58FDCF7949ABA62D411
				//{ -16, 68, 31, 95, -12, 45, -91, -113, -36, -9, -108, -102, -70, 98, -44, 17 }
				//%4;7t>;28<fc.5*6
				//4eY#X@~g.+U)2%$<
				//9u23fh$jkf^%43hj	
			}
			else
			{
				//heartbeat
				return FALSE;
			}


#ifdef DEBUG
			if (memcmp(mqq->state.qq,"2210853762",10) == 0 && crypttype == 1)
			{
				//memmove(key,"\x31\x48\xff\x44",4);
			}
#endif

			bRet = qq_decrypt((unsigned char*)pPacket+offset, TotalLen-offset, key, decodebuf, &decodelen);	
			if(bRet)
			{		
#ifdef DEBUG
				char tmp[MOBILEQQ_SERVER_BUFFER_SIZE];
				int tmplen = decodelen;
				memmove(tmp,decodebuf,tmplen);
				tmplen = Utf8ToGB2312((char*)tmp,tmplen);
				char * fn = "myTestlogQQ.dat";
				FILE * fp = fopen(fn,"a");
				if (fp == 0)
				{
					fp = fopen(fn,"w");
					return FALSE;
				}
				int cnt = fwrite(tmp, 1, tmplen, fp);
				if (cnt != tmplen)
				{
					return FALSE;
				}
				fclose(fp);
#endif				

				if (decodelen <= 32)
				{
					return FALSE;
				}
				BYTE * phead = decodebuf;
				int headlen = ntohl(*(unsigned int*)phead);
				if (headlen >= MOBILEQQ_SERVER_BUFFER_SIZE || headlen <= 0)
				{
					return FALSE;
				}
				BYTE * pbody = phead + headlen;
				int bodylen = ntohl(*(unsigned int*)pbody);	
				if (bodylen >= MOBILEQQ_SERVER_BUFFER_SIZE || bodylen <= 0)
				{
					return FALSE;
				}
				if (decodelen != headlen + bodylen)
				{
					//some packet is in multiparts when after crypted 
					//return FALSE;
				}
				int iRet = head(phead, headlen, pbody,bodylen,bc2s,mqq,decodelen);
				if(iRet)		//caution the return value in head function!
				{
					InitState(mqq);
					switch (mqq->state.type)
					{
					case MOBILE_QQ_AUTHKEY:
						bRet = getauthkey(pbody,bodylen,bc2s,mqq);
						break;
					case MOBILE_QQ_ONLINE:
						bRet = online(pbody, bodylen, bc2s,mqq);
						break;
					case MOBILE_QQ_OFFLINE:
						break;
					case MOBILE_QQ_ONLINEPUSH_REQPUSH:
						bRet = onlinereqpush(pbody, bodylen,mqq);
						break;
					case MOBILE_QQ_PBPUSHGROUPMSG:
						bRet = pbpushgroupmsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_PBGETGROUPMSG:
						bRet = pbgetgroupmsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_OFFLINEMSG:
						bRet = offlinemsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_PUSH_GROUP_MSG:
						bRet = pushgroupmsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_GET_GROUP_MSG:
						bRet = getgroupmsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_SEND_GROUP_MSG:
						bRet = sendgroupmsg(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_LIST_FRIEND:
						bRet = friendlist(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_ONLINE_FRIEND:
						bRet = onlinefriend(pbody, bodylen, mqq);
						break;
					case MOBILE_QQ_LIST_GROUPINFO:
						bRet = groupinfo(pbody, bodylen, mqq); 
						break;
					case MOBILE_QQ_TROOPLIST:
						bRet = trooplist(pbody,bodylen,mqq);
						break;
					case MOBILE_QQ_PUSHGETMSG:
						bRet = pushgetfriendmsg(pbody, bodylen, mqq); 
						break;
					case MOBILE_QQ_PUSHGETMSGV2:
						bRet = pushgetfriendmsgv2(pbody, bodylen, mqq); 
						break;
					case MOBILE_QQ_CONTACT:
						bRet = contact(pbody, bodylen, mqq); 
						break;
					case MOBILE_QQ_RECOMMENDLIST:
						bRet = recommendlist(pbody,bodylen,mqq);
						break;
					case MOBILE_QQ_PBSENDMSG:
						bRet = pbsendmsg(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_PBGETMSG:
						bRet = pbgetmsg(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_SIGNATURE:
						bRet = signature(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_BINDSTATE:
						bRet = bindstate(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_DISCUSSINFO:
						bRet = discussinfo(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_RICHSIG:
						bRet = richsig(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_GETROAMMSG:
						bRet = getroammsg(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_GETTROOP:
						bRet = gettroop(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_PBPRXYSVC:
						bRet = pbprxysvc(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_CHKUPDATE:
						bRet = chkupdate(pbody,bodylen, mqq);
						break;
					case MOBILE_QQ_SIMPLEINFO:
						bRet = simpleinfo(pbody,bodylen,mqq);
						break;
					case MOBILE_QQ_GETSET:
						bRet = getset(pbody,bodylen,mqq);
						break;	
					default:
						bRet = FALSE;
						break;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}

			pPacket += TotalLen;
		}

		return bRet;
	}
	catch (...)
	{
		WriteLog(IMSGPRO,"mobileqq analyse error\r\n");
		return FALSE;
	}
}




//false = error
BOOL head(BYTE* phead, int headlen,BYTE * pbody,int bodylen,BOOL bc2s,mobileqq * mqq,int decryptlen)
{
	try
	{
		int offset=0;
		if(bc2s)
		{
			offset=0x1C;
		}
		else
		{
			offset=0x0C;
		}

		BYTE * srchead = phead;
		while (phead < srchead + decryptlen)
		{

			headlen = ntohl(*(unsigned int*)phead);
			pbody = phead + headlen;
			bodylen = ntohl(*(unsigned int*)pbody);	

			int tmplen = ntohl(*(unsigned int*)(phead + offset));
			offset+=tmplen;
			if (offset > headlen || offset <= 0)
			{
				return FALSE;
			}

			int cmdlen = ntohl(*(unsigned int*)(phead + offset)) - 4;
			offset += 4;
			//char * cmdstr = (char*)pData + offset;
			char cmdstr[NAME_LEN] = {0};
			int ret = getcmdstr((char*)phead,headlen,cmdstr);
			if (ret == 0)
			{
				return FALSE;
			}
			
			if(memcmp(cmdstr, "login.auth",strlen("login.auth")) == 0)
			{
				mqq->state.type = MOBILE_QQ_AUTHKEY;
				if (bc2s)
				{
					offset += cmdlen;
					if (offset >= headlen)
					{
						return FALSE;
					}
					tmplen = ntohl(*(unsigned int*)(phead + offset));
					offset += tmplen;
					if (offset >= headlen)
					{
						return FALSE;
					}
					cmdlen = ntohl(*(unsigned int*)(phead + offset)) - 4;
					offset += 4;
					if (offset >= headlen || cmdlen > QQ_IMEI_IMSI_LEN)
					{
						return FALSE;
					}
					memmove(mqq->state.imei, phead + offset, cmdlen);
				}
			}
			else if (memcmp(cmdstr,"wtlogin.login",strlen("wtlogin.login")) == 0 ||		//wtlogin.name2uin wtlogin.login
					memcmp(cmdstr,"wtlogin.name2uin",strlen("wtlogin.name2uin")) == 0 ||	
					memcmp(cmdstr, "wtlogin.trans_emp",strlen("wtlogin.trans_emp"))==0 ||
					memcmp(cmdstr, "wtlogin.exchange_emp",strlen("wtlogin.exchange_emp"))==0 )
			{
				
				mqq->state.type = 0;
				offset += cmdlen;
				if (offset >= headlen)
				{
					return FALSE;
				}

				if (bc2s)
				{
					mqq->state.type = MOBILE_QQ_ONLINE;//why put this in the bs2s?

					tmplen = ntohl(*(unsigned int*)(phead + offset));
					offset += tmplen;
					if (offset >= headlen)
					{
						return FALSE;
					}

					cmdlen = ntohl(*(unsigned int*)(phead + offset)) - 4;
					if (cmdlen != 15 && cmdlen != 14)		//IMEI IMSI 14 OR 15
					{
						return FALSE;
					}
					offset += 4;
					memmove(mqq->state.imei, phead + offset, cmdlen);

					offset += cmdlen;
					if (offset >= headlen)
					{
						return FALSE;
					}
					tmplen = ntohl(*(unsigned int*)(phead + offset));
					offset += tmplen;
					if (offset >= headlen)
					{
						return FALSE;
					}

					tmplen = ntohs(*(unsigned short*)(phead + offset)) - 2;
					offset += 2;
					if ( *(phead + offset) == '|')
					{
						char * ender = mystrstr("|",1,(char*)phead + offset + 1,16);
						if (ender )
						{
							ender --;
							memmove(mqq->state.imsi, phead + offset + 1, ender - ((char*)phead + offset + 1) );

							ender +=1;
							if (memcmp(ender,"A4.7.0",6) >= 0)		//版本大于4.7.0
							{
								memmove(mqq->state.version,ender,6);
							}
						}
					}
					offset += tmplen;
				}
					
				char * mydata = (char*)pbody;
				mydata += 4;
				int mydatalen = bodylen - 4;
				if (*mydata != 02 || *(mydata + mydatalen - 1) != 03)		//02 totalpacketlen version cmd num qq data 03
				{
					return TRUE;	//why here should nbe true?
				}
				char * qq = mystrstr("\x08\x10\x00\x01",4, (char*)pbody,bodylen);
				if (qq == 0)
				{
					return TRUE;	//why here should nbe true?
				}
				mqq->state.qqnum = ntohl(*(unsigned int*)qq);
				ultoa(mqq->state.qqnum,mqq->state.qq,10);
		
				if (bc2s)
				{
					mydata = mystrstr("\x11\x29\x2a\xc1\xf5\x3d\x71\x28",8,(char*)pbody,256);
					if (mydata == 0)
					{
						return TRUE;	//why here should nbe true?
					}
					mydatalen = bodylen - (mydata - (char*)pbody) - 1;
					//GetQQKeyFromClient(mydata,mydatalen,bc2s,mqq);
				}
				else
				{
					//GetQQKeyFromServer((char*)mydata ,mydatalen,bc2s,mqq);
				}
			}
			else if (memcmp(cmdstr,"PhSigLcId.Check",strlen("PhSigLcId.Check")) == 0 ||
					memcmp(cmdstr, "ConfigService.ClientReq",strlen("ConfigService.ClientReq")) == 0 ||
					memcmp(cmdstr, "GrayUinPro.Check",strlen("GrayUinPro.Check"))==0 ||
					memcmp(cmdstr, "KQQConfig.SignatureReq",strlen("KQQConfig.SignatureReq"))==0 ||
					memcmp(cmdstr, "CliLogSvc.UploadReq",strlen("CliLogSvc.UploadReq"))==0 )
			{
				offset += cmdlen;
				if (offset >= headlen)
				{
					return FALSE;
				}
				tmplen = ntohl(*(unsigned int*)(phead + offset));
				offset += tmplen;
				if (offset >= headlen)
				{
					return FALSE;
				}	

				tmplen = ntohl(*(unsigned int*)(phead + offset)) - 4;
				if (tmplen != 14 && tmplen != 15)
				{
					return FALSE;
				}
				offset += 4;
				memmove(mqq->state.imei, phead + offset, tmplen);

				offset += tmplen;
				if (offset >= headlen)
				{
					return FALSE;
				}
				
				tmplen = ntohl(*(unsigned int*)(phead + offset)) ;
				offset += tmplen;
				if (offset >= headlen)
				{
					return FALSE;
				}

				tmplen = ntohs(*(unsigned short*)(phead + offset)) - 2;
				offset += 2;
				if (*(phead + offset) == '|')
				{
					offset += 1;
					char * ender = strstr((char*)(phead + offset),"|");
					if (ender == 0)
					{
						return FALSE;
					}
					memmove(mqq->state.imsi, phead + offset,  ender - (char*)(phead + offset));

					ender ++;
					memmove(mqq->state.version,ender,6);

					mqq->state.type = MOBILE_QQ_ONLINE;
				}
			}
			else if (memcmp(cmdstr, "login.chguin",strlen("login.chguin"))==0)
			{
				mqq->state.type = MOBILE_QQ_ONLINE;
				if (bc2s)
				{
					offset += cmdlen;
					if (offset >= headlen)
					{
						return FALSE;
					}
					tmplen = ntohl(*(unsigned int*)(phead + offset));
					offset += tmplen;
					if (offset >= headlen)
					{
						return FALSE;
					}
					cmdlen = ntohl(*(unsigned int*)(phead + offset)) - 4;
					offset += 4;
					if (offset >= headlen || cmdlen > QQ_IMEI_IMSI_LEN)
					{
						return FALSE;
					}
					memmove(mqq->state.imei, phead + offset, cmdlen);
				}
			}
			else if(memcmp(cmdstr, "OnlinePush.ReqPush",strlen("OnlinePush.ReqPush"))==0)
			{
				mqq->state.type = MOBILE_QQ_ONLINEPUSH_REQPUSH;
			}
			else if(memcmp(cmdstr, "MessageSvc.offlinemsg",strlen("MessageSvc.offlinemsg"))==0)
			{
				mqq->state.type = MOBILE_QQ_OFFLINEMSG;
			}
			else if(memcmp(cmdstr, "MessageSvc.GetMsgV",strlen("MessageSvc.GetMsgV"))==0 ||			//带有高版本的在前面，否则进入处理程序出错
				memcmp(cmdstr, "MessageSvc.PushMsgV",strlen("MessageSvc.PushMsgV"))==0 ||
				memcmp(cmdstr, "MessageSvc.SendMsgV",strlen("MessageSvc.SendMsgV"))==0)
			{
				mqq->state.type = MOBILE_QQ_PUSHGETMSGV2;
			}
			else if(memcmp(cmdstr, "MessageSvc.GetMsg",strlen("MessageSvc.GetMsg"))==0 || 
				memcmp(cmdstr, "MessageSvc.PushMsg",strlen("MessageSvc.PushMsg"))==0 ||
				memcmp(cmdstr, "MessageSvc.SendMsg",strlen("MessageSvc.SendMsg"))==0)
			{
				mqq->state.type = MOBILE_QQ_PUSHGETMSG;
			}
			else if(memcmp(cmdstr, "MessageSvc.GetGroupMsg",strlen("MessageSvc.GetGroupMsg"))==0 ||
					memcmp(cmdstr, "MessageSvc.PullGroupMsg",strlen("MessageSvc.PullGroupMsg"))==0)
			{
				mqq->state.type = MOBILE_QQ_GET_GROUP_MSG;
			}
			else if(memcmp(cmdstr, "MessageSvc.SendGroupMsg",strlen("MessageSvc.SendGroupMsg"))==0)
			{
				mqq->state.type = MOBILE_QQ_SEND_GROUP_MSG;
			}
			else if(memcmp(cmdstr, "MessageSvc.PushGroupMsg",strlen("MessageSvc.PushGroupMsg"))==0)
			{
				mqq->state.type = MOBILE_QQ_PUSH_GROUP_MSG;
			}
			else if(memcmp(cmdstr, "friendlist.getFriendGroupList",strlen("friendlist.getFriendGroupList"))==0 )
			{
				mqq->state.type = MOBILE_QQ_LIST_FRIEND;
			}
			else if(memcmp(cmdstr, "friendlist.getOnlineFriend",strlen("friendlist.getOnlineFriend"))==0 )
			{
				mqq->state.type = MOBILE_QQ_ONLINE_FRIEND;
			}
			else if(memcmp(cmdstr, "ProfileService.getGroupInfoReq",strlen("ProfileService.getGroupInfoReq"))==0 ||		//这2个命令还没有写代码
					memcmp(cmdstr, "friendlist.GetMultiTroopInfoReq",strlen("friendlist.GetMultiTroopInfoReq"))==0 ||
					memcmp(cmdstr, "friendlist.getMultiTroopInfoReq",strlen("friendlist.getMultiTroopInfoReq"))==0 )
			{
				mqq->state.type = MOBILE_QQ_LIST_GROUPINFO;
			}
			else if(memcmp(cmdstr, "friendlist.GetTroopList",strlen("friendlist.GetTroopList"))==0 ||
					memcmp(cmdstr, "friendlist.getTroopList",strlen("friendlist.getTroopList"))==0 )
			{
				mqq->state.type = MOBILE_QQ_TROOPLIST;
			}
			else if(memcmp(cmdstr, "QQServiceDiscussSvc.ReqGetDiscussInfo",strlen("QQServiceDiscussSvc.ReqGetDiscussInfo"))==0 ||
				memcmp(cmdstr, "QQService.DiscussRespHeader",strlen("QQService.DiscussRespHeader"))==0 )
			{
				mqq->state.type = MOBILE_QQ_DISCUSSINFO;
			}
			else if(memcmp(cmdstr, "account.RequestQueryQQBindingStat",strlen("account.RequestQueryQQBindingStat"))==0 )
			{
				mqq->state.type = MOBILE_QQ_BINDSTATE;
			}
			else if(memcmp(cmdstr, "account.RequestUploadAddressBook",strlen("account.RequestUploadAddressBook"))==0 ||
				memcmp(cmdstr, "account.RequestUpdateAddressBook",strlen("account.RequestUpdateAddressBook"))==0)
			{
				mqq->state.type = MOBILE_QQ_CONTACT;
			}
			else if ( memcmp(cmdstr, "account.RequestGetRecommendedList",strlen("account.RequestGetRecommendedList"))==0 )
			{
				mqq->state.type = MOBILE_QQ_RECOMMENDLIST;
			}
			else if (memcmp(cmdstr,"MessageSvc.PbSendMsg",strlen("MessageSvc.PbSendMsg")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_PBSENDMSG;
			}
			else if (memcmp(cmdstr,"MessageSvc.PbGetMsg",strlen("MessageSvc.PbGetMsg")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_PBGETMSG;
			}
			else if (memcmp(cmdstr,"OnlinePush.PbGetGroupMsg",strlen("OnlinePush.PbGetGroupMsg")) == 0 ||
					memcmp(cmdstr, "MessageSvc.PbGetGroupMsg",strlen("MessageSvc.PbGetGroupMsg"))==0 ||
					memcmp(cmdstr,"RegPrxySvc.PbGetGroupMsg",strlen("RegPrxySvc.PbGetGroupMsg")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_PBGETGROUPMSG;
			}
			else if(memcmp(cmdstr, "OnlinePush.PbPushGroupMsg",strlen("OnlinePush.PbPushGroupMsg"))==0 ||
				memcmp(cmdstr, "MessageSvc.PbPushGroupMsg",strlen("MessageSvc.PbPushGroupMsg"))==0 ||
				memcmp(cmdstr, "RegPrxySvc.PbPushGroupMsg",strlen("RegPrxySvc.PbPushGroupMsg"))==0 )
			{
				mqq->state.type = MOBILE_QQ_PBPUSHGROUPMSG;
			}
			else if( memcmp(cmdstr, "RegPrxySvc.PbPushMsg",strlen("RegPrxySvc.PbPushMsg"))==0 ||
					memcmp(cmdstr, "RegPrxySvc.PbGetMsg",strlen("RegPrxySvc.PbGetMsg"))==0 )
			{
				mqq->state.type = MOBILE_QQ_PBPRXYSVC;
			}
			else if (memcmp(cmdstr,"ProfileService.GetSignature",strlen("ProfileService.GetSignature")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_SIGNATURE;
			}
			else if (memcmp(cmdstr,"ProfileService.GetRichSig",strlen("ProfileService.GetRichSig")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_RICHSIG;
			}
			else if (memcmp(cmdstr,"MessageSvc.GetRoamMsgByTime",strlen("MessageSvc.GetRoamMsgByTime")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_GETROAMMSG;
			}
			else if (memcmp(cmdstr,"ProfileService.CheckUpdateReq",strlen("ProfileService.CheckUpdateReq")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_CHKUPDATE;
			}
			else if (memcmp(cmdstr,"friendlist.getTroopMemberList",strlen("friendlist.getTroopMemberList")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_GETTROOP;
			}
			else if (memcmp(cmdstr,"ProfileService.GetSimpleInfo",strlen("ProfileService.GetSimpleInfo")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_SIMPLEINFO;
			}
			else if (memcmp(cmdstr,"ProfileService.ReqGetSettings",strlen("ProfileService.ReqGetSettings")) == 0 )
			{
				mqq->state.type = MOBILE_QQ_GETSET;
			}

// 			else if (memcmp(cmdstr,"ConfigService.GetPictureServerReq",strlen("ConfigService.GetPictureServerReq")) == 0 
// 				|| memcmp(cmdstr,"KQQ.ConfigService.ConfigServantObj",strlen("KQQ.ConfigService.ConfigServantObj")) == 0 
// 				|| memcmp(cmdstr,"KQQ.ConfigService.ConfigServantObj",strlen("KQQ.ConfigService.ConfigServantObj")) == 0
// 				)
// 			{
// 				
// 			}
			else
			{
				//process another part
				//return FALSE;
			}

			phead = phead + headlen + bodylen;


#ifdef DEBUG
			if(phead < srchead + decryptlen)
			{
				int tmp = 1;
				int tmpint = 2;
			}
#endif

		}

		return TRUE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO,"mobileqq head error!\r\n");	
		return FALSE;
	}
}




BOOL getauthkey(BYTE * pData, int datalen,int b2cs,mobileqq * mqq)
{
	try
	{
		char * ptmp;
		if (b2cs)
		{
			ptmp = mystrstr("RequestHeader", strlen("RequestHeader"),(char*)pData,  datalen);
			if (ptmp == 0)
			{
				return FALSE;
			}
			memset(mqq->state.qq,0,QQ_IMEI_IMSI_LEN);
			int ret = getqqstr(ptmp, 64,mqq->state.qq);
		}
		else
		{		
			ptmp = mystrstr("RespondHeader",strlen("RespondHeader"),(char*)pData,datalen);
			if (ptmp == 0)
			{
				return FALSE;
			}
			memset(mqq->state.qq,0,QQ_IMEI_IMSI_LEN);
			int ret = getqqstr(ptmp, 64,mqq->state.qq);
		}

		ptmp = mystrstr("UserMainAccount", strlen("UserMainAccount"),(char *)pData,datalen);		//此处2处解密得到密钥
		if(ptmp)
		{
			ptmp = mystrstr("ResponseAuthWlogin",strlen("ResponseAuthWlogin"),(char *)pData,datalen);
			if(ptmp)
			{
				unsigned char key[QQ_IMEI_IMSI_LEN] = {0};
				memmove(key,ptmp - strlen("ResponseAuthWlogin") -9,4);
				
				ptmp = mystrstr("UserSimpleInfo",strlen("UserSimpleInfo"),(char *)pData,datalen);
				if (ptmp)
				{
					int nicklen = *(ptmp + 15);
					if (nicklen > NAME_LEN || nicklen <= 0)
					{
						return FALSE;
					}
					memmove(mqq->state.nick, ptmp + 16, nicklen);
				}

				mqq->state.qqnum = strtoul(mqq->state.qq,0,10);
				if(g_mobileqqkeymap.Lookup(mqq->state.qqnum,*(unsigned int*)mqq->state.key))
				{
					if (memcmp(key,(unsigned char*)mqq->state.key,4)== 0)
					{
						return FALSE;
					}
				}

				g_mobileqqkeymap.SetAt(mqq->state.qqnum,*(unsigned int*)key);
				memmove(mqq->state.key,key,4);
				savekey(mqq->state.qqnum,*(unsigned int*)key);
				return TRUE;
			}
		}

		return FALSE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"mobileqq get authkey error!\r\n");
		return FALSE;
	}
}






BOOL online(BYTE* pData, int datalen, BOOL bc2s,mobileqq * mqq)
{
	try
	{
		char * data = (char*)pData;
		if (bc2s)
		{
			data = mystrstr("RequestHeader", strlen("RequestHeader"),(char*)pData,  datalen);
			if (data )
			{
				memset(mqq->state.qq,0,QQ_IMEI_IMSI_LEN);
				int ret = getqqstr(data, 64,mqq->state.qq);
			}
		}
		else
		{
			data = mystrstr("RespondHeader", strlen("RespondHeader"),(char*)pData,  datalen);
			if (data )
			{
				memset(mqq->state.qq,0,QQ_IMEI_IMSI_LEN);
				int ret = getqqstr(data, 64,mqq->state.qq);
			}
		}

		if (mqq->state.onlineflag == 0)
		{
			if (*(mqq->state.qq) && *(mqq->state.imei) && *(mqq->state.imsi) )
			{
				mqq->writefile();
				mqq->state.onlineflag = 1;
			}
			else if (*(mqq->state.qq) && *(mqq->state.imsi) )
			{
				mqq->writefile();
				//mqq->state.onlineflag = 1;
			}
			else if (*(mqq->state.qq) && *(mqq->state.imei) )
			{
				mqq->writefile();
				//mqq->state.onlineflag = 1;
			}	
		}
		return TRUE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO,"mobileqq online error!\r\n");
		return FALSE;
	}

}





BOOL getset(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("GeneralSettings.RespGetSettings",strlen("GeneralSettings.RespGetSettings"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		while (ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x06",2, ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				return FALSE;
			}
			
			ptmp = flag;
			flag = mystrstr("message.group.policy.",strlen("message.group.policy."),ptmp,32);
			if (flag == 0)
			{
				return FALSE;
			}
			
			ptmp = flag;
			char * ender = mystrstr("\x16\x01",2,flag,32);
			if (ender == 0)
			{
				return FALSE;
			}
			ender -= 2;

			char groupid[QQ_IMEI_IMSI_LEN] = {0};
			memmove(groupid, flag, ender - flag);
			sprintf(mqq->state.groupid,"%s",groupid);
			//sprintf(mqq->state.msg,"群号:%s\r\n",groupid);
			mqq->writefile();
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"mobileqq getset error\r\n");
		return FALSE;
	}
}






BOOL simpleinfo(BYTE * pData, int datalen, mobileqq * mqq)
{	
	try
	{
		char * ptmp = mystrstr("list<KQQ.ProfSmpInfoRes>",strlen("list<KQQ.ProfSmpInfoRes>"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		unsigned int len = 0;
		char * flag = mystrstr("\x00\x00\x00\x00",4, ptmp,datalen - (ptmp - (char*)pData));
		if (flag == 0)
		{
			return FALSE;
		}
		unsigned int qq = ntohl(*(unsigned int*)flag);

		flag += 4;
		ptmp = flag;
		flag = mystrstr("\x56",1,ptmp,32);
		if (flag == 0)
		{
			return FALSE;
		}
		len = *(unsigned char *)flag;
		flag ++;
		char nick[NAME_LEN];
		memmove(nick, flag, len);
		Utf8ToGB2312(nick, len);
		flag += len;
		ptmp = flag;

		char country[NAME_LEN];
		flag = mystrstr("\xd6",1,flag,64);
		if (flag )
		{
			len = *(unsigned char *)flag;
			flag ++;
			memmove(country, flag, len);
			Utf8ToGB2312(country, len);
			flag += len;
			ptmp = flag;
		}
		else
		{
			return FALSE;
		}

		char province[NAME_LEN];
		flag = mystrstr("\xe6",1,flag,64);
		if (flag )
		{
			len = *(unsigned char *)flag;
			flag ++;
			memmove(province, flag, len);
			Utf8ToGB2312(province, len);
			flag += len;
			ptmp = flag;
		}
		else
		{
			return FALSE;
		}

		char city[NAME_LEN];
		flag = mystrstr("\xf6",1, ptmp,datalen - (ptmp - (char*)pData));
		if (flag)
		{
			flag ++;
			len = *(unsigned char *)flag;
			flag ++;
			memmove(city, flag,len);
			Utf8ToGB2312(city,len);
			flag += len;
			ptmp = flag;	
		}
		else
		{
			return FALSE;
		}

		sprintf(mqq->state.to,"QQ号码:%u 昵称:%s 签名:(%s)",qq,nick,0);
		mqq->writefile();
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"simpleinfo error\r\n");
		return FALSE;
	}
}





BOOL friendlist(BYTE* pData, int datalen,mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("mqq.IMService.FriendListServiceServantObj",strlen("mqq.IMService.FriendListServiceServantObj"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}
		
		ptmp = mystrstr("FLRESP",strlen("FLRESP"),(char*)pData,datalen );
		if (ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		unsigned int len = 0;
		unsigned int friendqq = 0;
		char nick[256] = {0};
		while(ptmp < (char*)pData + datalen)
		{
			char onlinestate[NAME_LEN];
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					break;
				}
				
				flag += 4;
			}
			friendqq = ntohl(*(unsigned int*)flag);

			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x36",1,ptmp,32);
			if (flag == 0)
			{
				continue;
				return FALSE;
			}
			len = *(unsigned char *)flag;
			flag ++;
			memmove(nick, flag, len);
			Utf8ToGB2312(nick, len);
			flag += len;
			ptmp = flag;

			char mail[NAME_LEN];
			flag = mystrstr("\xc6",1,flag,64);
			if (flag )
			{
				len = *(unsigned char *)flag;
				flag ++;
				memmove(mail, flag, len);
				Utf8ToGB2312(mail, len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				continue;
				return FALSE;
			}

			char name[NAME_LEN];
			flag = mystrstr("\xe6",1,flag,64);
			if (flag )
			{
				len = *(unsigned char *)flag;
				flag ++;
				memmove(name, flag, len);
				Utf8ToGB2312(name, len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				continue;
				return FALSE;
			}

			flag = mystrstr("\xf6\x1b",2, ptmp,datalen - (ptmp - (char*)pData));
			if (flag)
			{
				len = *(unsigned char *)flag;
				if (len <= NAME_LEN)
				{
					flag ++;
					memmove(onlinestate, flag,len);
					Utf8ToGB2312(onlinestate,len);
					flag += len;
					ptmp = flag;	
				}
				else
				{
					continue;
					return FALSE;
				}
			}

			sprintf(mqq->state.to,"QQ号码:%u 昵称:%s 签名:(%s)",friendqq,nick,name);
			mqq->writefile();
			cnt ++;
		}

		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"friendlist error\r\n");
		return  FALSE;
	}
}





BOOL onlinefriend(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("FSOLRESP",strlen("FSOLRESP"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					break;
				}

				flag += 4;
			}
			unsigned int friendqq = ntohl(*(unsigned int*)flag);
			
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\xc0",1,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				return FALSE;
			}
			ptmp = flag;
			flag = mystrstr("\xd0",1,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				return FALSE;
			}
			ptmp = flag;
			flag = mystrstr("\xe6",1,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				return FALSE;
			}
			
			char onlinestate[NAME_LEN];
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(onlinestate, flag, len);
			Utf8ToGB2312(onlinestate, len);
			sprintf(mqq->state.to, "QQ号码:%u 昵称:%s 上线:%s",friendqq,0,onlinestate);
			flag += len;
			ptmp = flag;
			mqq->writefile();
			cnt ++;		
		}
	
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"onlinefriend error\r\n");
		return FALSE;
	}

}





BOOL pbsendmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("\x00\x1a",2,(char*)pData,datalen );
		if (ptmp == 0)
		{
			ptmp = mystrstr("\x10\x01",2,(char*)pData,datalen );
			if (ptmp == 0)
			{
				ptmp = (char*)pData;
			}	
		}

		char * flag = mystrstr("\x0a",1,(char*)ptmp,32);
		if (flag == 0)
		{
			flag = (char*)ptmp;
		}


		unsigned int len = 0;
		ptmp = flag;
		for (int i = 0; i < datalen - ((char*)ptmp - (char*)pData); i ++)
		{
			if (*(flag + i) == 0x12 && *(flag + i + 2) == 0x0a && *(flag + i + 4) == 0x0a)
			{
				flag += i;
				flag += 5;
				len = *(unsigned char *)flag;
				flag ++;
				break;
			}
			else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 6) == 0x0a )
			{
				flag += i;
				flag += 7;
				len = *(unsigned char*)flag;
				flag += 2;
				break;
			}
			else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 5) == 0x0a)
			{
				flag += i;
				flag += 6;
				len = *(unsigned char *)flag;
				flag ++;
				break;
			}
		}
		
		if (i >= datalen - ((char*)ptmp - (char*)pData))
		{
			return FALSE;
		}

		memmove(mqq->state.msg, flag, len);
		Utf8ToGB2312(mqq->state.msg,len);
		strcpy(mqq->state.from , mqq->state.qq);
		mqq->writefile();
		flag += len;
		ptmp = flag;	
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pbsendmsg error\r\n");
		return FALSE;
	}
}




//ok
BOOL pbgetmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = (char*)pData;
		int cnt = 0;
		while (ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x10\x01",2,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				flag = mystrstr("\x00\x1a",2,ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					flag = ptmp;
				}		
			}

			ptmp = flag;
			unsigned int len = 0;
			char nick[NAME_LEN]= {0};					//可能没有昵称 必须先初始化
			flag = mystrstr("\x22",1,(char*)ptmp,32);
			if (flag)
			{
				len = *(unsigned char *)flag;
				flag ++;
				memmove(nick,flag ,len);
				Utf8ToGB2312(nick,len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				//有些数据包这里没有QQ昵称
				flag = ptmp;
			}
			
			flag = mystrstr("\x86\x01\x40",3,(char*)ptmp,datalen - ((char*)ptmp - (char*)pData));	//字体
			if (flag)
			{
				flag += 2;
				len = *(unsigned char *)flag;
				flag ++;
				char lettertype[NAME_LEN];
				memmove(lettertype,flag ,len);
				Utf8ToGB2312(lettertype,len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				//字体 没有字体字段的肯定没有消息字段
				return FALSE;
			}

			//flag = mystrstr("\x4d\xe7",2,(char*)pData,datalen);
			//flag = ptmp;

			for (int i = 0; i < datalen - ((char*)ptmp - (char*)pData); i ++)
			{
				if (*(flag + i) == 0x12 && *(flag + i + 2) == 0x0a && *(flag + i + 4) == 0x0a)
				{
					flag += i;
					flag += 5;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 6) == 0x0a )
				{
					flag += i;
					flag += 7;
					len = *(unsigned char*)flag;
					flag += 2;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 5) == 0x0a)
				{
					flag += i;
					flag += 6;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
			}
			
			if (i >= datalen - ((char*)ptmp - (char*)pData))
			{
				return FALSE;
			}

			memmove(mqq->state.msg, flag, len);
			Utf8ToGB2312(mqq->state.msg,len);
			strcpy(mqq->state.from, nick);
			mqq->writefile();
			flag += len;
			ptmp = flag;	
			cnt ++;
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pbgetmsg error\r\n");
		return FALSE;
	}
}






//有问题有问题
BOOL pbprxysvc(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = (char*)pData;
		int cnt = 0;
		while (ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x10\x01",2,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				flag = mystrstr("\x00\x1a",2,ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					flag = ptmp;
				}
			}

			unsigned int len = *(unsigned char *)flag;
			flag ++;
			char nick[NAME_LEN] = {0};
			ptmp = flag;
			flag = mystrstr("\x22",1,(char*)ptmp,32);
			if (flag )
			{
				memmove(nick,flag ,len);
				Utf8ToGB2312(nick,len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				flag = ptmp;
			}

			flag = mystrstr("\x86\x01\x40",3,(char*)ptmp,datalen - ((char*)ptmp - (char*)pData));	//字体
			if (flag)
			{
				flag += 2;
				len = *(unsigned char *)flag;
				flag ++;
				char lettertype[NAME_LEN];
				memmove(lettertype,flag ,len);
				Utf8ToGB2312(lettertype,len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				return FALSE;
			}

			for (int i = 0; i < datalen - ((char*)ptmp - (char*)pData); i ++)
			{
				if (*(flag + i) == 0x12 && *(flag + i + 2) == 0x0a && *(flag + i + 4) == 0x0a)
				{
					flag += i;
					flag += 5;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 6) == 0x0a )
				{
					flag += i;
					flag += 7;
					len = *(unsigned char*)flag;
					flag += 2;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 5) == 0x0a)
				{
					flag += i;
					flag += 6;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
			}
			
			if (i >= datalen - ((char*)ptmp - (char*)pData))
			{
				return FALSE;
			}
			char msg[MSG_LEN];
			memmove(msg,flag ,len);
			Utf8ToGB2312(msg,len);
			if (*nick)
			{
				strcpy(mqq->state.from,nick);
			}
			else
			{
				strcpy(mqq->state.from,mqq->state.qq);
			}
			
			strcpy(mqq->state.msg,msg);
			mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pbprxysvc error\r\n");
		return FALSE;
	}
}






BOOL pbgetgroupmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = (char*)pData;
		int cnt = 0;
		while (ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x10\x01",2,ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				return FALSE;
			}

			ptmp = flag;
			flag = mystrstr("\x22",1,(char*)ptmp,32);
			if (flag == 0)
			{
				return FALSE;
			}

			unsigned int len = *(unsigned char *)flag;
			flag ++;
			char nick[256];
			memmove(nick,flag ,len);
			Utf8ToGB2312(nick,len);
			flag += len;
			ptmp = flag;

			//flag = mystrstr("\x00\x1a",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
			//if (flag == 0)
			//{
			//	return FALSE;
			//}

			flag = mystrstr("\x86\x01\x40",3,(char*)ptmp,datalen - ((char*)ptmp - (char*)pData));	//字体
			if (flag )
			{
				flag += 2;
				len = *(unsigned char *)flag;
				flag ++;
				char lettertype[NAME_LEN];
				memmove(lettertype,flag ,len);
				Utf8ToGB2312(lettertype,len);
				flag += len;
				ptmp = flag;
			}
			else
			{
				return FALSE;
			}

			for (int i = 0; i < datalen - ((char*)ptmp - (char*)pData); i ++)
			{
				if (*(flag + i) == 0x12 && *(flag + i + 2) == 0x0a && *(flag + i + 4) == 0x0a)
				{
					flag += i;
					flag += 5;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 6) == 0x0a )
				{
					flag += i;
					flag += 7;
					len = *(unsigned char*)flag;
					flag += 2;
					break;
				}
				else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 5) == 0x0a)
				{
					flag += i;
					flag += 6;
					len = *(unsigned char *)flag;
					flag ++;
					break;
				}
			}

			if (i >= datalen - ((char*)ptmp - (char*)pData))
			{
				return FALSE;
			}

			//if (*(unsigned short*)flag == 0x0008)
			//{
			//	flag += 2;
			//}

			char msg[MSG_LEN];
			memmove(msg,flag ,len);
			Utf8ToGB2312(msg,len);
			strcpy(mqq->state.from,nick);
			strcpy(mqq->state.msg,msg);
			mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pbgetgroupmsg error\r\n");
		return FALSE;
	}
}




BOOL pbpushgroupmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("\x10\x01",2,(char*)pData,datalen);
		if (ptmp == 0)
		{
			return FALSE;
		}

		char * flag = mystrstr("\x22",1,(char*)ptmp,32);
		if (flag == 0)
		{
			return FALSE;
		}

		unsigned int len = *(unsigned char *)flag;
		flag ++;
		char nick[NAME_LEN];
		memmove(nick,flag ,len);
		Utf8ToGB2312(nick,len);
		flag += len;
		ptmp = flag;

		//flag = mystrstr("\x00\x1a",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
		//if (flag == 0)
		//{
		//	return FALSE;
		//}

		flag = mystrstr("\x86\x01\x40",3,(char*)ptmp,datalen - ((char*)ptmp - (char*)pData));	//字体
		if (flag)
		{
			flag += 2;
			len = *(unsigned char *)flag;
			flag ++;
			char lettertype[NAME_LEN];
			memmove(lettertype,flag ,len);
			Utf8ToGB2312(lettertype,len);
			flag += len;
			ptmp = flag;
		}
		else
		{
			return FALSE;
		}

		for (int i = 0; i < datalen - ((char*)ptmp - (char*)pData); i ++)
		{
			if (*(flag + i) == 0x12 && *(flag + i + 2) == 0x0a && *(flag + i + 4) == 0x0a)
			{
				flag += i;
				flag += 5;
				len = *(unsigned char *)flag;
				flag ++;
				break;
			}
			else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 6) == 0x0a )
			{
				flag += i;
				flag += 7;
				len = *(unsigned char*)flag;
				flag += 2;
				break;
			}
			else if (*(flag + i) == 0x12 && *(flag + i + 3) == 0x0a && *(flag + i + 5) == 0x0a)
			{
				flag += i;
				flag += 6;
				len = *(unsigned char *)flag;
				flag ++;
				break;
			}
		}
		
		if (i >= datalen - ((char*)ptmp - (char*)pData))
		{
			return FALSE;
		}


		memmove(mqq->state.msg,flag,len);
		Utf8ToGB2312(mqq->state.msg,len);
		strcpy(mqq->state.from,nick);
		mqq->writefile();
		flag += len;
		ptmp = flag;
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pbpushgroupmsg error\r\n");
		return FALSE;
	}
}







BOOL pushgetfriendmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("req_SendMsg",strlen("req_SendMsg"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			ptmp = mystrstr("req_GetMsg",strlen("req_GetMsg"),(char*)pData,datalen);
			if (ptmp == 0)
			{
				return FALSE;
			}
		}
		
		ptmp = mystrstr("\x0a\x00",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
		if (ptmp == 0)
		{
			return FALSE;
		}
		
		ptmp = mystrstr("\x0b\x00",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
		if (ptmp == 0)
		{
			return FALSE;
		}
		unsigned int len = *(unsigned char *)ptmp;
		ptmp ++;
		memmove(mqq->state.msg, ptmp,len);
		Utf8ToGB2312(mqq->state.msg,len);
		ptmp += len;
		ptmp ++;
		if (*ptmp == 0x42)
		{
			ptmp ++;
			unsigned int qq = ntohl(*(unsigned int*)ptmp);
			ultoa(qq,mqq->state.to,10);
		}

		//strcpy(mqq->state.from,mqq->state.qq);
		mqq->writefile();
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pushgetfriendmsg error\r\n");
		return FALSE;
	}
}








BOOL pushgetfriendmsgv2(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("req_SendMsgV",strlen("req_SendMsgV"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			ptmp = mystrstr("req_GetMsgV",strlen("req_GetMsgV"),(char*)pData,datalen);
			if (ptmp == 0)
			{
				ptmp = mystrstr("resp_GetMsgV",strlen("resp_GetMsgV"),(char*)pData,datalen);
				if (ptmp == 0)
				{
					ptmp = mystrstr("resp_SendMsgV",strlen("resp_SendMsgV"),(char*)pData,datalen);
					if (ptmp == 0)
					{
						return FALSE;
					}
				}
			}
		}

		char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				return FALSE;
			}
			flag += 4;
		}

		unsigned int toqq = ntohl(*(unsigned int*)flag);			
		ultoa(toqq,mqq->state.to,10);
		
		flag += 4;
		if (*flag != 0x1c && *flag != 0x13 && *flag != 0x12)
		{
			//continue;
		}
		flag ++;
		
		ptmp = flag;
		flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		//1d 00 00 xx 0d 0a 02
		if (flag == 0)
		{
			return FALSE;
		}
		unsigned int fromqq = ntohl(*(unsigned int*)flag);
		ultoa(fromqq,mqq->state.from,10);
		
		flag += 4;
		if (*flag != 0x12 && *flag != 0x13)
		{
			//continue;
		}
		flag ++;
		
		ptmp = flag;
		flag = mystrstr("\x46\x00\x52\x55",4, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			return FALSE;
		}
		flag += 3;
		
		
		ptmp = flag;
		if (memcmp(flag,"\x8d\x00",2) == 0 || memcmp(flag,"\x6d\x00",2) == 0 )
		{
			flag += 2;

			unsigned int len = *(unsigned char*)flag;
			if (len == 1)
			{
				flag ++;
				len = ntohs(*(unsigned short*)flag );
				if (len >= MSG_LEN)
				{
					return FALSE;
				}
				flag += 2;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				len = Utf8ToGB2312(mqq->state.msg, len);
			}
			else if (len == 0)
			{
				flag ++;
				len = *(unsigned char*)flag;
				flag ++;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				Utf8ToGB2312(mqq->state.msg, len);
			}
			else
			{
				return FALSE;
			}

			flag += len;
			ptmp = flag;
			mqq->writefile();
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pushgetfriendmsgv2 error\r\n");
		return FALSE;
	}
}







BOOL sendgroupmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("req_SendGroupMsg",strlen("req_SendGroupMsg"),(char*)pData,datalen);	//群消息请求
		if(ptmp == 0)
		{
			ptmp = mystrstr("resp_SendGroupMsg",strlen("resp_SendGroupMsg"),(char*)pData,datalen);
			if (ptmp == 0)
			{
				return FALSE;
			}
		}

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		//1d 00 00 xx 0d 0a 02
			if (flag == 0)
			{	
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		//1d 00 00 xx 0d 0a 03
				if (flag == 0)
				{
					break;
				}

				flag += 4;
				unsigned int fromqq = ntohl(*(unsigned int*)flag);
				ultoa(fromqq,mqq->state.from,10);
				
				flag += 4;
				if (*flag != 0x12 && *flag != 0x13)		//1c空消息
				{
					ptmp += 16;
					continue;
				}
				flag ++;
				unsigned int toqq = ntohl(*(unsigned int*)flag);
				ultoa(toqq,mqq->state.to,10);
				
				flag += 4;
				if (*flag != 0x26)
				{
					ptmp += 16;
					continue;
				}
				flag ++;

				unsigned int len = *(unsigned char*)flag;
				flag ++;
				memmove(mqq->state.msg, flag, len);
				Utf8ToGB2312(mqq->state.msg, len);
				ptmp = flag;
				cnt ++;
				mqq->writefile();
				continue;
			}

			unsigned int fromqq = ntohl(*(unsigned int*)flag);
			ultoa(fromqq,mqq->state.from,10);
			flag += 4;

			if (*flag != 0x12 && *flag != 0x13)
			{
				ptmp += 16;
				continue;
			}
			flag ++;
			unsigned int toqq = ntohl(*(unsigned int*)flag);
			ultoa(toqq,mqq->state.groupid,10);
			
			flag += 4;
			ptmp = flag;
			if (*flag != 0x26)
			{
				ptmp += 16;
				continue;
			}
			flag ++;
			
			unsigned int len = *(unsigned char*)flag;
			flag ++;
			memmove(mqq->state.msg, flag, len);
			Utf8ToGB2312(mqq->state.msg, len);
			ptmp = flag;
			cnt ++;
			mqq->writefile();
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"sendgroupmsg error\r\n");
		return FALSE;
	}
}




BOOL getroammsg(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * 	ptmp = mystrstr("resp_GetRoamMsg",strlen("resp_GetRoamMsg"),(char*)pData,datalen);
		if (ptmp == 0)
		{
			return FALSE;	
		}

		char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));	
			if (flag == 0)
			{
				return FALSE;
			}
			flag +=4;
		}
		unsigned int from = ntohl(*(unsigned int*)flag);
		
		flag += 4;
		if (*flag != 0x12 && *flag != 0x1c)
		{

		}

		ptmp = flag;
		flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));	
			if (flag == 0)
			{
				return FALSE;
			}
			flag +=4;
		}
		unsigned int to = ntohl(*(unsigned int*)flag);
		flag += 4;
		ptmp = flag;
			
		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			ptmp = flag;
			flag = mystrstr("\x7d\x00\x00",3, ptmp,datalen - (ptmp - (char*)pData));		//55 5a
			if (flag == 0)
			{
				break;
			}	
			
			unsigned int len = *(unsigned char*)flag;
			flag += 1;
			memmove(mqq->state.msg, flag, len);
			mqq->state.msg[len] = 0;
			Utf8ToGB2312(mqq->state.msg, len);
			flag += len;
			ptmp = flag;

			cnt ++;
			ultoa(from,mqq->state.from,10);
			ultoa(to,mqq->state.to,10);
			mqq->writefile();
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"getroammsg error\r\n");
		return FALSE;
	}
}






//ok
BOOL getgroupmsg(BYTE* pData, int datalen, mobileqq * mqq)	
{
	try
	{	
		char * 	ptmp = mystrstr("resp_GetGroupMsg",strlen("resp_GetGroupMsg"),(char*)pData,datalen);
		if (ptmp == 0)
		{
			ptmp = mystrstr("resp_PullGroupMsg",strlen("resp_PullGroupMsg"),(char*)pData,datalen);	//请求无数据
			if (ptmp == 0)
			{
				return FALSE;
			}
		}
		char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));	
			if (flag == 0)
			{
				return FALSE;
			}
			flag +=4;
		}


		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));	
				if (flag == 0)
				{
					break;
				}
				flag +=4;
			}
			unsigned int groupqq = ntohl(*(unsigned int*)flag);
			ultoa(groupqq,mqq->state.groupid,10);
			flag += 4;

			//if (memcmp(flag, "\x10\x01\x23",3))
			//{
				//if (memcmp(flag, "\x10\x01\x22",3))
				//{
					//return FALSE;
				//}
			//}
			flag += 3;
			unsigned int fromqq = ntohl(*(unsigned int*)flag);
			ultoa(fromqq,mqq->state.from,10);
			flag += 4;
			/*
			flag += 2;
			unsigned int len = *(unsigned char*)flag;
			char info[256];
			flag ++;
			memmove(info, flag, len);
			Utf8ToGB2312(info,len);

			flag += len;
			if (*flag != 0x46)
			{
			}
			ptmp = flag;
			flag = mystrstr("\x76\x00",2, ptmp,datalen - (ptmp - (char*)pData));		//55 5a
			if (flag == 0)
			{

			}
			*/
			
			ptmp = flag;
			flag = mystrstr("\x8d\x00",2, ptmp,datalen - (ptmp - (char*)pData));		//55 5a
			if (flag == 0)
			{
				flag = mystrstr("\x6d\x00",2, ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					continue;
				}
			}	
			
			unsigned int len = *(unsigned char*)flag;
			if (len == 1)
			{
				flag ++;
				len = ntohs(*(unsigned short*)flag );
				if (len >= MSG_LEN)
				{
					return FALSE;
				}
				flag += 2;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				Utf8ToGB2312(mqq->state.msg, len);
			}
			else if (len == 0)
			{
				flag ++;
				len = *(unsigned char*)flag;
				flag ++;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				Utf8ToGB2312(mqq->state.msg, len);
			}
			else
			{
				ptmp = flag;
				continue;
			}

			flag += len;
			ptmp = flag;

			cnt ++;
			mqq->writefile();			//10 messages 1 packet
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"getgroupmsg error\r\n");
		return FALSE;
	}
}




//ok
BOOL pushgroupmsg(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("req_PushGroupMsg",strlen("req_PushGroupMsg"),(char*)pData,datalen - strlen("req_PushGroupMsg"));
		if(ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}
				flag += 4;
			}

			flag += 4;
			ptmp = flag;
			flag = mystrstr("PushNotifyB",strlen("PushNotifyB"),ptmp,datalen - (ptmp - (char*)pData));
			if(flag == 0)
			{
				continue;
			}
			unsigned int groupqq = ntohl(*(unsigned int*)flag);
			ultoa(groupqq,mqq->state.groupid,10);		

			flag += 4;
			flag += 3;
			unsigned int fromqq = ntohl(*(unsigned int*)flag);
			ultoa(fromqq,mqq->state.from,10);
			flag += 4;

			ptmp = flag;
			flag = mystrstr("\xbd\x00",2,ptmp,datalen - (ptmp - (char*)pData));
			if(ptmp == 0)
			{
				flag = mystrstr("\x8d\x00",2,ptmp,datalen - (ptmp - (char*)pData));
				if(ptmp == 0)
				{
					break;
				}
			}

			unsigned int len = *(unsigned char*)flag;
			if (len == 1)
			{
				flag ++;
				len = ntohs(*(unsigned short*)flag );
				if (len >= MSG_LEN)
				{
					return FALSE;
				}
				flag += 2;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				len = Utf8ToGB2312(mqq->state.msg, len);
			}
			else if (len == 0)
			{
				flag ++;
				len = *(unsigned char*)flag;
				flag ++;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				Utf8ToGB2312(mqq->state.msg, len);
			}
			else
			{
				continue;
			}

			flag += len;
			ptmp = flag;
			char groupmembernick[256];
			flag = mystrstr("\xc6",1,ptmp,datalen - (ptmp - (char*)pData));
			if(flag)
			{
				len = *(unsigned char*)flag;
				flag ++;				
				memmove(groupmembernick, flag,len);
				Utf8ToGB2312(groupmembernick, len);
				flag += len;
				ptmp = flag;
			}

			ptmp = flag;
			cnt ++;
			mqq->writefile();
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"pushgroupmsg error\r\n");
		return FALSE;
	}
}





//ok
BOOL onlinereqpush(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("OnlinePushPack.SvcReqPushMsg",strlen("OnlinePushPack.SvcReqPushMsg"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}
		
		char * flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			return FALSE;
		}
		
		flag += 4;
		unsigned int toqq = ntohl(*(unsigned int*)flag);			
		ultoa(toqq,mqq->state.to,10);
		
		flag += 4;
		if (*flag != 0x12 && *flag != 0x13)
		{
			//continue;
		}
		flag ++;

		ptmp = flag;
		flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		//1d 00 00 xx 0d 0a 02
		if (flag == 0)
		{
			return FALSE;
		}
		unsigned int fromqq = ntohl(*(unsigned int*)flag);
		ultoa(fromqq,mqq->state.from,10);

		flag += 4;
		if (*flag != 0x12 && *flag != 0x13)
		{
			//continue;
		}
		flag ++;

		ptmp = flag;
		flag = mystrstr("\x46\x00\x52\x55",4, ptmp,datalen - (ptmp - (char*)pData));		
		if (flag == 0)
		{
			return FALSE;
		}
		flag += 3;
		
		ptmp = flag;
		if (memcmp(flag,"\x8d\x00",2) == 0 || memcmp(flag,"\x6d\x00",2) == 0 )
		{
			flag += 2;
			unsigned int len = *(unsigned char*)flag;
			if (len == 1)
			{
				flag ++;
				len = ntohs(*(unsigned short*)flag );
				if (len >= MSG_LEN)
				{
					return FALSE;
				}
				flag += 2;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				len = Utf8ToGB2312(mqq->state.msg, len);
			}
			else if (len == 0)
			{
				flag ++;
				len = *(unsigned char*)flag;
				flag ++;
				memmove(mqq->state.msg, flag, len);
				mqq->state.msg[len] = 0;
				Utf8ToGB2312(mqq->state.msg, len);
			}
			else
			{
				return FALSE;
			}

			flag += len;
			ptmp = flag;
			mqq->writefile();
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"onlinereqpush error\r\n");
		return FALSE;
	}
}




BOOL offlinemsg(BYTE* pData, int datalen, mobileqq * mqq)		//ok
{
	try
	{
		char * ptmp = mystrstr("req_offlinemsg",strlen("req_offlinemsg"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}
				flag += 4;
				ptmp = flag;
				flag = mystrstr("\x13",1,ptmp,64);
				if (flag == 0)
				{
					return FALSE;
				}
				flag += 4;
				unsigned int fromqq = ntohl(*(unsigned int*)flag);			
				ultoa(fromqq,mqq->state.from,10);
				flag += 4;
			}
			else
			{
				ptmp = flag;
				flag = mystrstr("\x12",1,ptmp,64);
				if (flag == 0)
				{
					return FALSE;
				}
				unsigned int fromqq = ntohl(*(unsigned int*)flag);			
				ultoa(fromqq,mqq->state.from,10);
				flag += 4;
			}

			ptmp = flag;
			flag = mystrstr("\x22",1,ptmp,64);
			if (flag == 0)
			{
				flag = mystrstr("\x23",1,ptmp,64);
				if (flag == 0)
				{
					flag = mystrstr("\x26",1,ptmp,64);
					if (flag == 0)
					{
						return FALSE;
					}
				}
				flag += 4;
			}
			unsigned int toqq = ntohl(*(unsigned int*)flag);
			ultoa(toqq,mqq->state.to,10);
			flag += 4;

			ptmp = flag;
			flag = mystrstr("\x55",1, ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				flag = ptmp;
				//break;
			}
			ptmp = flag;
			flag = mystrstr("\x46",1, ptmp,datalen - (ptmp - (char*)pData));
			if (flag == 0)
			{
				break;
			}	
			unsigned int len = *(unsigned char*)flag;
			flag ++;
			memmove(mqq->state.msg, flag, len);
			Utf8ToGB2312(mqq->state.msg, len);
			flag += len;
			ptmp = flag;
			cnt ++;
			mqq->writefile();
		}

		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"offlinemsg error\r\n");
		return FALSE;
	}
}




BOOL recommendlist(BYTE * pData, int datalen,mobileqq * mqq)
{
	return TRUE;

	try
	{
		int cnt = 0;
		char * 	ptmp = (char*)pData;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x40\x01\x56\x03",4, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				break;
			}
			flag += 4;
			unsigned int len = *(unsigned char*)flag;
			flag += 1;
			char phone[QQ_IMEI_IMSI_LEN] = {0};
			memmove(phone, flag, len);
			
			flag += len;
			if (*flag != 0x76)
			{
				ptmp += 64;
				continue;
			}
			flag ++;
			
			len = *(unsigned char*)flag;
			flag ++;
			char username[NAME_LEN] = {0};
			memmove(username, flag, len);
			Utf8ToGB2312(username, len);

			mqq->writecontact(username,phone);
			//sprintf(mqq->state.msg, "姓名:% 电话号码:%s",username,phone);
			//mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;	
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"recommendlist error\r\n");
		return FALSE;
	}
}





BOOL contact(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		int cnt = 0;
		char * 	ptmp = (char*)pData;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x06",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				break;
			}
			unsigned int len = *(unsigned char*)flag;
			flag += 1;
			char phone[NAME_LEN] = {0};
			memmove(phone, flag, len);

			flag += len;
			if (*flag != 0x16)
			{
				ptmp = flag;		//有些联系人格式不完整 不能退出
				continue;
			}
			flag ++;

			len = *(unsigned char*)flag;
			flag ++;
			char username[NAME_LEN] = {0};
			memmove(username, flag, len);
			Utf8ToGB2312(username, len);
			mqq->writecontact(username,phone);

			sprintf(mqq->state.msg, "姓名:%s 电话号码:%s",username,phone);
			mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;	
		}
		
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"contact error\r\n");
		return FALSE;
	}
}





BOOL bindstate(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("RespondQueryQQBindingStat",strlen("RespondQueryQQBindingStat"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		char * flag = mystrstr("\x0a\x06",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
		if(flag == 0)
		{
			return FALSE;
		}

		ptmp = flag;
		flag = mystrstr("\x16",1,(char*)ptmp,datalen - (ptmp - (char*)pData));
		if(flag == 0)
		{
			return FALSE;
		}

		unsigned int len = *(unsigned char*)flag;
		flag ++;
		char phone[NAME_LEN] = {0};
		memmove(phone, flag, len);
		flag += len;
		if (*flag != 0x26)
		{
			return FALSE;
		}
		flag ++;

		len = *(unsigned char*)flag;
		flag ++;
		char imeimac[NAME_LEN] = {0};
		memmove(imeimac, flag, len);
		imeimac[len] = 0;

		strcpy(mqq->state.phone,phone);
		sprintf(mqq->state.to, "QQ号码:%s 昵称:%s 签名:(%s)",mqq->state.qq,mqq->state.phone,0);
		mqq->writefile();
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"bindstate error\r\n");
		return FALSE;
	}
}







BOOL trooplist(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("RespondQueryQQBindingStat",strlen("RespondQueryQQBindingStat"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		unsigned int groupownerqq = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}

				flag += 8;
				ptmp = flag;
				flag = mystrstr("\x12",1, ptmp,16);		
				if (flag == 0)
				{
					ptmp += 16;
					continue;
				}
				unsigned int groupid = ntohl(*(unsigned int*)flag);
				
				flag += 4;
				ptmp = flag;
				flag = mystrstr("\x46",1, ptmp,16);		
				if (flag == 0)
				{
					ptmp += 16;
					continue;
				}
				unsigned int len = *(unsigned char *)flag;
				len ++;
				char groupname[NAME_LEN];
				memmove(groupname, flag, len);
				Utf8ToGB2312(groupname,len);
				flag += len;

				char desc[4096];
				if (*flag == 0x56)
				{
					flag ++;
					len = *(unsigned char *)flag;
					flag ++;
					memmove(desc, flag, len);
					Utf8ToGB2312(desc,len);
				}

				ultoa(groupid,mqq->state.groupid,10);
				strcpy(mqq->state.groupname,groupname);

				//sprintf(mqq->state.msg, "groupid:%u groupname:%s groupdesc:%s",groupid,groupname,desc);
				mqq->writefile();
				flag += len;
				ptmp = flag;
				continue;
			}

			unsigned int groupid = ntohl(*(unsigned int*)flag);
			flag += 4;
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x46",1, ptmp,16);		
			if (flag == 0)
			{
				ptmp += 16;
				continue;
			}
			unsigned int len = *(unsigned char *)flag;
			flag ++;
			char groupname[NAME_LEN];
			memmove(groupname, flag, len);
			Utf8ToGB2312(groupname,len);
			flag += len;

			char desc[4096];
			if (*flag == 0x56)
			{
				flag ++;
				len = *(unsigned char *)flag;
				flag ++;
				memmove(desc, flag, len);
				Utf8ToGB2312(desc,len);
			}
			ultoa(groupid,mqq->state.groupid,10);
			strcpy(mqq->state.groupname,groupname);
			//sprintf(mqq->state.msg, "群号:%u 群名称:%s 签名:%s",groupid,groupname,desc);
			mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;
		}

		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"trooplist error\r\n");
		return FALSE;
	}
}




//如何写文件 ？
BOOL discussinfo(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("QQService.RespGetDiscussInfo",strlen("QQService.RespGetDiscussInfo"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
			if(flag == 0)
			{
				flag = mystrstr("\x0a\x03",2,(char*)ptmp,datalen - (ptmp - (char*)pData));
				if (flag == 0)
				{
					return FALSE;
				}
				flag += 4;
			}
			unsigned int qq = ntohl(*(unsigned int*)flag);
			flag += 4;

			ptmp = flag;
			flag = mystrstr("\x41\x46",2,(char*)ptmp,16);
			if(flag == 0)
			{
				flag = mystrstr("\x16\x00\x0b\x3a\x06",5,(char*)ptmp,16);
				if(flag == 0)
				{	
					return FALSE;
				}
			}
			
			unsigned int len = *(unsigned char *)flag;
			flag ++;
			char groupmembernick[NAME_LEN];
			memmove(groupmembernick, flag, len);
			Utf8ToGB2312(groupmembernick,len);
			sprintf(mqq->state.msg, "群成员:%u 成员昵称:%s",qq,groupmembernick);
			mqq->writefile();
			flag += len;
			ptmp = flag;
			cnt ++;
		}

		return FALSE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"discussinfo error\r\n");
		return FALSE;
	}
}






BOOL groupinfo(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("GMTIRESP",strlen("GMTIRESP"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}
		
		int cnt = 0;
		ptmp = (char*)pData;
		unsigned int groupownerqq = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}

				flag += 8;
			}

			ptmp = flag;
			flag = mystrstr("\x12",1, ptmp,64);		
			if (flag == 0)
			{
				continue;
			}
			unsigned int groupid = ntohl(*(unsigned int*)flag);
			
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x26",1, ptmp,16);		
			if (flag == 0)
			{
				continue;
			}
			char groupname[256];
			unsigned int len = *(unsigned char *)flag;
			flag ++;
			memmove(groupname, flag, len);
			Utf8ToGB2312(groupname,len);
			
			flag += len;
			ptmp = flag;
			flag = mystrstr("\x36",1, ptmp, 64);	
			if (flag == 0)
			{
				continue;
			}
			
			unsigned int owner = 0;
			char description[256];
			if (*flag != 0x42 && *flag != 0x43)
			{
				len = *(unsigned char*)flag;
				flag ++;
				memmove(description, flag, len);
				Utf8ToGB2312(description,len);
				flag += len;
			}
			
			if (*flag == 0x42)
			{
				flag ++;
				owner =  ntohl(*(unsigned int*)flag);
				flag += 4;
			}
			else if ( *flag == 0x43)
			{
				flag ++;
				owner =  ntohl(*(unsigned int*)(flag + 4));
				flag += 8;
			}

			ultoa(groupid,mqq->state.groupid,10);
			strcpy(mqq->state.groupname,groupname);
			//sprintf(mqq->state.msg, "群号:%u 群名称:%s 群签名:%s 群主:%u",groupid,groupname, description,owner);
			mqq->writefile();
			ptmp = flag;
			cnt ++;
		}

		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"groupinfo error\r\n");
		return FALSE;
	}
}


BOOL richsig(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("KQQ.GetRichSigRes",strlen("KQQ.GetRichSigRes"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			ptmp = mystrstr("GetRichSigReq",strlen("GetRichSigReq"),(char*)pData,datalen);
			if (ptmp == 0)
			{
				return FALSE;
			}	
		}
		
		int cnt = 0;
		char * flag = ptmp;
		while(ptmp < (char*)pData + datalen)
		{
			flag = mystrstr("\x10\x01\x22",3, ptmp,datalen - (ptmp - (char*)pData));	
			if (flag == 0)
			{
				flag = mystrstr("\x10\x01\x23",3, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}
				flag += 4;
			}
			
			unsigned int friendqq = ntohl(*(unsigned int*)flag);
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x4d\x00",2, ptmp,64);	
			if (flag == 0)
			{
				break;
			}

			if (*flag == 1 && *(flag +1) == 0)
			{
				flag += 2;
				flag += 2;
			}
			else if (*flag == 0)
			{
				flag += 3;
			}
			else
			{
				ptmp = flag;
				continue;
			}
			
			char sig[NAME_LEN];
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(sig, flag, len);
			Utf8ToGB2312(sig,len);
			flag += len;
			ptmp = flag;
			sprintf(mqq->state.to,"QQ号码:%u 昵称:%s 签名:(%s)",friendqq,0,sig);
			mqq->writefile();
			cnt ++;
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"richsig error\r\n");
		return FALSE;
	}
}





BOOL getgroup(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = (char*)pData;
		
		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x08\x05\x32",3, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				return FALSE;
			}

			ptmp = flag;
			flag = mystrstr("\x08",1, ptmp,64);		
			if (flag == 0)
			{
				return FALSE;
			}

			ptmp = flag;
			flag = mystrstr("\x12",1, ptmp,64);		
			if (flag == 0)
			{
				return FALSE;
			}
			
			char groupname[NAME_LEN] = {0};
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(mqq->state.groupname, flag, len);
			Utf8ToGB2312(mqq->state.groupname,len);
			flag += len;
			ptmp = flag;

			mqq->state.type = MOBILE_QQ_RECOMMENDLIST;
			mqq->writefile();
			cnt ++;
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"getgroup error\r\n");
		return FALSE;
	}
}






BOOL chkupdate(BYTE * pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("KQQ.CheckUpdateResp",strlen("KQQ.CheckUpdateResp"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}
		
		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x08\x08\x4a",3, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				break;
			}
			ptmp = flag;
			flag = mystrstr("\x0a",1, ptmp,64);		
			if (flag == 0)
			{
				break;
			}
			if (memcmp(flag,"\x8a\x01",2) == 0)
			{
				flag += 2;
			}
			else
			{
				flag ++;
			}
			
			ptmp = flag;

			unsigned int friendqq = ntohl(*(unsigned int*)flag);
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x00",1, ptmp,64);		
			if (flag == 0)
			{
				break;
			}
			flag += 2;

			char sig[256] = {0};
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(sig, flag, len);
			Utf8ToGB2312(sig,len);
			flag += len;
			ptmp = flag;
			sprintf(mqq->state.to,"QQ号码:%u 昵称:%s 签名:(%s)",friendqq,0,sig);
			mqq->writefile();
			cnt ++;
		}

		int result = getgroup(pData,datalen,mqq);
		return TRUE;	
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"chkupdate error\r\n");
		return FALSE;
	}
}







BOOL signature(BYTE* pData, int datalen, mobileqq * mqq)
{
	try
	{
		char * ptmp = mystrstr("map<int64,string>",strlen("map<int64,string>"),(char*)pData,datalen);
		if(ptmp == 0)
		{
			return FALSE;
		}
		
		char * flag = mystrstr("\x08\x00",2, ptmp,64);		
		if (flag == 0)
		{
			return FALSE;
		}
		ptmp = flag;

		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			flag = mystrstr("\x02",1, ptmp,64);		
			if (flag == 0)
			{
				flag = mystrstr("\x03",1, ptmp,64);		
				if (flag == 0)
				{
					break;
				}
				flag += 4;
			}
			unsigned int friendqq = ntohl(*(unsigned int*)flag);
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x16",1, ptmp,64);		
			if (flag == 0)
			{
				break;
			}
			char sig[NAME_LEN] = {0};
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(sig, flag, len);
			Utf8ToGB2312(sig,len);
			flag += len;
			ptmp = flag;
			sprintf(mqq->state.to,"QQ号码:%u 昵称:%s 签名:(%s)",friendqq,0,sig);
			mqq->writefile();
			cnt ++;
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"signature error\r\n");
		return FALSE;
	}
}



BOOL gettroop(BYTE * pData, int datalen,mobileqq * mqq)
{
	try
	{
		char * ptmp = (char*)pData;

		char * flag = mystrstr("\x0a\x03\x00\x00\x00\x00",6, ptmp, 64);		
		if (flag == 0)
		{
			flag = mystrstr("\x0a\x02",2, ptmp, datalen);
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp, datalen);
				if (flag == 0)
				{
					return FALSE;
				}
				flag += 4;	
			}	
		}

		flag += 5;
		ptmp = flag;

		
		unsigned int groupid = ntohl(*(unsigned int*)ptmp);
		ultoa(groupid,mqq->state.groupid,10);
		ptmp += 4;
		
		int cnt = 0;
		while(ptmp < (char*)pData + datalen)
		{
			char * flag = mystrstr("\x0a\x02",2, ptmp,datalen - (ptmp - (char*)pData));		
			if (flag == 0)
			{
				flag = mystrstr("\x0a\x03",2, ptmp,datalen - (ptmp - (char*)pData));		
				if (flag == 0)
				{
					break;
				}
				flag += 4;
			}

			unsigned int friendqq = ntohl(*(unsigned int*)flag);
			flag += 4;
			ptmp = flag;
			flag = mystrstr("\x46",1, ptmp,32);	
			if (flag == 0)
			{
				ptmp += 32;
				continue;
			}
			char nick[NAME_LEN] = {0};
			unsigned int len = *(unsigned char *)flag;
			flag += 1;
			memmove(nick, flag, len);
			Utf8ToGB2312(nick,len);
			flag += len;


			ptmp = flag;
			flag = mystrstr("\x86",1, ptmp,32);	
			if (flag == 0)
			{
				ptmp += 32;
				continue;
			}

			len = *(unsigned char *)flag;
			flag += 1;
			memmove(mqq->state.groupname, flag, len);
			Utf8ToGB2312(mqq->state.groupname,len);


			flag += len;
			ptmp = flag;
			ultoa(groupid,mqq->state.groupid,10);

			mqq->writefile();
			cnt ++;
		}
		return TRUE;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"gettroop error\r\n");
		return FALSE;
	}
}




inline void savepwdkey(unsigned int qq, char * conststr)
{
	char * filename = "e:\\netspy\\para\\mobile_qq_key\\mobile_qq_pwdkey.mqqkey";
	HANDLE hfile= CreateFile(filename,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	unsigned long sizehigh = 0;
	unsigned long sizelow = GetFileSize(hfile, &sizehigh);
	if (sizelow == INVALID_FILE_SIZE || sizelow == 0)
	{
		CloseHandle(hfile);
		return ;
	}	
	char * buf = new char [sizelow + 4096];
	unsigned long readcnt = 0;
	unsigned int result = ReadFile(hfile, buf, sizelow, &readcnt, 0);
	if (readcnt != sizelow || result == 0)
	{
		CloseHandle(hfile);
		delete []buf;
		return;
	}
	
	qqpwd * tmpqqpwd = (qqpwd * )buf;
	for (int i = 0; i < sizelow/sizeof(qqpwd); i ++)
	{
		if (qq == tmpqqpwd->qq)
		{
			long movehigh = 0;
			long movelow = (char*)tmpqqpwd - buf + 4 + 16 + 16;
			unsigned int result = SetFilePointer(hfile, movelow, &movehigh,FILE_BEGIN);
			if (result == 0xffffffff)
			{
				CloseHandle(hfile);
				delete []buf;
				return;
			}
			unsigned long writecnt = 0;
			unsigned int cnt = WriteFile(hfile, conststr, 16, &writecnt, 0);
			if (writecnt != 16)
			{
				CloseHandle(hfile);
				delete []buf;
				return;
			}
			break;
		}
		tmpqqpwd ++;
	}
	delete []buf;
	CloseHandle(hfile);
	
	return;
}





inline void savekey(unsigned int qqnum, unsigned int key)
{
	try
	{
		char * filename = "e:\\netspy\\para\\mobile_qq_key\\mobile_qq_key.mqqkey";
		HANDLE hfile= CreateFile(filename,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, 
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);
		if (filename == INVALID_HANDLE_VALUE)
		{
			return;
		}
		
		char tmp[16] = {0};
		*(unsigned int*)tmp = qqnum;
		*(unsigned int*)(tmp + 4) = key;
		long movehigh = 0;
		unsigned int result = SetFilePointer(hfile, 0, &movehigh,FILE_END);
		if (result == 0xffffffff)
		{
			return;
		}
		unsigned long writecnt = 0;
		unsigned int cnt = WriteFile(hfile, tmp, 8, &writecnt, 0);
		if (writecnt != 8)
		{
			//return;
		}
		
		CloseHandle(hfile);
		return;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"savekey error\r\n");
		return ;
	}
}






void mobileqq::writefile()
{
	try
	{
		CString strResult;
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_pImsgMissObj->GetTmpPath();
		//ASSERT(!tmppath.IsEmpty());
		if (tmppath.IsEmpty())
		{
			WriteLog(IMSGPRO,"mobile qq open file path error\r\n");
			return;
		}
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imsg_moicq.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, m_sip, m_dip, rand());

		char  * msgtype;
		switch (state.type)
		{
			case MOBILE_QQ_ONLINE:
			case MOBILE_QQ_SIMPLEINFO:
				msgtype = "上线";
				break;
			case MOBILE_QQ_OFFLINE:
				msgtype = "下线";
				break;
			case MOBILE_QQ_ONLINEPUSH_REQPUSH:
			case MOBILE_QQ_OFFLINEMSG:
			case MOBILE_QQ_PUSHGETMSG:
			case MOBILE_QQ_PUSHGETMSGV2:
			case MOBILE_QQ_PBSENDMSG:
			case MOBILE_QQ_PBGETMSG:
			case MOBILE_QQ_BINDSTATE:
			case MOBILE_QQ_GETROAMMSG:
			case MOBILE_QQ_PBPRXYSVC:
				msgtype = "消息";
				break;
			case MOBILE_QQ_SEND_GROUP_MSG:
			case MOBILE_QQ_GET_GROUP_MSG:
			case MOBILE_QQ_PUSH_GROUP_MSG:
			case MOBILE_QQ_PBGETGROUPMSG:
			case MOBILE_QQ_PBPUSHGROUPMSG:
				msgtype = "群组消息";
				break;

			case MOBILE_QQ_LIST_FRIEND:
			case MOBILE_QQ_SIGNATURE:
			case MOBILE_QQ_RICHSIG:
			case MOBILE_QQ_ONLINE_FRIEND:
			case MOBILE_QQ_CHKUPDATE:
				msgtype = "好友列表";
				break;
			case MOBILE_QQ_LIST_GROUPINFO:
			case MOBILE_QQ_TROOPLIST:
			case MOBILE_QQ_DISCUSSINFO:
			case MOBILE_QQ_GETTROOP:
			case MOBILE_QQ_GETSET:
				msgtype = "群组资料";
				break;


			/*	
			case MOBILE_QQ_RECOMMENDLIST:
			case MOBILE_QQ_CONTACT:
				msgtype = "通讯录";
				writecontact();
				return;
				*/

			default:
				return;
		}

		strResult.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nstate=%s\r\nplatform=%s\r\nfrom=%s\r\nto=%s\r\nqqgroupid=%s\r\nqqgroupname=%s\r\nmsg=%s\r\n", 
		state.qq, state.imei, state.imsi,msgtype, state.ostype, state.from, state.to,state.groupid, state.groupname, state.msg);
		WriteIndexFile("imsg_moicq","imsg_moicq",strResult,g_pImsgMissObj);
		return;
	}
	catch(...)
	{
		WriteLog(IMSGPRO,"mobileqq writefile error\r\n");
		return ;
	}
}







void mobileqq::writecontact(char * UserName, char * UserPhone)
{
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = g_pImsgMissObj->GetTmpPath();
	if (tmppath.IsEmpty())
	{
		WriteLog(IMSGPRO,"writecontact error\r\n");
		return ;
	}
	//ASSERT(!tmppath.IsEmpty());
	m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_TXL_QQTXL.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, m_sip, m_dip, rand());
	CString Txt;
	Txt.Format("user=%s\r\nimei=%s\r\nimsi=%s\r\nnick=%s\r\ncontact=%s\r\n",
	state.qq, state.imei, state.imsi,UserName,UserPhone);
	WriteIndexFile("TXL_QQTXL","TXL_QQTXL",Txt,g_pImsgMissObj);	
	return;
}