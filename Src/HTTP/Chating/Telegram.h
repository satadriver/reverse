
#include "..\\src\\http\\http.h"
#include "..\\src\\http\\HttpSession.h"


typedef struct _TELEGRAMINFO
{
	unsigned char AuthKey[8];
	unsigned char MsgId[8];
	unsigned int  Cmd;
	unsigned int  Cnt;
	unsigned long Time;
}TELEGRAMINFO, *LPTELEGRAMINFO;


extern CMap< CString,LPCSTR,TELEGRAMINFO *,TELEGRAMINFO * &>cmapTelegram;

int IsTelegramPacket(DATEHEADER & packh, const char * pPacket);
int ProcessTelegramPacket(DATEHEADER & packh, const char * pPacket, HttpProtocol * pHttpPro);
void WriteTelegramIndexFile(DATEHEADER & packh, HttpProtocol * pHttpPro);


/*
  //lenth		4
  //seqno		4
  //authkey	8
  //msgid		8
  //datalen	4
  //cmd		4
  //random	16
//crc32		4
typedef struct _TELEGRAMFIRPACKET
{
	unsigned int PacketSize;
	unsigned int SeqNum;
	unsigned char AuthKey[8];
	unsigned char MsgId[8];
	unsigned int  DataLen;
	unsigned int  Cmd;
	unsigned char Random[16];
}TELEGRAMFIRSTPACKET, * LPTELEGRAMFIRSTPACKET;
*/


