
#ifndef HTTP_CONTACTS_CONTACTSXIMALAYA_H_H_H
#define HTTP_CONTACTS_CONTACTSXIMALAYA_H_H_H

#include "Contacts.h"

namespace contacts
{
	class XiMaLaYa : public Contacts
	{
	public:
		XiMaLaYa(HttpSession *http_session, const string &pro_name);
		~XiMaLaYa();

		int static IsXiMaLaYa(HttpSession *http_session);
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