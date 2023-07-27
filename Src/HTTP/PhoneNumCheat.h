
#ifndef PHONE_NUM_CHEAT_H_H_H
#define PHONE_NUM_CHEAT_H_H_H

//PNC = PHONE_NUM_CHEAT


BOOL DealPhoneNumCheat(HttpSession *pHttpSession,const char *pPacket);

//第二号码
int IsPNCSecondNumPacket(HttpSession *pHttpSession);
BOOL ProcessPNCSecondNumPacket(HttpSession *pHttpSession,const char * pPacket, int Action);

//变号神器
int IsPNCBHSQpacket(HttpSession *pHttpSession);
BOOL ProcessPNCBHSQpacket(HttpSession *pHttpSession,const char * pPacket, int Action);

//变号王
int IsPNCBHWpacket(HttpSession *pHttpSession);
BOOL ProcessPNCBHWpacket(HttpSession *pHttpSession,const char * pPacket, int Action);


//变声变号
int IsPNCBSBHpacket(HttpSession *pHttpSession);
BOOL ProcessPNCBSBHpacket(HttpSession *pHttpSession,const char * pPacket, int Action);

//变音变号
int IsPNCBYBHpacket(HttpSession *pHttpSession);
BOOL ProcessPNCBYBHpacket(HttpSession *pHttpSession,const char * pPacket, int Action);

//X8
int IsPNCX8packet(HttpSession *pHttpSession);
BOOL ProcessPNCX8packet(HttpSession *pHttpSession,const char * pPacket, int Action);


#endif