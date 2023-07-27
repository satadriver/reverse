#ifndef HTTP_SHOPPING_SHOPPINGMAILEGOU_H_H_H
#define HTTP_SHOPPING_SHOPPINGMAILEGOU_H_H_H
/************************************************************************/
/* ���ֹ�                                                                     */
/************************************************************************/
#include "Shopping.h"

namespace shopping
{
	class MaiLeGOu : public Shopping
	{
	public:
		MaiLeGOu(HttpSession *http_session, const string &pro_name);
		~MaiLeGOu();		
		static int IsMaiLeGOu(HttpSession *http_session);	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//IE pc������ַ
		int PCWebAddAddress(const char *packet);
		//Google pc������ַ
		int PCWebGoogleAddAddress(const char *packet);
		//pc������������ַ
		int PCWebChuPingAddAddress(const char *packet);
		//pc�޸ĵ�ַ
		int PCWebModifyAddress(const char *packet);	
		//pc�ύ����
		int PCWebNewOrder(const char *packet);		
		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //IE pc������ַ
			kPCWebAddAddress,
			//Google pc ������ַ
			kPCWebGoogleAddAddress,
            //pc������������ַ
			kPCWebChuPingAddAddress,	
			//pc�޸ĵ�ַ
			kPCWebModifyAddress,
			//pc�ύ����
			kPCWebNewOrder,
		};		
	};
}

#endif 
