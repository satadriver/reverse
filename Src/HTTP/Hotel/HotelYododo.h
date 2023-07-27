#ifndef HTTP_HOTELYODODO_H_H_H
#define HTTP_HOTELYODODO_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelYododo : public Hotel
	{
		public:
			HotelYododo(HttpSession *http_session, const string &pro_name);
			~HotelYododo();
			
			static int IsHotelYododo(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			//���ھƵ�
			int WebRegister(const char *packet);
			int WebLogin(const char *packent);
            int WebSearch(const char *packet);
			int WebIntSearch(const char *packet);
			int WebIntNewOrder(const char *packet);

			//��ջ
			int WebHotelLogin(const char *packet);
			int WebHotelSearch(const char *packet);
			int WebHotelOrder(const char *packet);
			int AndroidLogin(const char *packet);
			int AndroidSearch(const char *packet);
			int AndroidOrder(const char *packet);
			enum 
			{
				//���ھƵ�
				kWebRegister = 1,
				kWebLogin,
				kWebSearch,
				kWebIntSearch,
				kWebIntNewOrder,

				//��ջ
				kWebHotelLogin,
				kWebHotelSearch,
			    kWebHotelOrder,
				kAndroidLogin,
				kAndroidSearch,
				kAndroidOrder,
			};
	};
}
#endif