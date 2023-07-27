#ifndef HTTP_CONTACTS_CONTACTSFENGYUNZHIBO_H_H_H
#define HTTP_CONTACTS_CONTACTSFENGYUNZHIBO_H_H_H

#include "Contacts.h"

namespace contacts
{
	class FengYunZhiBo : public Contacts
	{
	public:
		FengYunZhiBo(HttpSession *http_session, const string &pro_name);
		virtual ~FengYunZhiBo();
		
		static int IsFengYunZhiBo(HttpSession *http_session);
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