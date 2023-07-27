#ifndef HTTP_EMPLOY_EMPLOY_H_H_H
#define HTTP_EMPLOY_EMPLOY_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
#include "../json/json.h"			

namespace employ
{
	class Employ : public HttpPro
	{
		public:
			Employ(HttpSession *http_session, const string &pro_name);
			virtual ~Employ();		
			static int ProcessEmploy(HttpSession *http_session ,const char *packet);
			void ConstructUploadAttachmentOuptputString();
		protected:
			string from_;
			string attachment_;	
			string app_;
	};
}
#endif