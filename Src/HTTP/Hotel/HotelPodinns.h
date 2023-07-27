#ifndef HTTP_HOTELPODINNS_H_H_H
#define HTTP_HOTELPODINNS_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelPodinns : public Hotel
	{
		public:
			HotelPodinns(HttpSession *http_session, const string &pro_name);
			~HotelPodinns();
			
			static int IsHotelPodinns(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebRegister(const char *packet);
			int WebLogin(const char *packet);
			int WebNewOrder(const char *packet);
			int WebCommonUser(const char*packet);
			int AndroidLogin(const char *packet);
			int AndroidLocation(const char *packet);
			enum 
			{
				kWebRegister = 1,
				kWebLogin,
				kWebNewOrder,
				kWebCommonUser,
				kAndroidLogin,
				kAndroidLocation,
			};
	};
}
#endif