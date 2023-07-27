#ifndef HTTP_SUPPLY58_H_H_H
#define HTTP_SUPPLY58_H_H_H

#include "Supply.h"
namespace supply
{
	class Supply58 : public Supply
	{
		public:
			Supply58(HttpSession *http_session, const string &pro_name);
			~Supply58();
			
			static int IsSupply58(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebPublicRecruitment(const char *packet);
			int WebPublic(const char *packet);
			int WebPublicParttime(const char *packet);
			enum 
			{
				kWebPublicRecruitment = 1,
				kWebPublic,
				kWebPublicParttime,
			};
	};
}
#endif