#ifndef HTTP_PLANE_PLANE_H_H_H
#define HTTP_PLANE_PLANE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h"	 //使用json 提取数据
#include "../../tinyxml/tinyxml.h" //使用xml 提取数据
//#pragma comment(lib, "../../tinyxml.lib")

namespace plane
{
	class Airplane : public HttpPro
	{
	public:
		Airplane(HttpSession *http_session, const string &pro_name);
		virtual ~Airplane();
		
		static int ProcessAirplane(HttpSession *http_session ,const char *packet);

	protected:
		string password;		//密码
		string adtCount;		//成年人数量
		string chdCount;		//儿童数量
	    string infCount;		//婴儿数量
		string deptDt;			//出发日期
		string deptDtRe;		//返程日期
		string deptCdTxt;		//出发地
		string arrCdTxt;		//目的地
		string username;		//乘客姓名
		string idtype_;         //乘客证件类型
		string idcard;			//乘客身份证号
		string contactname_;	//联系人姓名
		string mobile;			//联系人电话
		string email;			//邮箱时
		string orderno;			//订单号
		string triptype_;		//行程类型
	};
}

#endif