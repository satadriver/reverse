#ifndef		QQSYNCASISTPUB_H
#define 	QQSYNCASISTPUB_H










void __cdecl New_sub_16f4();
int __fastcall sub_144C0(int a1, int a2);									// 52, packet size
int __fastcall sub_144C8(int result, unsigned int a2);
int __fastcall sub_B764(unsigned int a1, unsigned int a2, int a3);			//a1 = key  a2 = 64, a3 = int [] return by c160
int __fastcall sub_C160(int result);
//��ԭʼ��Կ����C160���ص������еĵ�28��ʼ���ֽ���
void *__fastcall sub_C190(void *result, int a2, unsigned int a3);			//int[] return by c160 ,key,keylen
//��λ��C160 C190�������Կ����λָ��һ������ a2��һ������
__int64 __fastcall sub_C250(__int64 a1, int a2)	;							//�����Կ�����ɺ͸��ƣ�����Ϊ������ַ
int __fastcall sub_C2C4(const void *a1, unsigned int a2, void *a3)	;		//a1== key, a2 == keylen, a3 == int[] 
unsigned int __fastcall sub_C340(void *src, size_t n, int a3, int a4, void *dest, int a6);
unsigned int __fastcall sub_C49C(void *a1, int a2, int a3, int a4, void *a5, int a6);	
//src == dest == packet, a2== a6 == pack len, a3 = key, a4 = keylen
int __fastcall sub_C4D8(void *src, int a2, int a3, int a4, void *dest, int a6);	
int __fastcall sub_C62C(void *src, int a2, int a3, int a4, void *dest, int a6);
int  __fastcall sub_183C(char * str,int strlen,char * key, int keylen, char * decbuf, int decbuflen,int Flag);

int AscToHex(unsigned char * Asc,int AscLen, unsigned char * Hex);


#endif