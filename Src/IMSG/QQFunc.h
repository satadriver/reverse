// PubFunc.h: interface for the CPubFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBFUNC_H__1BE7D383_1114_428D_9B3A_472DED9AE1FC__INCLUDED_)
#define AFX_PUBFUNC_H__1BE7D383_1114_428D_9B3A_472DED9AE1FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "oicq.h"
//////////////////////////////////////////////////////////////////////////
//
#define KB(a) a*1024
#define PACKET_SIZE KB(4)
#define	MSG_CONTENT_LEN KB(4)
typedef struct bytebuffer{
	int pos;	///< 当前位置
	int len;	///< 已有缓冲区数据, 读取用
	int size;	///< 缓冲区大小, 写入用
	unsigned char data[PACKET_SIZE];
}bytebuffer;

#define TOKEN_LEN 256		//256
typedef struct token{
	short	len;
	unsigned char	data[TOKEN_LEN];
}token;

int		get_token2( bytebuffer* p, token* tok );
int		get_token( bytebuffer* p, token* tok );
int		get_data( bytebuffer* p, unsigned char* data, int len );
int		get_string( bytebuffer* p, char* str, int len );
unsigned int	get_int( bytebuffer* p );
unsigned short	get_word( bytebuffer* p );
unsigned char	get_byte( bytebuffer* p );

typedef struct qq_udp_header
{
    u_char    begin_tag;    //always 0x02 till now
    u_short   sender_ver;   //client version, details below
    u_short   qq_cmd;       //command code, QQ_CMD_XXX 
    u_short   sq_no;        //sequence number.
}qq_udp_header;

typedef struct qq_login_data
{
    u_int     qq_no;                 //qq number.
    u_char    key[LEN_TEA_KEY];      //random key before, but now '0x01'x16 for udp, for server efficiency?tcp 443 still random
    u_char    data[1];               //often const in udp 0x80, tcp 0xA0 
}qq_login_data;

typedef struct qq_friend_data
{
    u_int		qq_no;                 //qq number.
	u_short		face;
	u_char		age;
	u_char		sex;
	u_char		lenNicename;
	u_char		nicename[256];
	u_int		flg;
}qq_friend_data;

typedef struct FriendStatus
{
    u_int		qq_no;                 //qq number.
	u_char		unknow1;
	u_int		ip;
	u_short		port;
	u_char		unknow2;
	u_char		status;
	u_short		unknow3;
	u_char		unknowkey[16];
}FriendStatus;

typedef struct ReceiveIMHeader
{
    u_int sender;
    u_int receiver;
    u_int sequence;
    u_char senderIp[4];
    u_short senderPort;
    u_short type; 
}ReceiveIMHeader;

// 输出信息
void OutMessage(OicqSession *s,	DWORD qqcmd,
				unsigned char *pQQData, const int Datalen,
				char* msgbuf,	int& msglen,
				BOOL bC2S, time_t tmPacket);
#endif // !defined(AFX_PUBFUNC_H__1BE7D383_1114_428D_9B3A_472DED9AE1FC__INCLUDED_)
