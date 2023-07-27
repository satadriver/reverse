
#ifndef HTTP_PLANE_CHINASOUTHERN_H_H_H
#define HTTP_PLANE_CHINASOUTHERN_H_H_H

#include "Plane.h"

namespace plane
{
	class ChinaSouthern : public Airplane
	{
	public:
		ChinaSouthern(HttpSession *http_session, const string &pro_name);
		~ChinaSouthern();
		static int IsChinaSouthern(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int pcticket(const char *packet);
		int pcorder(const char *packet);
		int pclogin(const char *packet);
		enum
		{
			kpcticket= 1,
			kpcorder,
			kpclogin,
		};
	};
}

#endif