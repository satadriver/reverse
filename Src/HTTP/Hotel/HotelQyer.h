#ifndef HTTP_HOTELQYER_H_H_H
#define HTTP_HOTELQYER_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelQyer : public Hotel
	{
		public:
			HotelQyer(HttpSession *http_session, const string &pro_name);
			~HotelQyer();
			
			static int IsHotelQyer(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebSearch(const char *packet);
			enum 
			{
				kWebSearch = 1,
			};
	};
}
#endif