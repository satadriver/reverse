
#ifndef PHONE_MEDIA_APP_H_H_H
#define PHONE_MEDIA_APP_H_H_H

BOOL DealPhoneMedia( HttpSession *pHttpSession,const char *pPacket);

//NBA直播
int IsPhoneMediaNBALive(HttpSession *pHttpSession);
BOOL PhoneMediaNBALive( HttpSession *pHttpSession,const char *pPacket );

//爱奇艺
int IsPhoneMediaAiQiYi(HttpSession *pHttpSession);
BOOL PhoneMediaAiQiYi( HttpSession *pHttpSession,const char *pPacket );

//百度音乐
int IsPhoneMediaBaiDuMusic(HttpSession *pHttpSession);
BOOL PhoneMediaBaiDuMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//PPTV
int IsPhoneMediaPPTV(HttpSession *pHttpSession);
BOOL PhoneMediaPPTV( HttpSession *pHttpSession,const char *pPacket ,int Value);

//天天动听
int IsPhoneMediaTianTianPlay(HttpSession *pHttpSession);
BOOL PhoneMediaTianTianPlay( HttpSession *pHttpSession,const char *pPacket ,int Value);

//合拍校园
int IsPhoneMediaHePaiSchool(HttpSession *pHttpSession);
BOOL PhoneMediaHePaiSchool( HttpSession *pHttpSession,const char *pPacket ,int Value);

//新华电视
int IsPhoneMediaXinHuaTv(HttpSession *pHttpSession);
BOOL PhoneMediaXinHuaTv( HttpSession *pHttpSession,const char *pPacket ,int Value);

//优酷
int IsPhoneMediaYouKu(HttpSession *pHttpSession);
BOOL PhoneMediaYouKu( HttpSession *pHttpSession,const char *pPacket );

//开讯视频
int IsPhoneMediaKaiXunVideo(HttpSession *pHttpSession);
BOOL PhoneMediaKaiXunVideo( HttpSession *pHttpSession,const char *pPacket ,int Value);

//酷我音乐
int IsPhoneMediaKWMusic(HttpSession *pHttpSession);
BOOL PhoneMediaKWMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//QQ音乐
int IsPhoneMediaQQMusic(HttpSession *pHttpSession);
BOOL PhoneMediaQQMusic( HttpSession *pHttpSession,const char *pPacket ,int Value);

//考拉FM
int IsPhoneMediaKaoLaFM(HttpSession *pHttpSession);
BOOL PhoneMediaKaoLaFM(HttpSession *pHttpSession,const char *pPacket,int Value);



#endif