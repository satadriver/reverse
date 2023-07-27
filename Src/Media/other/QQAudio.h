#ifndef QQAUDIO_H_H_H
#define QQAUDIO_H_H_H






class QQAudio:public MediaSession
{
public:
	QQAudio(DATEHEADER& packh, int aT, char * pPacket);
	~QQAudio();
	virtual ListToFile(void);
	virtual InsertPacket(DATEHEADER& packh,const char *pPacket);
	int static IsQQAudio(DATEHEADER& packh,const char *pPacket);
	
	unsigned char 	AudioBuf[0x20000];	//128k
	unsigned int	AudioBufLen;
	unsigned int	FlagUid;
	unsigned int	FlagHead;
	unsigned int	FlagEnd;
	unsigned int	PackCnt;
	char 			SenderQQ[16];
	char 			RecverQQ[16];
	
	unsigned int	SenderIP;
	unsigned short	SenderPort;
	unsigned int	RecverIP;
	unsigned short	RecverPort;
};


#endif