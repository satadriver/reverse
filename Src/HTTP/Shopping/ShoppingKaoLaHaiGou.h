#ifndef HTTP_SHOPPING_KAOLAHAIGOU_H_H_H
#define HTTP_SHOPPING_KAOLAHAIGOU_H_H_H
/************************************************************************/
/* ���׿�������       
web:	 �ύ����-	�ջ����˺�(��¼�˺�) �ջ��˵�ַ(��ַ��) ��ӵ�ַ-�˻��������ֻ��ţ���ַ
android: �ύ����-	�ջ����������绰(*)���ջ��˵�ַ(��ַ��) ��ӵ�ַ-�˻��������ֻ��ţ���ַ(��ַ��)                                                */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class KaoLaHaiGou: public Shopping
	{
	public:
		KaoLaHaiGou(HttpSession *http_session, const string &pro_name);
		~KaoLaHaiGou();	
		static int IsKaoLaHaiGou(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		int PCWebAddAddress(const char *packet);	//pc��ӵ�ַ
		int PCWebModifyAddress(const char *packet);	//pc�޸ĵ�ַ
		int PCWebNewOrder(const char *packet);		//pc�ύ���� 

		int AndroidAddAddress(const char *packet);		//android��ӵ�ַ
		int AndroidModifyAddress(const char *packet);	//android�޸ĵ�ַ
		int AndroidNewOrder(const char *packet);		//android�ύ����
		enum
		{
			kPCWebAddAddress = 1,
			kPCWebModifyAddress,
			kPCWebNewOrder,
			kAndroidAddAddress,
			kAndroidModifyAddress,
			kAndroidNewOrder,
		};	
	};
}
#endif