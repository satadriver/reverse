
#ifndef PHONE_MEDIA_APP_H_H_H
#define PHONE_MEDIA_APP_H_H_H

BOOL DealPhoneMedia( HttpSession *pHttpSession,const char *pPacket);

//NBAֱ��
int IsPhoneMediaNBALive(HttpSession *pHttpSession);
BOOL PhoneMediaNBALive( HttpSession *pHttpSession,const char *pPacket );

//������
int IsPhoneMediaAiQiYi(HttpSession *pHttpSession);
BOOL PhoneMediaAiQiYi( HttpSession *pHttpSession,const char *pPacket );

//�ٶ�����
int IsPhoneMediaBaiDuMusic(HttpSession *pHttpSession);
BOOL PhoneMediaBaiDuMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//PPTV
int IsPhoneMediaPPTV(HttpSession *pHttpSession);
BOOL PhoneMediaPPTV( HttpSession *pHttpSession,const char *pPacket ,int Value);

//���춯��
int IsPhoneMediaTianTianPlay(HttpSession *pHttpSession);
BOOL PhoneMediaTianTianPlay( HttpSession *pHttpSession,const char *pPacket ,int Value);

//����У԰
int IsPhoneMediaHePaiSchool(HttpSession *pHttpSession);
BOOL PhoneMediaHePaiSchool( HttpSession *pHttpSession,const char *pPacket ,int Value);

//�»�����
int IsPhoneMediaXinHuaTv(HttpSession *pHttpSession);
BOOL PhoneMediaXinHuaTv( HttpSession *pHttpSession,const char *pPacket ,int Value);

//�ſ�
int IsPhoneMediaYouKu(HttpSession *pHttpSession);
BOOL PhoneMediaYouKu( HttpSession *pHttpSession,const char *pPacket );

//��Ѷ��Ƶ
int IsPhoneMediaKaiXunVideo(HttpSession *pHttpSession);
BOOL PhoneMediaKaiXunVideo( HttpSession *pHttpSession,const char *pPacket ,int Value);

//��������
int IsPhoneMediaKWMusic(HttpSession *pHttpSession);
BOOL PhoneMediaKWMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//QQ����
int IsPhoneMediaQQMusic(HttpSession *pHttpSession);
BOOL PhoneMediaQQMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//����FM
int IsPhoneMediaKaoLaFM(HttpSession *pHttpSession);
BOOL PhoneMediaKaoLaFM(HttpSession *pHttpSession,const char *pPacket,int Value);



#endif