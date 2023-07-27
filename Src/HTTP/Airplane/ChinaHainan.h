
#ifndef HTTP_PLANE_CHINAHAINAN_H_H_H
#define HTTP_PLANE_CHINAHAINAN_H_H_H

#include "Plane.h"

namespace plane
{
	class ChinaHainan : public Airplane
	{
	public:
		ChinaHainan(HttpSession *http_session, const string &pro_name);
		~ChinaHainan();
		static int IsChinaHainan(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		
		int pcticket(const char *packet);
		int pcticketmulti(const char *packet);
		int pcorder(const char *packet);
		int pclogin(const char *packet);
		enum
		{
			kpcticket= 1,
            kpcticketmulti,
			kpcorder,
			kpclogin,
		};
	};
}

#endif