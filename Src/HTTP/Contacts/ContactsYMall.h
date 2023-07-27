
#ifndef HTTP_CONTACTS_CONTACTSYMALL_H_H_H
#define HTTP_CONTACTS_CONTACTSYMALL_H_H_H

#include "Contacts.h"

namespace contacts
{
	class YMall : public Contacts
	{
	public:
		YMall(HttpSession *http_session, const string &pro_name);
		~YMall();

		int static IsYMall(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:

		enum
		{
			kAndroidUploadContacts = 1
		};

		int AndroidUploadContacts(const char *packet);
	};
}

#endif