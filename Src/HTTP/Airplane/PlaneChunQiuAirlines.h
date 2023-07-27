
#ifndef HTTP_PLANE_CHUNQIUAIRLINES_H_H_H
#define HTTP_PLANE_CHUNQIUAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class ChunQiuAirlines : public Airplane
	{
	public:
		ChunQiuAirlines(HttpSession *http_session, const string &pro_name);
		~ChunQiuAirlines();
		static int IsChunQiuAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		int pcticket(const char *packet);
		enum
		{
			kpcticket=1,
		};
	};
}

#endif