
#ifndef HTTP_SEARCH_SEARCHSINA_H_H_H
#define HTTP_SEARCH_SEARCHSINA_H_H_H

#include "Search.h"

namespace search
{
	class Sina : public Search
	{
	public:
		Sina(HttpSession *http_session, const string &pro_name);
		~Sina();
		static int IsSina(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int WWW(const char *packet);
		
		enum
		{
			kWWW = 1,	
		};
		
	};
}

#endif