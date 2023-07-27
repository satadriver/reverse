
#ifndef HTTP_SHOPPING_AIQIYISHANGCHENG_H_H_H
#define HTTP_SHOPPING_AIQIYISHANGCHENG_H_H_H

#include "Shopping.h"

/************************************************************************/
/* �������̳�                                                           */
/************************************************************************/
namespace shopping
{
	class AiQiYiShangCheng : public Shopping
	{
	public:
		AiQiYiShangCheng(HttpSession *http_session, const string &pro_name);
		~AiQiYiShangCheng();		
		static int IsAiQiYiShangCheng(HttpSession *http_session);		
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