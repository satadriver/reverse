#ifndef HTTP_CONTACTS_CONTACTSYYBACKUP_H_H_H
#define HTTP_CONTACTS_CONTACTSYYBACKUP_H_H_H

#include "Contacts.h"

namespace contacts
{
	class YYBackup : public Contacts
	{
	public:
		YYBackup(HttpSession *http_session, const string &pro_name);
		virtual ~YYBackup();
		
		static int IsYYBackup(HttpSession *http_session);
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