#if !defined(AFX_MOBILEQQ2012_H__9086B4F2_5614_485C_BD0F_9A707D15CFC0__INCLUDED_)
#define AFX_MOBILEQQ2012_H__9086B4F2_5614_485C_BD0F_9A707D15CFC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"
#include "..\\LimitPara.h"


#define MOBILE_QQ_AUTHKEY						0x01
#define MOBILE_QQ_ONLINE						0x02	
#define MOBILE_QQ_OFFLINE						0x03
#define MOBILE_QQ_ONLINEPUSH_REQPUSH			0x04
#define MOBILE_QQ_PBPUSHGROUPMSG				0x05
#define MOBILE_QQ_PBGETGROUPMSG					0x06
#define MOBILE_QQ_OFFLINEMSG					0x07
#define MOBILE_QQ_PUSHGETMSG					0x08
#define MOBILE_QQ_PUSHGETMSGV2					0x09
#define MOBILE_QQ_PUSHGETMSGV4					0x0a
#define MOBILE_QQ_PBPRXYSVC						0x0b
#define MOBILE_QQ_SIMPLEINFO					0x0c
#define MOBILE_QQ_GETSET						0x0d
#define MOBILE_QQ_PUSH_GROUP_MSG				0x17
#define MOBILE_QQ_GET_GROUP_MSG					0x18
#define MOBILE_QQ_SEND_GROUP_MSG				0x19
#define MOBILE_QQ_LIST_FRIEND					0x1a
#define MOBILE_QQ_ONLINE_FRIEND					0x1b
#define MOBILE_QQ_CONTACT						0x1c
#define MOBILE_QQ_BINDSTATE						0x1d
#define MOBILE_QQ_RECOMMENDLIST					0x1e
#define MOBILE_QQ_LIST_GROUPINFO				0x1f
#define MOBILE_QQ_DISCUSSINFO					0x20
#define MOBILE_QQ_TROOPLIST						0x21
#define MOBILE_QQ_PBSENDMSG						0x22
#define MOBILE_QQ_PBGETMSG						0x23
#define MOBILE_QQ_SIGNATURE						0x24
#define MOBILE_QQ_RICHSIG						0x25
#define MOBILE_QQ_GETROAMMSG					0x26
#define MOBILE_QQ_GETTROOP						0x27
#define MOBILE_QQ_CHKUPDATE						0x28
#define MOBILE_QQ_OTHER							0xffffffff

#define MOBILEQQ_PROTOCOLHTTP					1
#define MOBILEQQ_PROTOCOLIMSG					2

#define MOBILE_QQ_CRYPT_FIXED					2
#define MOBILE_QQ_CRYPT_AUTO					1
#define MOBILE_QQ_CRYPT_NONE					0

#define PACKET_FILTER_BUFFER_SIZE				16
#define MOBILEQQ_SERVER_BUFFER_SIZE				0x6000
#define MOBILEQQ_CLIENT_BUFFER_SIZE				0x1000
#define QQ_IMEI_IMSI_LEN						16
#define	NAME_LEN								256
#define MSG_LEN									4096
#define MAX_SPLIT_COUNTS_IN_ONE_PACKET			8			//friend packet is not like the normal format,is one part
#define MAX_TRANSPORT_UNIT						1500


extern MobileqqpwdkeyMap	g_mobileqqpwdkeymap;
extern MobileqqKeyMap	g_mobileqqkeymap;


struct MOBILEQQSTATE
{
	unsigned int	qqnum;
	char			phone[QQ_IMEI_IMSI_LEN];
	char			qq[QQ_IMEI_IMSI_LEN];
	char			imei[QQ_IMEI_IMSI_LEN];
	char			imsi[QQ_IMEI_IMSI_LEN];
	char			groupid[QQ_IMEI_IMSI_LEN];
	char			groupownerqq[QQ_IMEI_IMSI_LEN];
	char			groupownernick[NAME_LEN];
	char			groupname[NAME_LEN];
	char			nick[NAME_LEN];
	char			from[NAME_LEN];
	char			to[NAME_LEN];
	int				type;
	int				onlineflag;
	char			ostype[QQ_IMEI_IMSI_LEN];
	unsigned char	key[QQ_IMEI_IMSI_LEN];
	char			msg[MSG_LEN];

	unsigned char	version[QQ_IMEI_IMSI_LEN];
	unsigned char	password[QQ_IMEI_IMSI_LEN];
	unsigned char   secondmd5[QQ_IMEI_IMSI_LEN];
	unsigned char	mainkey[QQ_IMEI_IMSI_LEN];
	unsigned char	sessionkey[QQ_IMEI_IMSI_LEN];
};




