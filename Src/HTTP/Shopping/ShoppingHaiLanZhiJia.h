#ifndef HTTP_SHOPPING_HAILANZHIJIA_H_H_H
#define HTTP_SHOPPING_HAILANZHIJIA_H_H_H
/************************************************************************/
/* ����֮��                                                             */
/************************************************************************/
#include "Shopping.h"
namespace shopping
{
	class HaiLanZhiJia: public Shopping
	{
	public:
		HaiLanZhiJia(HttpSession *http_session, const string &pro_name);
		~HaiLanZhiJia();	
		static int IsHaiLanZhiJia(HttpSession *http_session );	
		int Process(const char *packet,int action);	
	private:
		//pc��¼��Ϣ
		int PCWebLogin(const char *packet);		
		//pc������ַ�޸ĵ�ַ
		int PCWebAddAddress(const char *packet);
		int PCWebModiyAdd(const char *packet);
		//�ֻ��˵�¼��Ϣ
		int MobileLogin(const char *packet);
		//�ֻ��������޸ĵ�ַ
		int MobileAddress(const char *packet);
		int MobileModiy(const char *packet);

		enum
		{
			//pc��¼��Ϣ
			kPCWebLogin = 1,
            //pc������ַ
			kPCWebAddAddress,
			kPCWebModiyAdd,
			//�ֻ��˵�¼
			kMobileLogin,
			//�ֻ��������޸ĵ�ַ
			KMobileAddress,
			kMobileModiy,
		};	
	};
}
#endif