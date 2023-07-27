

#ifndef VOIP_CALL_AND_MESSAGE_H_H_H
#define VOIP_CALL_AND_MESSAGE_H_H_H



BOOL IsVoipALITONGMessage(HttpSession *pHttpSession);
BOOL VoipALITONGMessage( HttpSession *pHttpSession ,const char *pPacket,int Value);

BOOL IsVoipZHCALLMessage(HttpSession * pHttpSession);
BOOL VoipZHCALLMessage( HttpSession * pHttpSession ,const char *pPacket);

BOOL IsVoip97CALLMessage(HttpSession * pHttpSession);
BOOL Voip97CALLMessage( HttpSession * pHttpSession ,const char *pPacket);

BOOL IsVoipDYTMessage(HttpSession * pHttpSession);
BOOL VoipDYTMessage( HttpSession * pHttpSession,const char *pPacket);

//////////////////////////////////////////////////////////////////////////

#define VOIP_LOGIN 1
#define VOIP_CALL 2
#define VOIP_PHONE_LOGIN 3
#define VOIP_PHONE_CALL 4
#define VOIP_MSG 5
#define VOIP_PHONE_MSG 6
#define VOIP_REG 7
#define VOIP_PHONE_REG 8
#define VOIP_FRIEND 9
#define VOIP_PHONE_FRIEND 9

BOOL DealVoipCall(HttpSession *pHttpSession,const char* pPacket);

int IsVoipYMCALLCall(HttpSession *pHttpSession);
BOOL VoipYMCALLCall( HttpSession *pHttpSession,const char* pPacket,int Value = 0);

int IsVoip97CALLCall(HttpSession *pHttpSession);
BOOL Voip97CALLCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipDACALLCall(HttpSession *pHttpSessiono);
BOOL VoipDACALLCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipKUAIBOCall(HttpSession *pHttpSession);
BOOL VoipKUAIBOCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipKUBAOCall(HttpSession *pHttpSession);
BOOL VoipKUBAOCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipALICALLCall(HttpSession *pHttpSession);
BOOL VoipALICALLCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsZHCALLCall(HttpSession *pHttpSession);
BOOL ZHCALLCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipDYTCall(HttpSession * pHttpSession);
BOOL VoipDYTCall( HttpSession * pHttpSession,const char *pPacket,int Value = 0);

int IsVoipKcCall(HttpSession * pHttpSession);
int VoipKcCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipSKYCall(HttpSession * pHttpSession);

int IsVoipTUXINCall(HttpSession * pHttpSession);
int VoipTUXINCall(HttpSession *pHttpSession,const char *pPacket,int Value);

int IsVoipFEIYINCall(HttpSession * pHttpSession);
int VoipFEIYINCall(HttpSession * pHttpSession,const char * pPacket,int Value);

int IsVoipFEIYUCall(HttpSession * pHttpSession);
int VoipFEIYUCall(HttpSession * pHttpSession,const char * pPacket,int Value);

int IsVoipYLTCall(HttpSession * pHttpSession);

int IsVoipADLCall(HttpSession * pHttpSession);

int IsVoipZSBCall(HttpSession * pHttpSession);

int IsVoipYOUXINCall(HttpSession *pHttpSession);
BOOL VoipYOUXINCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipRYBCall(HttpSession * pHttpSession);

int IsVoipWEIWEICall(HttpSession * pHttpSession);

int IsVoip66CALLCall(HttpSession *pHttpSession);
BOOL Voip66CALLCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0 );

int IsVoipHHTCall(HttpSession *pHttpSession);

int IsVoipYUNTONGCall(HttpSession *pHttpSession);
BOOL VoipYUNTONGCall( HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipYUNHUCall(HttpSession *pHttpSession);
BOOL VoipYUNHUCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0);

int IsVoipYUNHUACall(HttpSession *pHttpSession);
BOOL VoipYUNHUACall(HttpSession *pHttpSession,const char *pPacket,int Value = 0 );

int IsVoipHHCallCall(HttpSession *pHttpSession);
BOOL VoipHHCallCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0 );

int IsVoipAiLiaoCall(HttpSession *pHttpSession);
BOOL VoipAiLiaoCall(HttpSession *pHttpSession,const char *pPacket,int Value = 0 );

int IsVoipTTCall(HttpSession *pHttpSession);
int VoipTTCall( HttpSession *pHttpSession,const char* pPacket,int Value);



#endif