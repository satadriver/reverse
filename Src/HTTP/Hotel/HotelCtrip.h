#ifndef HTTP_HOTELCTRIP_H_H_H
#define HTTP_HOTELCTRIP_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelCtrip : public Hotel
	{
		public:
			HotelCtrip(HttpSession *http_session, const string &pro_name);
			~HotelCtrip();
			
			static int IsHotelCtrip(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebSearch(const char *packet);
			int WebIntSearch(const char *packet);
			int WebNewOrder(const char *packet);
			int WebIntNewOrder(const char *packet);
			enum 
			{
				kWebSearch = 1,
				kWebIntSearch,
				kWebNewOrder,
				kWebIntNewOrder,
			};
	};
}
#endif