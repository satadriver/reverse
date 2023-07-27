
#ifndef HTTP_SHOPPING_AIQIYISHANGCHENG_H_H_H
#define HTTP_SHOPPING_AIQIYISHANGCHENG_H_H_H

#include "Shopping.h"

/************************************************************************/
/* 爱奇艺商城                                                           */
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
		//pc新增地址
		int PCWebAddAddress(const char *packet);
		//pc修改地址
		int PCWebModifyAddress(const char *packet);	
		enum
		{
			kPCWebAddAddress = 1,
			kPCWebModifyAddress,
		};
	};
}

#endif