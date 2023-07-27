#ifndef HTTP_LOCATION_LOCATIONOTHER_H_H_H
#define HTTP_LOCATION_LOCATIONOTHER_H_H_H

#include "Location.h"

namespace location
{
	class Other : public Location
	{
	public:
		Other(HttpSession *http_session, const string &pro_name);
		~Other();
		
		static int IsOther(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int changba(const char *packet);
		enum 
		{
			kchangba = 1,
		};
	};
}

#endif