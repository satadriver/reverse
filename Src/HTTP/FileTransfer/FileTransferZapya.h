#ifndef HTTP_FILETRANSFER_ZAPYA_H_H_H
#define HTTP_FILETRANSFER_ZAPYA_H_H_H

#include "FileTransfer.h"

namespace filetransfer
{
	class ZapyaFileTransfer: public FileTransfer
	{
	public:
		ZapyaFileTransfer(HttpSession *http_session, const string &pro_name);
		~ZapyaFileTransfer();
		static int IsZapyaFileTransfer(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//android×¢²á
		int AndroidRegister(const char *packet);
		//androidµÇÂ¼
		int AndroidLogin(const char *packet);
		//¶¨Î»
		int AndroidLocation(const char *packet);
		enum
		{
			kAndroidRegister = 1,
			kAndroidLogin,
            kAndroidLocation,
		};
	};
}

#endif