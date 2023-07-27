
#ifndef HTTP_PLANE_CHINAEASTERN_H_H_H
#define HTTP_PLANE_CHINAEASTERN_H_H_H

#include "Plane.h"

namespace plane
{
	class ChinaEastern : public Airplane
	{
	public:
		ChinaEastern(HttpSession *http_session, const string &pro_name);
		~ChinaEastern();
		static int IsChinaEastern(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//µÇÂ¼
		int pclogin(const char *packet);
		int pcticket(const char *packet);
		int pcorder(const char *packet);
		enum
		{
			//µÇÂ¼
			kpclogin = 1,
			kpcticket,
			kpcorder,
		};
	};
}

#endif