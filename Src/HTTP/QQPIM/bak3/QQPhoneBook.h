#ifndef QQPHONEBOOK_H
#define QQPHONEBOOK_H

#include "QQSyncAsistPub.h"




int ParseQQPhoneBookUpPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro, QQPIMDATA * QQpimdata);

int ProcQQPhoneBookUpPack(char * Data, int DataLen, DATEHEADER & packh);

int ParseQQPhoneBookDownPack(char * Data, int DataLen, DATEHEADER & packh,HttpProtocol * HttpPro, QQPIMDATA * QQpimdata);

int ProcQQPhoneBookDownPack(DATEHEADER & packh, char * Data, int DataSize, QQSyncAsist * QQsyncasist,HttpProtocol * HttpPro);

int ProcQQPhoneBookChat(DATEHEADER & packh ,const char * pPacket, int PacketSize, HttpProtocol * HttpPro);

#endif