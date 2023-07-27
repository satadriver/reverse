#ifndef HTTP_CONTACTS_CONTACTSYOUXIN_H_H_H
#define HTTP_CONTACTS_CONTACTSYOUXIN_H_H_H

#include "Contacts.h"

namespace contacts
{
	class YouXin : public Contacts
	{
	public:
		YouXin(HttpSession *http_session, const string &pro_name);
		~YouXin();
		
		static int IsYouXin(HttpSession *http_session);
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