
#ifndef HTTP_PLANE_PLANEAIRCHINA_H_H_H
#define HTTP_PLANE_PLANEAIRCHINA_H_H_H

#include "Plane.h"

namespace plane
{
	class PlaneAirChina: public Airplane
	{
	public:
		PlaneAirChina(HttpSession *http_session, const string &pro_name);
		~PlaneAirChina();
		static int IsPlaneAirChina(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//��¼
		int pclogin(const char *packet);
		//��ѯ ��ҳ
		int pcticket(const char *packet);
		int pcticketsecond(const char *packet);
		//�ύ���� ��תҳ
		int pcorder(const char *packet);
		enum
		{
			//��¼
			kpclogin = 1, 
            //��ѯ ��ҳ
			kpcticket,
			//��ѯ ��תҳ
			kpcticketsecond,
			//�ύ����
			kpcorder,
		};
	};
}

#endif