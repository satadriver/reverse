#ifndef VoipTalkBox_H
#define VoipTalkBox_H

#define TALKBOX_MESSAGE			1
#define TALKBOX_AUDIO			2
#define TALKBOX_LOGIN			3
#define TALKBOX_COMPLETE		4
#define TALKBOX_OTHER			5

#define TcpFltCnt				64
#define AUDIOBUF_SIZE			0x80000
#define MESSAGEBUF_SIZE			0x1000

typedef struct  
{
	unsigned int SendSequence;
	unsigned int SenderAck;
	unsigned int PackSize;
	unsigned int PackIndex;
}SENDERINFO,* LPSENDERINFO;



typedef struct
{
	unsigned char * pInputBuf;
	unsigned char * pOutputBuf;
	unsigned int  	uiInputLen;
	unsigned int    uiOutputLen;
	unsigned int	uiFlag;	
	unsigned int	uiCallerID;
	unsigned int	uiCalleeID;
}TALKBOXDATA, *LPTALKBOXDATA;


class VoipTalkBox : public MediaSession
{
public:
	VoipTalkBox(DATEHEADER & packh, int aT,const char * pPacket);
	virtual	~VoipTalkBox();
	virtual int InsertPacket(DATEHEADER & dataheader,const char * pPacket);
	virtual int ListToFile();
	static int IsVoipTalkBox(DATEHEADER& packh,const char *pPacket);
	int static DecodeTalkBoxMessage(unsigned char * DecodeBuf,const unsigned char * EncodeBuf, size_t EncodeSize);
	int ParseTalkBoxPacket( LPTALKBOXDATA pTalkBoxData );
	int ProcLoginPack(DATEHEADER & packh, const char * pPacket);
	int CheckTable(unsigned char TalkboxTable[7][256]);

private:
	unsigned int	m_Opcode;
	unsigned char *	m_MsgBufPtr;
	unsigned char   m_MsgBuf[MESSAGEBUF_SIZE];
	unsigned char * m_SenderAudioBufPtr;
	unsigned char   m_SenderAudioBuf[AUDIOBUF_SIZE];
	unsigned char * m_RecverAudioBufPtr;
	unsigned char   m_RecverAudioBuf[AUDIOBUF_SIZE];
	SENDERINFO *    m_SenderInfoPtr;
	SENDERINFO		m_SenderInfoBuf[TcpFltCnt];

	unsigned int	m_CallerIP;
	unsigned int	m_CalleeIP;
	unsigned short	m_CallerPORT;
	unsigned short	m_CalleePORT;
};

#endif