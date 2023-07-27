#ifndef WECHATCONTACT_H
#define WECHATCONTACT_H

#include "QQSyncAsistPub.h"


//如果没有IMSI则不打印输出
void WriteWechatContctIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata);

int ProcWechatContactVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
						   HttpProtocol * HttpPro);

int ParseWechatContactUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);

int ParseWechatContactDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);

//要放在POST多包处
int ProcWechatContactUpPack(char * Data, int DataSize,DATEHEADER & packh);

//QQ通讯录同步数据包没有公共密钥 数据包在解密前不能获得QQ号码和IMEI 所以只能用SOCKET来标示 否则数据包找不到密钥解密失败
int ProcWechatContactDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro);

#endif