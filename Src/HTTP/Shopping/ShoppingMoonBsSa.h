#ifndef HTTP_SHOPPING_MOONBASA_H_H_H
#define HTTP_SHOPPING_MOONBASA_H_H_H
/************************************************************************/
/* �ΰ�ɯ                                                               */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MoonBaSa: public Shopping
	{
	public:
		MoonBaSa(HttpSession *http_session, const string &pro_name);
		~MoonBaSa();	
		static int IsMoonBaSa(HttpSession *http_session );	
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
    
		//android��¼
		int AndroidLogin(const char *packet);
		//android������ַ
		int AndroidAddAddress(const char *packet);
		//android�޸ĵ�ַ
		int AndroidEditAddress(const char *packet);
		//android�ύ����
		int AndroidOrder(const char *packet);
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
			//android��¼
			kAndroidLogin,
			//android������ַ
			kAndroidAddAddress,
			//android�޸ĵ�ַ
			kAndroidEditAddress,
			//android�ύ����
			kAndroidOrder,
		};	
	};
}
#endif