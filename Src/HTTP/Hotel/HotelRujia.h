#ifndef HTTP_HOTELRUJIA_H_H_H
#define HTTP_HOTELRUJIA_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelRujia : public Hotel
	{
		public:
			HotelRujia(HttpSession *http_session, const string &pro_name);
			~HotelRujia();
			
			static int IsHotelRujia(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebRegister(const char *packet);
			int WebSearch(const char *packet);
			int WebNewOrder(const char *packet);
			int WebIntNewOrder(const char *packet);
			enum 
			{
				kWebRegister = 1,
				kWebSearch,
				kWebNewOrder,
				kWebIntNewOrder,
			};
	};
}
#endif