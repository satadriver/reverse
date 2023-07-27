// Decode.h: interface for the Decode class.
//				
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_)
#define AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/************************************************************************/
/*    解http 中的uid pass      密码已内置                               */
/************************************************************************/
 
//解淘宝http post上去获得好友状态中  uid , base64解码后的
extern BOOL Dec_TaoBao_Uid(unsigned char* buf ,int buflen);
//解淘宝 http 登陆时的密码
extern BOOL Dec_TaoBao_Pass(unsigned char* buf,unsigned char* outbuf,int buflen);

/************************************************************************/
/*解聊天内容 key为md5前8位,解聊天记录中自动保存密码的key:"\x19\x76\x12\x27\x13\x4f\x0b\x1e"  */
//采用 18nnad7f1njdy7f23nadifu23djfdu+密码的  md5的前8位
/************************************************************************/
//变形des初始化key
extern void Bian_Des_SetKey(char* key);
//taobao 旺旺文本解码
extern BOOL Bian_Dec_TaoBao(UCHAR *buf,int datalen);
  
/************************************************************************/
/* 解密聊天记录的sqllite数据文件   */
/************************************************************************/
//旺旺都是通过0x400为一块数据块进行解码的，即1024字节
void DeCode(const int m/*保留*/,const int n,/*保留*/char *buf,
      int bufSize,int fOffset,const int l=0/*保留*/);

#endif // !defined(AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_)
