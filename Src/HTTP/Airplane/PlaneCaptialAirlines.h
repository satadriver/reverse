
#ifndef HTTP_PLANE_PLANECAPTIALAIRLINES_H_H_H
#define HTTP_PLANE_PLANECAPTIALAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class CaptalAirLines: public Airplane
	{
	public:
		CaptalAirLines(HttpSession *http_session, const string &pro_name);
		~CaptalAirLines();
		static int IsCaptalAirLines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	public:
        //���ڵ��� ���� ��ѯ
		int pcticketcn(const char *packet);
		//���ʵ��� ���� ��ѯ
		int pcticketint(const char *packet);
		//���ʵ��� ���� �ύ����
		int pcordercn(const char *packet);
		//���ʵ��� ���� �ύ����
		int pcorderint(const char *packet);

		enum
		{
			//ע���¼
			kpclogin = 1, 
			//��ע���¼
			kpcnonreglogin,
            //���ڵ��� ���� ��ѯ
			kpcticketcn,
			//���ʵ��� ���� ��ѯ
			kpcticketint,
			//���ڵ��� ���� �ύ����
			kpcordercn,
			//���ʵ��� ���� �ύ����
			kpcorderint,
		};
	};
}

#endif