
#ifndef HTTP_SHOPPING_H_H_H
#define HTTP_SHOPPING_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" //ʹ��json ��ȡ�ַ���

namespace shopping
{
	class Shopping : public HttpPro
	{
	public:
		Shopping(HttpSession *http_session, const string &pro_name);
		virtual ~Shopping();		
		static int ProcessShopping(HttpSession *http_session ,const char *packet);	
	protected:		
		string user_;       //�û���
		string password_;   //����
		string name_;       //����
		string phone_;      //�绰
		string address_;	//��ַ
		string postcode_;	//�ʱ�		2016-09-06
		string email_;		//����		2016-09-06
        string tel_;        //�̶��绰	2016-09-06
		string addressid_;  //��ַid	2016-09-06
		string idcard_;     //���֤��  2016-09-06
		string imei_;       //imei
		string imsi_;       //imsi
		string longitude_;  //����
		string latitude_;   //γ��
		string mac_;        //Mac��ַ
	//������Ϣ
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