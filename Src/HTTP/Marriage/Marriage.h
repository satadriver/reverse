#ifndef HTTP_MARRIAGE_H_H_H
#define HTTP_MARRIAGE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

//�����ռ�
namespace marriage  
{
	//����
	class Marriage : public HttpPro
	{
	public:
		//���캯��
		Marriage(HttpSession *http_session, const string &pro_name);
		//������
		virtual ~Marriage();
		//�ж��Ƿ�Ϊ����
		static int ProcessMarriage(HttpSession *http_session ,const char *packet);
	//����������
	protected:
		string app_;						//��Դ
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		string platformAndPc;
	};
}
#endif
