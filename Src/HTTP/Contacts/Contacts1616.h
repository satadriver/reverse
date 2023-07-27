
#ifndef HTTP_CONTACTS_CONTACT1616_H_H_H
#define HTTP_CONTACTS_CONTACT1616_H_H_H

#include "Contacts.h"

namespace contacts
{
	class Contacts1616 : public Contacts
	{
	public:
		Contacts1616(HttpSession *http_session, const string &pro_name);
		~Contacts1616();
		
		int static IsContacts1616(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		enum
		{
			kUploadContacts = 1
		};
		
		int UploadContacts(const char *packet);
	};
}
#endif