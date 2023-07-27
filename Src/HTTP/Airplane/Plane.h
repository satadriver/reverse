#ifndef HTTP_PLANE_PLANE_H_H_H
#define HTTP_PLANE_PLANE_H_H_H

#include "../HttpPro.h"
#include "../json/json.h"	 //ʹ��json ��ȡ����
#include "../../tinyxml/tinyxml.h" //ʹ��xml ��ȡ����
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
		string password;		//����
		string adtCount;		//����������
		string chdCount;		//��ͯ����
	    string infCount;		//Ӥ������
		string deptDt;			//��������
		string deptDtRe;		//��������
		string deptCdTxt;		//������
		string arrCdTxt;		//Ŀ�ĵ�
		string username;		//�˿�����
		string idtype_;         //�˿�֤������
		string idcard;			//�˿����֤��
		string contactname_;	//��ϵ������
		string mobile;			//��ϵ�˵绰
		string email;			//����ʱ
		string orderno;			//������
		string triptype_;		//�г�����
	};
}

#endif