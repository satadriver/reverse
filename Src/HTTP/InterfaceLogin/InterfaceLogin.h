#ifndef HTTP_INTERFACELOGIN_H_H_H
#define HTTP_INTERFACELOGIN_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//�����ռ�
namespace interfacelogin  
{
	//����
	class InterfaceLogin : public HttpPro
	{
	public:
		//���캯��
		InterfaceLogin(HttpSession *http_session, const string &pro_name);
		//������
		virtual ~InterfaceLogin();
		//�ж��Ƿ�Ϊ����
		static int ProcessInterfaceLogin(HttpSession *http_session ,const char *packet);
	//����������
	protected:
       //string name
	};
}
#endif
