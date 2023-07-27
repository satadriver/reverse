#ifndef HTTP_SHOPPING_MAIHAOHAO_H_H_H
#define HTTP_SHOPPING_MAIHAOHAO_H_H_H
/************************************************************************/
/* ��ú�                                                               */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MaiHaoHao: public Shopping
	{
	public:
		MaiHaoHao(HttpSession *http_session, const string &pro_name);
		~MaiHaoHao();	
		static int IsMaiHaoHao(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//pc������ַ
		int PCWebAddAddress(const char *packet);
		//pc�޸ĵ�ַ
		int PCWebModifyAddress(const char *packet);	
		//pc�ύ����
		int PCWebNewOrder(const char *packet);		
		//pcλ����Ϣ ���� 86 �й�
		int PCWebLocation(const char *packet);	

		//android��¼��Ϣ
		int AndroidLogin(const char *packet);			
		//android������ַ 
		int AndroidAddAddress(const char *packet);  
		//android�޸ĵ�ַ
		int AndroidModifyAddress(const char *packet); 
		//android�ύ����
		int AndroidNewOrder(const char *packet);		
		//androidλ����Ϣ
		int AndroidLocation(const char *packet);      
		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //pc������ַ
			kPCWebAddAddress,
			//pc�޸ĵ�ַ
			kPCWebModifyAddress,
			//pc�ύ����
			kPCWebNewOrder,
			//pcλ����Ϣ ���� 86 �й�
			kPCWebLocation,
			//android��¼��Ϣ
			kAndroidLogin,
			//android������ַ
			kAndroidAddAddress,
			//android�޸ĵ�ַ
			kAndroidModifyAddress,
			//android�ύ����
			kAndroidNewOrder,
			//androidλ����Ϣ
			kAndroidLocation,
		};	
	};
}
#endif