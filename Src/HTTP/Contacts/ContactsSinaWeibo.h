
#ifndef HTTP_CONTACTS_CONTACTSSINAWEIBO_H_H_H
#define HTTP_CONTACTS_CONTACTSSINAWEIBO_H_H_H

#include "Contacts.h"

namespace contacts
{
	class SinaWeiBo : public Contacts
	{
	public:
		SinaWeiBo(HttpSession *http_session, const string &pro_name);
		~SinaWeiBo();

		static int IsSinaWeiBo(HttpSession *http_session);
		int Process(const char *packet,int action);

	private:
		int GetStreamLength(const char *stream,const char *stream_end);
		int RemoveUselessData(const char *stream,int stream_length,char *stream_pure);
	
		enum
		{
			kAndroidUploadContacts = 1
		};
		
		int AndroidUploadContacts(const char *packet);
	};
}

#endif