#ifndef AQWS360_H_H_H
#define AQWS360_H_H_H

BOOL IsAQWS360DevProcess(const char *pPacket);
BOOL AQWS360DevProcess(const char *pPacket,DATEHEADER& packh ,HttpProtocol* httpprotocl);
#endif