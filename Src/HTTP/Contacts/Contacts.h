
#ifndef HTTP_CONTACTS_CONTACTS_H_H_H
#define HTTP_CONTACTS_CONTACTS_H_H_H

#include "../HttpPro.h"

namespace contacts
{
	class Contacts : public HttpPro
	{
	public:
		Contacts(HttpSession *http_session, const string &pro_name);
		virtual ~Contacts();

		int static ProcessContacts(HttpSession *http_session,const char *packet);
		
	protected:
		string name_;
		string phone_;
	};
}

#endif