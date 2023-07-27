#ifndef		QQSYNCASISTPUB_H
#define 	QQSYNCASISTPUB_H


#include "..\\src\\http\\http.h"
#include "..\\src\\http\\HttpSession.h"
#include "..\\src\\http\\VoipUrl.h"
#include "defs.h"





#define QQPIM								1
#define QQPHONEBOOK							2
#define QQPHONEBOOK_DATA					3

#define ACTION_SYNCCONTACT					0x80
#define ACTION_SYNCMESSAGE					0x81
#define ACTION_SYNCCALL						0x82
#define ACTION_RECYCLE						0x83
#define ACTION_ROLLBACK						0x84
#define ACTION_MESSAGE						0X85
#define ACTION_CALL							0X86
#define ACTION_CONTACT						0X87
#define ACTION_WHERE						0x88

#define QQPIMDATA_ITEM_LEN					28
#define QQSYNCASIST_DATABUF_LEN				0x10000		//64K
#define QQSYNCASIST_UPDATABUF_LEN			0x10000
#define QQPIM_DECRYPTBUF_LEN				0x10000
#define QQPIM_TIMEOUT		120
#define VCARD_MAX_SIZE		256
#define VMESSAGE_MAX_SIZE	2048
#define VCALL_MAX_SIZE		256



//#pragma pack(push)		//内存对齐
//#pragma pack(1)
typedef struct		//为了方便文件操作不能写成类
{
	int	 QQLen;
	char QQ[QQPIMDATA_ITEM_LEN];
	int ImeiLen;
	char Imei[QQPIMDATA_ITEM_LEN];
	int ImsiLen;
	char Imsi[QQPIMDATA_ITEM_LEN];
	int QQPimKeyLen;
	unsigned char QQPimKey[QQPIMDATA_ITEM_LEN];
}QQPIMDATA,*LPQQPIMDATA;
//#pragma pack(pop)

void InitQQPimData(LPQQPIMDATA QQPimData);






class QQSyncAsist
{
public:
	QQPIMDATA	QQPimData;
	char 		DataBuf[QQSYNCASIST_DATABUF_LEN];
	int			DataLen;
	int			ContentLen;
	char		UpDataBuf[QQSYNCASIST_UPDATABUF_LEN];
	int			UpDataLen;
	time_t		Time;
	int			Flag;
	
	QQSyncAsist();
	QQSyncAsist(int Flag);
	~QQSyncAsist();
};




extern CMap< CString,LPCSTR,QQPIMDATA*,QQPIMDATA*& >cmapQQPimData;
extern int cmapQQPimDataCnt;
extern CMap< CString,LPCSTR,QQSyncAsist*,QQSyncAsist*& >cmapQQSyncAsist;
extern int cmapQQSyncAsistCnt;




void __cdecl New_sub_16f4();
int __fastcall sub_144C0(int a1, int a2);									// 52, packet size
int __fastcall sub_144C8(int result, unsigned int a2);
int __fastcall sub_B764(unsigned int a1, unsigned int a2, int a3);			//a1 = key  a2 = 64, a3 = int [] return by c160
int __fastcall sub_C160(int result);
//将原始密钥传入C160返回的数组中的第28开始的字节中
void *__fastcall sub_C190(void *result, int a2, unsigned int a3);			//int[] return by c160 ,key,keylen
//低位是C160 C190制造的密钥，高位指向一个整数 a2是一个整数
__int64 __fastcall sub_C250(__int64 a1, int a2)	;							//完成密钥的生成和复制，参数为两个地址
int __fastcall sub_C2C4(const void *a1, unsigned int a2, void *a3)	;		//a1== key, a2 == keylen, a3 == int[] 
unsigned int __fastcall sub_C340(void *src, size_t n, int a3, int a4, void *dest, int a6);
unsigned int __fastcall sub_C49C(void *a1, int a2, int a3, int a4, void *a5, int a6);	
//src == dest == packet, a2== a6 == pack len, a3 = key, a4 = keylen
int __fastcall sub_C4D8(void *src, int a2, int a3, int a4, void *dest, int a6);	
int __fastcall sub_C62C(void *src, int a2, int a3, int a4, void *dest, int a6);
int  __fastcall sub_183C(char * str,int strlen,char * key, int keylen, char * decbuf, int decbuflen,int Flag);

int AscToHex(unsigned char * Asc,int AscLen, unsigned char * Hex);

int ProcVObject(char * Buf, int BufLen, char * DestBuf, int * DestLen, char * HeaderFlag, char * EnderFlag, int Offset);
int ProcQQPhoneBookVobject(char * Buf, int BufLen, char * DestBuf, int * DestLen, char * HeaderFlag, char * EnderFlag, int Offset);
char * FindMemory(char Flag[], int FlagLen, char Address[], int Offset);
void WriteQQPimIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, char * Data,QQPIMDATA * QQpimdata);

int GetIPPort(unsigned char * IPPort, unsigned char * IP, unsigned short * Port);

int ProcVMessage(char * Buf, int BufLen, char * DestBuf, int * DestLen);


CString PreVCARD(char * Buf, int BufLen);
#endif