
#ifndef HTTP_LOCATION_LOCATIONQINGTINGFM_H_H_H
#define HTTP_LOCATION_LOCATIONQINGTINGFM_H_H_H

#include "Location.h"

namespace location
{
	class QingTingFM : public Location
	{
	public:
		QingTingFM(HttpSession *http_session, const string &pro_name);
		~QingTingFM();
		
		static int IsQingTingFM(HttpSession *http_session);
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