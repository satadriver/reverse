#ifndef HTTP_MARRIAGE_H_H_H
#define HTTP_MARRIAGE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//命名空间
namespace marriage  
{
	//类名
	class Marriage : public HttpPro
	{
	public:
		//构造函数
		Marriage(HttpSession *http_session, const string &pro_name);
		//虚析构
		virtual ~Marriage();
		//判断是否为此类
		static int ProcessMarriage(HttpSession *http_session ,const char *packet);
	//定义额外变量
	protected:
		string app_;						//来源
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		string platformAndPc;
	};
}
#endif
