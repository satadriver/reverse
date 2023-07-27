
#ifndef HTTP_CHATING_CHATING_H_H_H
#define HTTP_CHATING_CHATING_H_H_H

#include "../HttpPro.h"

namespace chating
{
	class Chating : public HttpPro
	{
	public:
		Chating(HttpSession *http_session, const string &pro_name);
		virtual ~Chating();

		static int ProcessChating(HttpSession *http_session ,const char *packet);
	protected:
		
		string GetOutputText();
		string user_;
		string password_;
		string from_;
		string to_;

		string message_;

	};
}

#endif