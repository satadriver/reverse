// Decode.h: interface for the Decode class.
//				
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_)
#define AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/************************************************************************/
/*    ��http �е�uid pass      ����������                               */
/************************************************************************/
 
//���Ա�http post��ȥ��ú���״̬��  uid , base64������
extern BOOL Dec_TaoBao_Uid(unsigned char* buf ,int buflen);
//���Ա� http ��½ʱ������
extern BOOL Dec_TaoBao_Pass(unsigned char* buf,unsigned char* outbuf,int buflen);

/************************************************************************/
/*���������� keyΪmd5ǰ8λ,�������¼���Զ����������key:"\x19\x76\x12\x27\x13\x4f\x0b\x1e"  */
//���� 18nnad7f1njdy7f23nadifu23djfdu+�����  md5��ǰ8λ
/************************************************************************/
//����des��ʼ��key
extern void Bian_Des_SetKey(char* key);
//taobao �����ı�����
extern BOOL Bian_Dec_TaoBao(UCHAR *buf,int datalen);
  
/************************************************************************/
/* ���������¼��sqllite�����ļ�   */
/************************************************************************/
//��������ͨ��0x400Ϊһ�����ݿ���н���ģ���1024�ֽ�
void DeCode(const int m/*����*/,const int n,/*����*/char *buf,
      int bufSize,int fOffset,const int l=0/*����*/);

#endif // !defined(AFX_DECODE_H__3F4DD951_398E_4A94_8350_7FC22F731C8C__INCLUDED_)
