#ifndef HTTP_SHOPPING_KUAILEGOU_H_H_H
#define HTTP_SHOPPING_KUAILEGOU_H_H_H
/************************************************************************/
/* ���ֹ�                                                               */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class KuaiLeGou: public Shopping
	{
	public:
		KuaiLeGou(HttpSession *http_session, const string &pro_name);
		~KuaiLeGou();	
		static int IsKuaiLeGou(HttpSession *http_session );	
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