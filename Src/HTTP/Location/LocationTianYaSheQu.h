#ifndef HTTP_LOCATIONTIANYASHEQU_H_H_H
#define HTTP_LOCATIONTIANYASHEQU_H_H_H

#include "Location.h"

namespace location
{
	class TianYaSheQu : public Location
	{
	public:
		TianYaSheQu(HttpSession *http_session, const string &pro_name);
		~TianYaSheQu();
		
		static int IsTianYaSheQu(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
		};
	};
}

#endif