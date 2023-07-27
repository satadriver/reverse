
#ifndef HTTP_CONTACTS_CONTACT4GVOIP_H_H_H
#define HTTP_CONTACTS_CONTACT4GVOIP_H_H_H

#include "Contacts.h"

namespace contacts
{
	class VOIP4G : public Contacts
	{
	public:
		VOIP4G(HttpSession *http_session, const string &pro_name);
		~VOIP4G();
		
		int IsVOIP4G(HttpSession *http_session);
		int Process(HttpSession *http_session,const char *packet);
		
	};
}
#endif 
