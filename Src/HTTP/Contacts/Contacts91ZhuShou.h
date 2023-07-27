
#ifndef HTTP_CONTACTS_CONTACT91ZHUSHOU_H_H_H
#define HTTP_CONTACTS_CONTACT91ZHUSHOU_H_H_H

#include "Contacts.h"

namespace contacts
{
	class ZhuShou91 : public Contacts
	{
	public:
		ZhuShou91(HttpSession *http_session, const string &pro_name);
		~ZhuShou91();
		
		int static IsZhuShou91(HttpSession *http_session);
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