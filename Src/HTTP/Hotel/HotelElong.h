#ifndef HTTP_HOTELELONG_H_H_H
#define HTTP_HOTELELONG_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelElong : public Hotel
	{
		public:
			HotelElong(HttpSession *http_session, const string &pro_name);
			~HotelElong();
			
			static int IsHotelElong(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebSearch(const char *packet);
			int WebNewOrder(const char *packet);
			int WebIntNewOrder(const char *packet);
			enum 
			{
				kWebSearch = 1,
				kWebNewOrder,
				kWebIntNewOrder,
			};
	};
}
#endif