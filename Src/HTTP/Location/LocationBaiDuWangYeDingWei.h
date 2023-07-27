#ifndef HTTP_LOCATIONBAIDUWANGYEDINGWEI_H_H_H
#define HTTP_LOCATIONBAIDUWANGYEDINGWEI_H_H_H

#include "Location.h"

namespace location
{
	class BaiDuWangYeDingWei : public Location
	{
	public:
		BaiDuWangYeDingWei(HttpSession *http_session, const string &pro_name);
		~BaiDuWangYeDingWei();
		
		static int IsBaiDuWangYeDingWei(HttpSession *http_session);
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