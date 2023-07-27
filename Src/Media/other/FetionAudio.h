#ifndef FETIONAUDIO_H_H_H
#define FETIONAUDIO_H_H_H






class FetionAudio:public MediaSession
{
public:
	FetionAudio(DATEHEADER& packh, int aT, char * pPacket);
	~FetionAudio();
	virtual ListToFile(void);
	virtual InsertPacket(DATEHEADER& packh,const char *pPacket);
	int static IsFetionAudio(DATEHEADER& packh,const char *pPacket);
	
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