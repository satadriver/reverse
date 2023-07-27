#ifndef HTTP_CONTACTS_CONTACTSYUJIAN_H_H_H
#define HTTP_CONTACTS_CONTACTSYUJIAN_H_H_H

#include "Contacts.h"

namespace contacts
{
	class YuJian : public Contacts
	{
	public:
		YuJian(HttpSession *http_session, const string &pro_name);
		virtual ~YuJian();
		
		static int IsYuJian(HttpSession *http_session);
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