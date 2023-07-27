

#ifndef HTTP_COMPUTERDEVICE_COMPUTERDEVICE_H_H_H
#define HTTP_COMPUTERDEVICE_COMPUTERDEVICE_H_H_H

#include "../HttpPro.h"

namespace computerdevice
{
	class ComputerDevice : public HttpPro
	{
	public:
		ComputerDevice(HttpSession *http_session, const string &pro_name);
		virtual ~ComputerDevice();
		
		static int ProcessComputerDevice(HttpSession *http_session ,const char *packet);
		
	protected:

		string mid_;
		string mid_from_;
	private:
	};
}

#endif
