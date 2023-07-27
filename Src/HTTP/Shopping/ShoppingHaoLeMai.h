#ifndef HTTP_SHOPPING_HAOLEMAI_H_H_H
#define HTTP_SHOPPING_HAOLEMAI_H_H_H
/************************************************************************/
/*������                                                                    */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class HaoLeMai: public Shopping
	{
	public:
		HaoLeMai(HttpSession *http_session, const string &pro_name);
		~HaoLeMai();	
		static int IsHaoLeMai(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//pc������ַ
		int PCWebAddAddress(const char *packet);
		//pc�޸ĵ�ַ
		int PCWebModifyAddress(const char *packet);	     
		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //pc������ַ
			kPCWebAddAddress,
			//pc�޸ĵ�ַ
			kPCWebModifyAddress,
		};	
	};
}
#endif