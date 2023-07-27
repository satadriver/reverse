
#ifndef HTTP_SHOPPING_YINYUESHANGCHENG_H_H_H
#define HTTP_SHOPPING_YINYUESHANGCHENG_H_H_H

#include "Shopping.h"

/************************************************************************/
/* �����̳�                                                           */
/************************************************************************/
namespace shopping
{
	class YinYueShangCheng : public Shopping
	{
	public:
		YinYueShangCheng(HttpSession *http_session, const string &pro_name);
		~YinYueShangCheng();		
		static int IsYinYueShangCheng(HttpSession *http_session);		
		int Process(const char *packet,int action);
		
	private:
		//pc������ַ
		int PCWebAddAddress(const char *packet);
		//pc�޸ĵ�ַ
		int PCWebModifyAddress(const char *packet);	
		enum
		{
			kPCWebAddAddress = 1,
			kPCWebModifyAddress,
		};
	};
}

#endif