struct MobileQQFilter
{
	unsigned int lastack;
	unsigned int lastseq;
	unsigned int lastpacksize;
	unsigned int unuse;
};



class mobileqq : public SocketSession
{
public:
	mobileqq(DATEHEADER& packh, int aT,int flag);
	virtual ~mobileqq();
	
	virtual BOOL InsertPacket(DATEHEADER& packh,const char *pPacket);
	BOOL processpacket(DATEHEADER& packh, const char * pPacket,mobileqq * mqq);
	BOOL PacketFilter(DATEHEADER& packh,const char *pPacket);
	void writefile();
	void writecontact(char * UserName, char * UserPhone);

	MOBILEQQSTATE state;
	char  clientbuf[MOBILEQQ_CLIENT_BUFFER_SIZE];
	int clientbufptr;
	char  serverbuf[MOBILEQQ_SERVER_BUFFER_SIZE];
	int serverbufptr;
	MobileQQFilter filterflag[PACKET_FILTER_BUFFER_SIZE];
	MobileQQFilter * filterflagptr;
	unsigned int srcip;
	unsigned int dstip;
	unsigned short srcport;
	unsigned short dstport;
};




int GetQQKeyFromServer(char * data,int datalen, BOOL bc2s,mobileqq * mqq);
int GetQQKeyFromClient(char * data,int datalen, BOOL bc2s,mobileqq * mqq);
char * mystrstr(char Flag[], int FlagLen, char Address[], int Offset);
BOOL getqqstr(char * pdata, int datalen,char * qq);
BOOL getcmdstr(char * pdata, int datalen,char * cmd);
void InitState(mobileqq * mqq);
BOOL analyse(DATEHEADER& packh,const char* pPacket,mobileqq * mqq);
BOOL processpacket(DATEHEADER& packh,const char* pPacket,mobileqq * mqq);
BOOL head(BYTE* phead, int headlen,BYTE * pbody,int bodylen,BOOL bc2s,mobileqq * mqq,int decryptlen);
BOOL getauthkey(BYTE * pdata, int datalen,BOOL b2cs,mobileqq * mqq);
BOOL online(BYTE* pdata, int datalen, BOOL bc2s,mobileqq * mqq);
BOOL friendlist(BYTE* pData, int datalen, mobileqq  *mqq);
BOOL onlinefriend(BYTE* pData, int datalen, mobileqq * mqq);
BOOL onlinereqpush(BYTE* pData, int datalen, mobileqq * mqq);	
BOOL offlinemsg(BYTE* pData, int datalen, mobileqq * mqq);		
BOOL pushgetfriendmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pushgetfriendmsgv2(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pushgetfriendmsgv4(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pbpushgroupmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pbgetgroupmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pbsendmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pbgetmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL groupinfo(BYTE* pData, int datalen, mobileqq * mqq);
BOOL discussinfo(BYTE* pData, int datalen, mobileqq * mqq);
BOOL trooplist(BYTE* pData, int datalen, mobileqq * mqq);
BOOL pushgroupmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL sendgroupmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL getgroupmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL friendmsg(BYTE* pData, int datalen, mobileqq * mqq);
BOOL recommendlist(BYTE * pData, int datalen, mobileqq * mqq);
BOOL contact(BYTE * pData, int datalen, mobileqq * mqq);
BOOL bindstate(BYTE * pData, int datalen, mobileqq * mqq);
BOOL signature(BYTE * pData, int datalen, mobileqq * mqq);
BOOL richsig(BYTE * pData, int datalen, mobileqq * mqq);
BOOL getroammsg(BYTE * pData, int datalen, mobileqq * mqq);
BOOL gettroop(BYTE * pData, int datalen, mobileqq * mqq);
BOOL pbprxysvc(BYTE * pData, int datalen, mobileqq * mqq);
BOOL chkupdate(BYTE * pData, int datalen, mobileqq * mqq);
BOOL getgroup(BYTE * pData, int datalen, mobileqq * mqq);
BOOL simpleinfo(BYTE * pData, int datalen, mobileqq * mqq);
BOOL getset(BYTE * pData, int datalen, mobileqq * mqq);
void savekey(unsigned int qqnum, unsigned int key);
inline void savepwdkey(unsigned int qq, char * conststr);




inline void InitOldMobileQQKey()
{
	WIN32_FIND_DATA finddata;
	char * filefilter  = "e:\\netspy\\para\\mobileqqkey-raw\\";
	char * newfilename = "e:\\netspy\\para\\mobile_qq_key\\mobile_qq_key.mqqkey";
	char oldpath[MAX_PATH];
	char oldfilename[MAX_PATH];
	HANDLE hfindfile ;
	HANDLE hfilenew;
	HANDLE hfileold;

	char * tmp = 0;
	char * ptr = 0;
	char strqq[16]={0};
	unsigned long qq = 0;
	char strkey[16] = {0};
	unsigned long key = 0;

	hfilenew = CreateFile(newfilename, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,0,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if (hfilenew == INVALID_HANDLE_VALUE)
	{
		return;
	}

	//必须保证名字是从0开始的字符串数字
	for (int n = 0; n< 100; n ++)
	{
		memset(oldpath,0,MAX_PATH);
		sprintf(oldpath,"%s%u\\*.ini",filefilter,n);

		hfindfile = FindFirstFile(oldpath, &finddata);
		if (hfindfile == INVALID_HANDLE_VALUE)
		{
			continue;
		}	
		
		do
		{
			sprintf(oldfilename,"%s%u\\%s",filefilter,n,finddata.cFileName);
			hfileold= CreateFile(oldfilename,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, 
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
			if (hfileold == INVALID_HANDLE_VALUE)
			{
				continue;
			}

			unsigned long sizehigh = 0;
			unsigned long sizelow = GetFileSize(hfileold, &sizehigh);
			if (sizelow == INVALID_FILE_SIZE)
			{
				continue ;
			}

			unsigned long readcnt = 0;
			tmp= new char [sizelow + 4096];
			unsigned int result = ReadFile(hfileold, tmp, sizelow, &readcnt, 0);
			if (readcnt != sizelow)
			{
				continue;
			}
			CloseHandle(hfileold);

			int flag = 0;
			ptr = tmp;
			char * hdr;
			char * ender;
			hdr = tmp;
			ender = hdr;
			while (hdr < tmp + sizelow && ender < tmp + sizelow)
			{
				ender = strstr(hdr,"\x01");
				if (ender == 0)
				{
					break;
				}
				memset(strqq, 0 ,16);
				memmove(strqq, hdr, ender - hdr);
				qq = strtoul(strqq, 0, 10);
				
				ender ++;
				hdr = ender;
				ender = strstr(hdr,"\r\n");
				if (ender == 0)
				{
					break;
				}
				memset(strkey, 0 ,16);+
				memmove(strkey, hdr, ender - hdr);
				key = strtoul(strkey, 0, 16);
				ender += 2;
				hdr = ender;


				char qqtmp[32] = {0};
				*(unsigned int*)qqtmp = qq;
				*(unsigned int*)(qqtmp+ 4)= key;
				if (key != 0 && qq != 0)
				{
					long movehigh = 0;
					unsigned int result = SetFilePointer(hfilenew, 0, &movehigh,FILE_END);
					unsigned long writecnt = 0;
					result = WriteFile(hfilenew, qqtmp, 8, &writecnt, 0);
					if (writecnt != 8)
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}	
			delete []tmp;		
		}while (FindNextFile(hfindfile,&finddata));
	}
	CloseHandle(hfilenew);
	FindClose(hfindfile);		
	return;
}





inline void InitMobileQQKey()
{
	char * filename = "e:\\netspy\\para\\mobile_qq_key\\mobile_qq_key.mqqkey";
	HANDLE hfile= CreateFile(filename,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	if (filename == INVALID_HANDLE_VALUE)
	{
		return;
	}

	unsigned long sizehigh = 0;
	unsigned long sizelow = GetFileSize(hfile, &sizehigh);
	if (sizelow == INVALID_FILE_SIZE || sizelow == 0)
	{
		return ;
	}	
	char * buf = new char [sizelow + 4096];

	unsigned long readcnt = 0;
	unsigned int result = ReadFile(hfile, buf, sizelow, &readcnt, 0);
	if (readcnt != sizelow || result == 0)
	{
		return;
	}
	CloseHandle(hfile);
	
	unsigned int qq = 0;
	unsigned int key = 0;
	char * hdr = buf;
	int cnt = 0;

	g_mobileqqkeymap.RemoveAll();
	g_mobileqqkeymap.InitHashTable(0x100000);
	while (hdr < buf + sizelow )
	{
		qq = *(unsigned int*)hdr;
		key =  *(unsigned int*)(hdr + 4);
		hdr += 8;
		g_mobileqqkeymap.SetAt(qq,key);
		cnt ++;
	}
	delete []buf;
	cnt = g_mobileqqkeymap.GetCount();

	char * fname = "e:\\netspy\\para\\mobile_qq_key\\mobile_qq_pwdkey.mqqkey";
	hfile= CreateFileA(fname,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	sizehigh = 0;
	sizelow = GetFileSize(hfile, &sizehigh);
	if (sizelow == INVALID_FILE_SIZE || sizelow == 0)
	{
		return ;
	}	
	buf = new char [sizelow + 4096];
	
	readcnt = 0;
	result = ReadFile(hfile, buf, sizelow, &readcnt, 0);
	if (readcnt != sizelow || result == 0)
	{
		CloseHandle(hfile);
		delete []buf;	
		return;
	}
	CloseHandle(hfile);

	qqpwd * myqqpwd = (qqpwd * )buf;
	cnt = 0;
	g_mobileqqpwdkeymap.RemoveAll();
	g_mobileqqpwdkeymap.InitHashTable(0x100000);
	for (int i = 0; i < sizelow/sizeof(qqpwd); i ++)
	{
		qq = myqqpwd->qq;
		g_mobileqqpwdkeymap.SetAt(qq,myqqpwd);
		myqqpwd ++;
		cnt ++;
	}
	delete []buf;		//核实删除
	cnt = g_mobileqqpwdkeymap.GetCount();
	
	return;
}

//packh.m_datalen is actual length of the stream that had been gotton
//true = check packet ok flase = check packet size error
inline int ParseQQPacketLen(DATEHEADER& packh, const char* pPacket)
{
	try
	{
		//one packet should be cut into less than MAX_SPLIT_COUNTS_IN_ONE_PACKET part
		int flag = 0;
		int TotolLen = 0;
		const char * Packet = pPacket;
		while (pPacket < Packet + packh.m_datalen - 4)
		{
			int TmpLen = ntohl(*(unsigned int*)pPacket);	
			if (TmpLen <= 0 || TmpLen >= MOBILEQQ_SERVER_BUFFER_SIZE)	//must be smaller than the buffer
			{
				return FALSE;
			}
			else if (packh.m_dport == 443 || packh.m_dport == 80 || packh.m_dport == 8080 || packh.m_dport == 14000 )
			{
				if (TmpLen >= MOBILEQQ_CLIENT_BUFFER_SIZE)
				{
					return FALSE;
				}
			}
			TotolLen += TmpLen;
			if (TotolLen == packh.m_datalen)
			{
				if (Packet + packh.m_datalen == pPacket + TmpLen)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else if (flag > MAX_SPLIT_COUNTS_IN_ONE_PACKET)
			{
				return FALSE;
			}
			else if (TmpLen == 0 || flag <= MAX_SPLIT_COUNTS_IN_ONE_PACKET)
			{
				flag += 1;
			}
			pPacket += TmpLen;
		}

		return FALSE;
	}
	catch (...)
	{
		WriteLog("IMSGPRO","mobileqq parse packet lenth error\r\n");
		return FALSE;
	}
}

inline DWORD isMobileQQPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen <= 32)
	{
		return FALSE;
	}

	if(packh.m_dport==80 || packh.m_dport==443 || packh.m_dport==8080 || packh.m_dport==14000 ||		//port must be one of this 4 prots
		packh.m_sport==80 || packh.m_sport==443 || packh.m_sport==8080 || packh.m_sport==14000)		
	{
		int Version = ntohl(*(unsigned int*)(pPacket + 4));								//版本号 3 = 2010 11 = 2016
		if ( Version >= 2 && Version <= 16 )									
		{
			int CryptType = *(pPacket + 8);											//加密方式 0 = not crypted
			if (CryptType == MOBILE_QQ_CRYPT_FIXED || CryptType == MOBILE_QQ_CRYPT_AUTO || CryptType == MOBILE_QQ_CRYPT_NONE)
			{
				//if (memcmp(pPacket + 9, "\x00\x00\x00", 3) == 0)					//偏移头部高位3字节 here is error
				//{
					int HeaderLen = ntohl(*(unsigned int*)pPacket);							//验证长度
					if (HeaderLen == packh.m_datalen)
					{
						return TRUE;
					}
					else if (HeaderLen < packh.m_datalen)
					{
						return ParseQQPacketLen(packh,pPacket);
					}
				//}
			}
		}
	}

	return FALSE;
}

#endif // !defined(AFX_MOBILEQQ2012_H__9086B4F2_5614_485C_BD0F_9A707D15CFC0__INCLUDED_)
