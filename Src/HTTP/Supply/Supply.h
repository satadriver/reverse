#ifndef HTTP_SUPPLY_H_H_H
#define HTTP_SUPPLY_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 
#include "../BoundaryParser.h"
//命名空间
namespace supply  
{
	//类名
	class Supply:public HttpPro
	{
		public:
			//构造函数
			Supply(HttpSession *http_session, const string &pro_name);
			//虚析构
			virtual ~Supply();
			//判断是否为此类
			static int ProcessSupply(HttpSession *http_session ,const char *packet);
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
