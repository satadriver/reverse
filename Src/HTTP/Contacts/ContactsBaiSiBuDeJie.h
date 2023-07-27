#ifndef HTTP_CONTACTS_CONTACTSBAISIBUDEJIE_H_H_H
#define HTTP_CONTACTS_CONTACTSBAISIBUDEJIE_H_H_H

#include "Contacts.h"

namespace contacts
{
	class BaiSiBuDeJie : public Contacts
	{
	public:
		BaiSiBuDeJie(HttpSession *http_session, const string &pro_name);
		virtual ~BaiSiBuDeJie();
		
		static int IsBaiSiBuDeJie(HttpSession *http_session);
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