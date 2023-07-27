#ifndef HTTP_SOFTWARE_DUOTE_H_H_H 
#define HTTP_SOFTWARE_DUOTE_H_H_H

#include "Software.h"

namespace software
{
	class DuoTe: public Software
	{
	public:
		DuoTe(HttpSession *http_session, const string &pro_name);
		~DuoTe();
		static int Is(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//����
		int WebSearch(const char *packet);
		//����
		int WebDown(const char *packet);
		//����apk
		int ApkDown(const char *packet);
		enum
		{
			kWebSearch = 1,
			kWebDown,
			kApkDown,
		};
	};
}

#endif