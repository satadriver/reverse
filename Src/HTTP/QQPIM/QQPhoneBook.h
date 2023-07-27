#ifndef QQPHONEBOOK_H
#define QQPHONEBOOK_H

#include "QQSyncAsistPub.h"


int ProcQQPhoneBookVObject(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
						   HttpProtocol * HttpPro);
int ProcPhoneBookVMessage(char * Buf, int BufLen,char * HeaderFlag, char * EnderFlag, QQPIMDATA * QQpimdata,DATEHEADER &packh,
						  HttpProtocol * HttpPro);
void WriteQQPhoneBookIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserPhone,QQPIMDATA * QQpimdata);

void WriteQQPhoneBookMsgIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro, CString UserName, CString UserMsg,QQPIMDATA * QQpimdata);


int ParseQQPhoneBookUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro, QQPIMDATA * QQpimdata);

int ProcQQPhoneBookUpPack(char * Data, int DataLen, DATEHEADER & packh);

int ParseQQPhoneBookDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro, QQPIMDATA * QQpimdata);

int ProcQQPhoneBookDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro);


#endif