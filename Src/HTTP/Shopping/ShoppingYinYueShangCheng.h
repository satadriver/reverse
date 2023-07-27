
#ifndef HTTP_SHOPPING_YINYUESHANGCHENG_H_H_H
#define HTTP_SHOPPING_YINYUESHANGCHENG_H_H_H

#include "Shopping.h"

/************************************************************************/
/* 音悦商城                                                           */
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