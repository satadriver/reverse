#ifndef HTTP_CONTACTS_CONTACTSDOUGUO_H_H_H
#define HTTP_CONTACTS_CONTACTSDOUGUO_H_H_H

#include "Contacts.h"

namespace contacts
{
	class DouGuo : public Contacts
	{
		public:
			DouGuo(HttpSession *http_session, const string &pro_name);
			virtual ~DouGuo();	
			int Process(const char *packet,int action);	
			static int IsDouGuo(HttpSession *http_session);
		private:	
			enum
			{
				kAndroidUploadContacts = 1
			};		
			int AndroidUploadContacts(const char *packet);
	};
}

#endif