#ifndef HTTP_HOTELQUNER_H_H_H
#define HTTP_HOTELQUNER_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelQuner : public Hotel
	{
		public:
			HotelQuner(HttpSession *http_session, const string &pro_name);
			~HotelQuner();
			
			static int IsHotelQuner(HttpSession *http_session);
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