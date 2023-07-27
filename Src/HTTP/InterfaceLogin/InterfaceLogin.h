#ifndef HTTP_INTERFACELOGIN_H_H_H
#define HTTP_INTERFACELOGIN_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//命名空间
namespace interfacelogin  
{
	//类名
	class InterfaceLogin : public HttpPro
	{
	public:
		//构造函数
		InterfaceLogin(HttpSession *http_session, const string &pro_name);
		//虚析构
		virtual ~InterfaceLogin();
		//判断是否为此类
		static int ProcessInterfaceLogin(HttpSession *http_session ,const char *packet);
	//定义额外变量
	protected:
       //string name
	};
}
#endif
