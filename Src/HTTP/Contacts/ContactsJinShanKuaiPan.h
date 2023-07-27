
#ifndef HTTP_CONTACTS_CONTACTJINSHANKUIAPAN_H_H_H
#define HTTP_CONTACTS_CONTACTJINSHANKUIAPAN_H_H_H

#include "Contacts.h"

namespace contacts
{
	class JinShanKuaiPan : public Contacts
	{
	public:
		JinShanKuaiPan(HttpSession *http_session, const string &pro_name);
		~JinShanKuaiPan();
		
		int static IsJinShanKuaiPan(HttpSession *http_session);
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