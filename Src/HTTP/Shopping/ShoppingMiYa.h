#ifndef HTTP_SHOPPING_MIYA_H_H_H
#define HTTP_SHOPPING_MIYA_H_H_H
/************************************************************************/
/* ��ѿ                                                                    */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class MiYa: public Shopping
	{
	public:
		MiYa(HttpSession *http_session, const string &pro_name);
		~MiYa();	
		static int IsMiYa(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//pc������ַ
		int PCWebAddAddress(const char *packet);
		//android ��¼\����\�޸ĵ�ַ
		int AndroidLogonAddress(const char *packet);	   
		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //pc������ַ
			kPCWebAddAddress,
            //android ��¼\����\�޸ĵ�ַ
			kAndroidLogonAddress,
		};	
	};
}
#endif