
#ifndef HTTP_CHATING_CHATINGVOXER_H_H_H
#define HTTP_CHATING_CHATINGVOXER_H_H_H

#include "Chating.h"

namespace chating
{
	class Voxer : public Chating
	{
	public:
		Voxer(HttpSession *http_session, const string &pro_name);
		~Voxer();
		static int IsVoxer(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int Login(const char *packet);
		enum
		{
			kLogin = 1
		};
	};
}

#endif