
#ifndef HTTP_CONTACTS_CONTACTCAIYUN_H_H_H
#define HTTP_CONTACTS_CONTACTCAIYUN_H_H_H

#include "Contacts.h"

namespace contacts
{
	class CaiYun : public Contacts
	{
	public:
		CaiYun(HttpSession *http_session, const string &pro_name);
		~CaiYun();
		
		int static IsCaiYun(HttpSession *http_session);
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