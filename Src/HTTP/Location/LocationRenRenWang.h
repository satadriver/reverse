#ifndef HTTP_LOCATIONAN_LOCATIONRENRENWANG_H_H_H
#define HTTP_LOCATIONAN_LOCATIONRENRENWANG_H_H_H

#include "Location.h"

namespace location
{
	class RenRenWang : public Location
	{
	public:
		RenRenWang(HttpSession *http_session, const string &pro_name);
		~RenRenWang();
		
		static int IsRenRenWang(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1
		};
	};
}

#endif