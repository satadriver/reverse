#ifndef HTTP_HOTEL_H_H_H
#define HTTP_HOTEL_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//命名空间
namespace hotel  
{
	//类名
	class Hotel:public HttpPro
	{
		public:
			//构造函数
			Hotel(HttpSession *http_session, const string &pro_name);
			//虚析构
			virtual ~Hotel();
			//判断是否为此类
			static int ProcessHotel(HttpSession *http_session ,const char *packet);
		//定义额外变量
		protected:
		   string app_;
		   string platformWeb;
		   string platformPC;
		   string platformAndroid;
		   string platformIos;
		   string platformAndPc;
	};
}
#endif
