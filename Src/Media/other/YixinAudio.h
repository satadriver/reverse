#ifndef YIXINAUDIO_H_H_H
#define YIXINAUDIO_H_H_H






class YixinAudio:public MediaSession
{
public:
	YixinAudio(DATEHEADER& packh, int aT, char * pPacket);
	~YixinAudio();
	virtual ListToFile(void);
	virtual InsertPacket(DATEHEADER& packh,const char *pPacket);
	int static IsYixinAudio(DATEHEADER& packh,const char *pPacket);
	
	unsigned char 	AudioBuf[0x20000];	//128k
	unsigned int	AudioBufLen;
	unsigned int	PackCnt;
	char 			SenderID[256];
	char 			RecverID[256];
	
	unsigned int	FlagUid;
	unsigned int	FlagHead;
	unsigned int	FlagEnd;
	
	unsigned int	SenderIP;
	unsigned short	SenderPort;
	unsigned int	RecverIP;
	unsigned short	RecverPort;
};


#endif