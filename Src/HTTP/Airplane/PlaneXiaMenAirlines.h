
#ifndef HTTP_PLANE_PLANEXIAMENAIRLINES_H_H_H
#define HTTP_PLANE_PLANEXIAMENAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class XiaMenAirLines: public Airplane
	{
	public:
		XiaMenAirLines(HttpSession *http_session, const string &pro_name);
		~XiaMenAirLines();
		static int IsXiaMenAirLines(HttpSession *http_session);
		int Process(const char *packet,int action);
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
            //���ڵ��� ���� ��ѯ
			kpcticketcn = 1,
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