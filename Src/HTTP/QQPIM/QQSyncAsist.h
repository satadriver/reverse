#ifndef QQSYNCASIST_H_H_H
#define QQSYNCASIST_H_H_H



#include "QQSyncAsistPub.h"







int ProcQQPimPacketStream(DATEHEADER & packh, const char * pPacket, HttpProtocol * HttpPro);

int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket, int Flag);
int GetQQPimClientData(const char * pContent, int ContentLen, LPQQPIMDATA QQPimData);

int	ProcQQPimHttpDownPacket(DATEHEADER & packh,const char * pPacket,int PacketSize,QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro);
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist);	
	
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen,DATEHEADER &packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);

int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize,HttpProtocol * HttpPro);	//密钥写在文件中不需要获取参数
int ParseQQpimSyncPacket(char * DecryptBuf,int DecryptBufLen, char ** pDataBuf, unsigned int * pDataBufLen, char * QQ, char * Imei);


int ProcQQPimVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,HttpProtocol * HttpPro);
int ProcQQPimVMessage(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,HttpProtocol * HttpPro);

void WriteQQPimIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata);

void WriteQQPimMsgIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserPhone,CString UserName, CString UserMsg,
							  QQPIMDATA * QQpimdata);

#endif


