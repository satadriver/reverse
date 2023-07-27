// Uc.h: interface for the Uc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UC_H__7D5BFF04_1A5E_4F5E_A9D9_FFB70F997E59__INCLUDED_)
#define AFX_UC_H__7D5BFF04_1A5E_4F5E_A9D9_FFB70F997E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//*************************************************
//	类名称:			UCIMSession
//	类继承:       	SocketSession
//	描述:			UC聊天的还原类
//	主要函数:		
//	使用事项:		
//**************************************************
class UCIMSession : public SocketSession
{
public:
	UCIMSession(DATEHEADER& packh, int aT);
	virtual ~UCIMSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
private:

	// 打开实体文件
	void OpenRealFile();
	void WriteRealFile(char* pData, int datalen);

	u_short parse_uc( const u_char * byteArray, const size_t byteArrayLen, FILE* pipe, DATEHEADER& packh);
	int outputtext_udp(u_char* text, u_short textlen, FILE* pipe);
	int outputtext_tcp(u_char* text, u_short textlen, FILE* pipe);

};

#pragma pack(1) 

#define UC_BEGIN_TAG          0x01
#define UC_BEGIN_TAG2         0x0302    //webchat only
#define UC_END_TAG2           0x0504    //webchat only
#define UC_END_TAG            0x06      //webchat only
#define UC_UNKNOWN2           0x00010000
#define UC_HEAD_LEN           0x0D      // sizeof(char) + sizeof(short)*2 + sizeof(int)*2

//commonly, cmd code of response is +1 respectively

#define UC_CMD_QUERY          0x003C    // some query.
#define UC_CMD_LOGIN_54       0x0054	// 4.10.625.login
#define UC_CMD_LOGIN_58       0x0058	// 3.90.810,4.00.200,4.01.920.login
#define UC_CMD_LOGIN_5C       0x005C	// 3.80.904.login
#define UC_CMD_LOGIN_5E       0x005E	// 3.50.908.login
#define UC_CMD_LOGIN_62       0x0062	// 3.00.948,3.20.login
#define UC_CMD_LOGIN_6B       0x006B	// 2.40.login
#define UC_CMD_LOGIN_6A       0x006A	// 2.20.006.login
#define UC_CMD_LOGIN_6C       0x019f	// 2.20.006.以后版本login

// ft longmaster's protocol designer, change the login cmdcode almost every
// release version, maybe some login cmdcode left out

#define UC_CMD_CHG_STATUS_68  0x0068	// sometimes change status use 0x0068?
#define UC_CMD_RREQ_FRI       0x0076	// request who has me as friend.
#define UC_CMD_FRI_LIST       0x008B	// return friend list.
#define UC_CMD_TMP_STORE      0x008D	// send to server to store temporarily.
#define UC_CMD_BROADCAST      0x0094	// UC advertisement.
#define UC_CMD_SEARCH         0x0098	// common search ucid.
#define UC_CMD_FRI_STATUS     0x009C	// friend status info.
#define UC_CMD_REDIR          0x00A2	// redirected packet, need decipher again.
#define UC_CMD_KEEP_ALIVE_C8  0x00C8
#define UC_CMD_KEEP_ALIVE_CB  0x00CB
#define UC_CMD_CHG_INFO       0x00D7    // change info
#define UC_CMD_GET_FRI_INFO   0x00EA	// get friend info details.
#define UC_CMD_CHG_STATUS     0x00FE	// change status
#define UC_CMD_TALK_3E9       0x03E9	// 4.01.920, 4.10.625
#define UC_CMD_TALK_487       0x0487	// 2.40 ~ 4.00.200
#define UC_CMD_ADD_FRI        0x0491	// add friend request.

#define UC_STATUS_OFFLINE     0x00
#define UC_STATUS_HIDDEN      0x01
#define UC_STATUS_ONLINE      0x02
#define UC_STATUS_GOAWAY      0x03

#define UC_WEB_UNK1           0x14E4
#define UC_WEB_BROADCAST      0x14F0
#define UC_WEB_UNK2           0x14FA
#define UC_WEB_UNK3           0x14FE
#define UC_WEB_USERIN         0x1505
#define UC_WEB_USEROUT        0x1506
#define UC_WEB_CHAT           0x1507
#define UC_WEB_CHAT2          0x1508
#define UC_WEB_ROOMINFO       0x1519
#define UC_WEB_ROOMLINKS      0x151A
#define UC_WEB_UNK4           0x151B
#define UC_WEB_GETLIST        0x151C
#define UC_WEB_UNK5           0x1520

#define LIBNET_ETH_H           0xe     /* Etherner header:     14 bytes */
#define LIBNET_IP_H            0x14    /* IP header:           20 bytes */
#define LIBNET_TCP_H           0x14    /* TCP header:          20 bytes */
#define LIBNET_UDP_H           0x8     /* UDP header:           8 bytes */


typedef struct uc_header
{
	u_char	begin_tag;    //always 0x01.
	u_short	uc_sn;        //uc packet sequence number.
	u_int	uc_unknown1;  //unknown yet. 
	u_int	uc_unknown2;  //always 0x00010000.
	u_short	uc_cipher_len;//cipher data length.
	u_char	data[1];      //following are enciphered data.
	
}uc_header;

/*
typedef struct uc_text
{
	u_short	text_len;      //len of the data.
	u_char	direction;     //02=S2C 03=C2S
	u_short	uc_cmd;	       //uc cmd code.
//	u_short	version_len;   //version length.
	u_char	data[1];       //version data...
}uc_text;
//UC packet struct after decipher, but I didn't use it.
//
*/

typedef struct uc_webchat_header
{
	u_char	begin_tag;    //always 0x01.
	u_short	begin_tag2;   //always 0x0302.
	u_short	uc_cipher_len;//cipher data length.
	u_short	uc_sn;        //uc packet sequence number.
	u_char	crc;          //maybe a CRC. anyway, we'll ignore it.
	u_char	data[1];      //following are enciphered data.

/*  ...
	u_short *end_tag2;
	u_char  *end_tag;
	u_short *p_cipher_len;
*/
}uc_webchat_header;

/*
typedef struct uc_webchat_text
{
	u_short	data_len;
	u_short	unknown1;
	u_short	room_id;
	u_short	unknown2;	//always 0x0000;

	u_short	sub_len;
	u_short	uc_cmd;
	u_char	data[1];

}uc_webchat_text;
//sina webchat packet struct after decipher, but I didn't use it.
//
*/

typedef struct ip_info
{
	u_int	src_ip;
	u_int	dst_ip;
	u_short	src_port;
	u_short	dst_port;

}ip_info;
#pragma pack()

//*************************************************
//	函数名称:		IsUcPacket
//	描述:			检查一个包是否是UC的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是UC的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsUcPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen > 12)
	{
		if (packh.m_type == TCPPacket)
		{
			if (memcmp(pPacket,"\x01\x02\x03",3) == 0)
			{
				return TRUE;
			}
		}
		else if (packh.m_type == UDPPacket)
		{
			if (memcmp(pPacket,"\x01",1) == 0 && memcmp(pPacket+7,"\x00\x00\x01\x00",4) == 0)
			{
				return TRUE;
			}
		}

	}
	return FALSE;
}



#endif // !defined(AFX_UC_H__7D5BFF04_1A5E_4F5E_A9D9_FFB70F997E59__INCLUDED_)
