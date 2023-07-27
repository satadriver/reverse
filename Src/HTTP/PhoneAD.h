
#ifndef PHONE_AD_H_H_H
#define PHONE_AD_H_H_H

#define PHONEAD_UMENG 1
#define PHONEAD_WAPX 2
#define PHONEAD_IBUTIAN 3
#define PHONEAD_MOGO 4
#define PHONEAD_PUATA 5
#define PHONEAD_ADSAGE 6

//友盟开发平台 PHONEAD_UMENG
//万普开发平台 PHONEAD_WAPX

typedef struct _PHONE_AD_INFO
{
	CString strIMEI;
	CString strIMSI;
	CString strModel;
}SPHONE_AD_INFO;

BOOL DealPhoneAd(HttpSession *pHttpSession,const char *pPacket);
int IsPhoneADPacket( HttpSession *pHttpSession);
BOOL PhoneADPacket( HttpSession *pHttpSession,const char *pPacket,int adtype);

#endif