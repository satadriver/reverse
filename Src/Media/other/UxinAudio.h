#ifndef UXINAUDIO_H_H_H
#define UXINAUDIO_H_H_H






class UXinAudio:public MediaSession
{
public:
	UXinAudio(DATEHEADER& packh, int aT, char * pPacket);
	~UXinAudio();
	virtual ListToFile(void);
	virtual InsertPacket(DATEHEADER& packh,const char *pPacket);
	int static IsUXinAudio(DATEHEADER& packh,const char *pPacket);
	
	unsigned char 	AudioBuf[0x20000];	//128k
	unsigned int	AudioBufLen;
	unsigned int	PackCnt;
	char 			SenderID[16];
	char 			RecverID[16];

	unsigned int	FlagUid;
	unsigned int	FlagHead;
	unsigned int	FlagEnd;

	unsigned int	SenderIP;
	unsigned short	SenderPort;
	unsigned int	RecverIP;
	unsigned short	RecverPort;
};


#endif