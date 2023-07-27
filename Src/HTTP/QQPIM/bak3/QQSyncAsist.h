#ifndef QQSYNCASIST_H_H_H
#define QQSYNCASIST_H_H_H



#include "QQSyncAsistPub.h"







int ProcQQPimPacketStream(DATEHEADER & packh, const char * pPacket, HttpProtocol * HttpPro);

int ProcQQPimHttpUpPacket(HttpSession * pHttpSession,DATEHEADER & packh,const char * pPacket, int Flag);
int GetQQPimClientData(const char * pContent, int ContentLen, LPQQPIMDATA QQPimData);

int	ProcQQPimHttpDownPacket(DATEHEADER & packh,const char * pPacket,int PacketSize,QQSyncAsist * QQsyncAsist, HttpProtocol * HttpPro);
int GetQQPimServerData(DATEHEADER & packh,const char * DecryptBuf, int DecryptBufLen, QQSyncAsist * QQsyncAsist);		
int ProcQQPim_Recycle_RollBack(char * pDataBuf, int pDataBufLen,DATEHEADER &packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);
int ProcQQPimMessage(char * DecryptBuf, int DecryptBufLen,char * DestBuf, int * DestBufLen);

int ProcQQPimSyncPacket(DATEHEADER & packh ,const char * pPacket, int PacketSize,HttpProtocol * HttpPro);	//密钥写在文件中不需要获取参数
int ParseQQpimSyncPacket(char * DecryptBuf,int DecryptBufLen, char ** pDataBuf, unsigned int * pDataBufLen, char * QQ, char * Imei);






#endif


