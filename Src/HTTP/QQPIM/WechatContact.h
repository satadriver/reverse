#ifndef WECHATCONTACT_H
#define WECHATCONTACT_H

#include "QQSyncAsistPub.h"


//���û��IMSI�򲻴�ӡ���
void WriteWechatContctIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata);

int ProcWechatContactVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
						   HttpProtocol * HttpPro);

int ParseWechatContactUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);

int ParseWechatContactDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro,QQPIMDATA * QQpimdata);

//Ҫ����POST�����
int ProcWechatContactUpPack(char * Data, int DataSize,DATEHEADER & packh);

//QQͨѶ¼ͬ�����ݰ�û�й�����Կ ���ݰ��ڽ���ǰ���ܻ��QQ�����IMEI ����ֻ����SOCKET����ʾ �������ݰ��Ҳ�����Կ����ʧ��
int ProcWechatContactDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro);

#endif