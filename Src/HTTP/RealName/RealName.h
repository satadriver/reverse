#ifndef HTTP_REALNAME_REALNAME_H_H_H
#define HTTP_REALNAME_REALNAME_H_H_H

#include "../HttpPro.h"
#include "../json/json.h"
#include "../BoundaryParser.h"

namespace realname
{
	class RealName : public HttpPro
	{
	public:
		RealName(HttpSession *http_session, const string &pro_name);
		virtual ~RealName();		
		static int ProcessRealName(HttpSession *http_session ,const char *packet);	
	protected:	
		string GetOutputText();
		string name_;		//姓名
		string gender_;		//性别
		string birthday_;	//出生日期
		string idType_;		//证件类型
		string id_;			//证件号
		string phone_;		//电话号码
		string email_;		//邮箱
		string address_;	//地址
		string app_;		//来源
	};
}
#endif
