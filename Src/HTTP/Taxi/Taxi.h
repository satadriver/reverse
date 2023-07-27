
#ifndef HTTP_TAXI_TAXI_H_H_H
#define HTTP_TAXI_TAXI_H_H_H

#include "../HttpPro.h"

namespace taxi
{
	class Taxi : public HttpPro
	{
	public:
		Taxi(HttpSession *http_session, const string &pro_name);
		virtual ~Taxi();
		
		static int ProcessTaxi(HttpSession *http_session ,const char *packet);
		
	protected:

//		int GetRealFileSize(const char *stream_begin,const char *stream_end);

		string phone_;

		string imei_;
		string imsi_;

		string longitude_;
		string latitude_;

		string from_;
		string to_;
	private:
		string GetOutputText();

	};
}

#endif