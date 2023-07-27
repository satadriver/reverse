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
		string name_;		//����
		string gender_;		//�Ա�
		string birthday_;	//��������
		string idType_;		//֤������
		string id_;			//֤����
		string phone_;		//�绰����
		string email_;		//����
		string address_;	//��ַ
		string app_;		//��Դ
	};
}
#endif
