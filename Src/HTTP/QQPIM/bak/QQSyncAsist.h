#ifndef QQSYNCASIST_H_H_H
#define QQSYNCASIST_H_H_H


#include "..\\src\\http\\http.h"
#include "..\\src\\http\\HttpSession.h"
#include "..\\src\\http\\VoipUrl.h"
#include "QQSyncAsistPub.h"


#define ACTION_SYNCCONTACT					0x80
#define ACTION_SYNCMESSAGE					0x81
#define ACTION_SYNCCALL						0x82
#define ACTION_RECYCLE						0x83
#define ACTION_ROLLBACK						0x84
#define ACTION_MESSAGE						0X85
#define ACTION_CALL							0X86
#define ACTION_CONTACT						0X87
#define ACTION_WHERE						0x88

#define QQPIMDATA_ITEM_LENTH				28
#define QQSYNCASIST_DATABUF_LENTH			0x10000		//64K
#define QQPIM_DECRYPTBUF_LENTH				0x10000
	


//#pragma pack(push)		//内存对齐
//#pragma pack(1)
typedef struct		//为了方便文件操作不能写成类
{
	int	 QQLen;
	char QQ[QQPIMDATA_ITEM_LENTH];
	int ImeiLen;
	char Imei[QQPIMDATA_ITEM_LENTH];
	int ImsiLen;
	char Imsi[QQPIMDATA_ITEM_LENTH];
	int QQPimKeyLen;
	unsigned char QQPimKey[QQPIMDATA_ITEM_LENTH];
}QQPIMDATA,*LPQQPIMDATA;
//#pragma pack(pop)








class QQSyncAsist
{
public:
	QQPIMDATA	QQPimData;
	char 		DataBuf[QQSYNCASIST_DATABUF_LENTH];
	int			DataLen;
	int			ContentLen;
	time_t		Time;
	QQSyncAsist();
	~QQSyncAsist();
};




extern CMap< CString,LPCSTR,QQPIMDATA*,QQPIMDATA*& >cmapQQPimData;
extern int cmapQQPimDataCnt;
extern CMap< CString,LPCSTR,QQSyncAsist*,QQSyncAsist*& >cmapQQSyncAsist;
extern int cmapQQSyncAsistCnt;



int ProcQQPimPacketStream(DATEHEADER & packh, const char * pPacket, HttpProtocol * HttpPro);

int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket);
int GetQQPimClientData(const char * pContent, int ContentLen, LPQQPIMDATA QQPimData);

int	ProcQQPimHttpDownPacket(DATEHEADER & packh,const char * pPacket,int PacketSize,QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro);
int GetQQPimServerData(const char * DecryptBuf, int DecryptBufLen, char * DestBuf, int * DestBufLen,QQSyncAsist * QQsyncAsist);		//写参数到文件中
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen, char * DestBuf, int * DestBufLen);

int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize,HttpProtocol * HttpPro);	//密钥写在文件中不需要获取参数
int ParseQQpimSyncPacket(char * DecryptBuf,int DecryptBufLen, char ** pDataBuf, unsigned int * pDataBufLen, char * QQ, char * Imei);

int ProcVcardFile(char * Buf, int BufLen, char * DestBuf, int * DestLen);

void WriteQQPimIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, char * Data,QQPIMDATA * QQpimdata);

int ProcVObject(char * Buf, int BufLen, char * DestBuf, int * DestLen, char * HeaderFlag, char * EnderFlag, int Offset);
char * FindMemory(char Flag[], int FlagLen, char Address[], int Offset);
#endif


