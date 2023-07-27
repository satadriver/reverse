#ifndef HTTP_HOTELSUPEREIGHT_H_H_H
#define HTTP_HOTELSUPEREIGHT_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelSuperEight : public Hotel
	{
		public:
			HotelSuperEight(HttpSession *http_session, const string &pro_name);
			~HotelSuperEight();
			
			static int IsHotelSuperEight(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebRegister(const char *packet);
			int WebLogin(const char *packet);
			int WebNewOrder(const char *packet);
			enum 
			{
				kWebRegister = 1,
				kWebLogin,
				kWebNewOrder,
			};
	};
}
#endif