#ifndef HTTP_LOCATIONAN_LOCATIONSUIEXING_H_H_H
#define HTTP_LOCATIONAN_LOCATIONSUIEXING_H_H_H

#include "Location.h"

namespace location
{
	class SuiEXingWLAN : public Location
	{
	public:
		SuiEXingWLAN(HttpSession *http_session, const string &pro_name);
		~SuiEXingWLAN();
		
		static int IsSuiEXingWLAN(HttpSession *http_session);
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