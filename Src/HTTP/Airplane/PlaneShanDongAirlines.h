
#ifndef HTTP_PLANE_PLANESHANDONGAIRLINES_H_H_H
#define HTTP_PLANE_PLANESHANDONGAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class ShanDongAirlines : public Airplane
	{
	public:
		ShanDongAirlines(HttpSession *http_session, const string &pro_name);
		~ShanDongAirlines();
		static int IsShanDongAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��¼
		int pclogin(const char *packet);
		//��ѯ
		int pcticket(const char *packet);
		//�ύ����
		int pcorder(const char *packet);
		enum
		{
			//��¼
			kpclogin = 1,
            //��ѯ
			kpcticket,
			//�ύ����
			kpcorder,
		};
	};
}

#endif