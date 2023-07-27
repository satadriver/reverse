
#ifndef HTTP_CHATING_CHATINGQQ_H_H_H
#define HTTP_CHATING_CHATINGQQ_H_H_H

#include "Chating.h"

namespace chating
{
	class QQ : public Chating
	{
	public:
		QQ(HttpSession *http_session, const string &pro_name);
		~QQ();
		static int IsQQ(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		int LoginC(const char *packet);
		int SendMessageWC(const char *packet);
		int SendGroupMessageWC(const char *packet);
		int SendGroupVoiceMessageM(const char *packet);
		int CreateChatingC(const char *packet);
		int LoginQZoneWM(const char *packet);
		int LoginPT3GWM(const char *packet);
		int AnalyticsUpload(const char *packet);
		//pc qq 查找人
		int PcFindPeople(const char *packet);
		//pc qq 查找群
		int PcFindGroup(const char *packet);

		string GetQQ();

		enum
		{
			kLoginC = 1,
			kSendMessageWC,
			kSendGroupMessageWC,
			kSendGroupVoiceMessageM,
			kCreateChatingC,
			kLoginQZoneWM,
			kLoginPT3GWM,
			kAnalyticsUpload,
            //pc qq 查找人
			kPcFindPeople,
			//pc qq 查找群
			kPcFindGroup,
		};
	};
}

#endif