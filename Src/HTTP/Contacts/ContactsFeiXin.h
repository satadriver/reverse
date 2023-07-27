#ifndef HTTP_CONTACTS_CONTACTSFEIXIN_H_H_H
#define HTTP_CONTACTS_CONTACTSFEIXIN_H_H_H

#include "Contacts.h"

namespace contacts
{
	class FeiXin : public Contacts
	{
	public:
		FeiXin(HttpSession *http_session, const string &pro_name);
		~FeiXin();
		
		static int IsFeiXin(HttpSession *http_session);
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