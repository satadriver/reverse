
#ifndef HTTP_PLANE_LIANHEAIRLINES_H_H_H
#define HTTP_PLANE_LIANHEAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class LianHeAirlines : public Airplane
	{
	public:
		LianHeAirlines(HttpSession *http_session, const string &pro_name);
		~LianHeAirlines();
		static int IsLianHeAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		int pcticket(const char *packet);
		enum
		{
		   //≤È—Ø
			kpcticket=1,
		};
	};
}

#endif