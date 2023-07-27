
#ifndef HTTP_PLANE_PLANEJUNEYAOAIR_H_H_H
#define HTTP_PLANE_PLANEJUNEYAOAIR_H_H_H

#include "Plane.h"

namespace plane
{
	class PlaneJuneYaoAir: public Airplane
	{
	public:
		PlaneJuneYaoAir(HttpSession *http_session, const string &pro_name);
		~PlaneJuneYaoAir();
		static int IsPlaneJuneYaoAir(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//��¼
		int pclogin(const char *packet);
		//��ѯ
		int pcticket(const char *packet);
		//�ύ���� 
		int pcorder(const char *packet);
		//�ύ���� ����
		int pcorderint(const char *packet);
		enum
		{
			//��¼
			kpclogin = 1, 
            //��ѯ ��ҳ
			kpcticket,
			//�ύ����
			kpcorder,
			//�ύ���� ����
			kpcorderint,
		};
	};
}

#endif