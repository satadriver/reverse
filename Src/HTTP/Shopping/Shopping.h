
#ifndef HTTP_SHOPPING_H_H_H
#define HTTP_SHOPPING_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" //使用json 提取字符串

namespace shopping
{
	class Shopping : public HttpPro
	{
	public:
		Shopping(HttpSession *http_session, const string &pro_name);
		virtual ~Shopping();		
		static int ProcessShopping(HttpSession *http_session ,const char *packet);	
	protected:		
		string user_;       //用户名
		string password_;   //密码
		string name_;       //姓名
		string phone_;      //电话
		string address_;	//地址
		string postcode_;	//邮编		2016-09-06
		string email_;		//邮箱		2016-09-06
        string tel_;        //固定电话	2016-09-06
		string addressid_;  //地址id	2016-09-06
		string idcard_;     //身份证号  2016-09-06
		string imei_;       //imei
		string imsi_;       //imsi
		string longitude_;  //经度
		string latitude_;   //纬度
		string mac_;        //Mac地址
	//其它信息
    public:	
	  	static void InitIdentityCardAddressCodeMap();
	protected:
		string GetAddress(const string &addressCode);
		string addressName_;
		static map<string,string>addressCode_addressName_;
	private:
		static void StringReplace(string &src_string,const string &old_string ,const string &new_string);	
	};
}

#endif