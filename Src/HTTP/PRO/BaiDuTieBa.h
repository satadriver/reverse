
#ifndef HTTP_BAIDUTIEBA_H_H_H
#define HTTP_BAIDUTIEBA_H_H_H


#include "../HttpPro.h"
#include "Attached/DeviceInfo.h"

class BaiDuTieBa : public HttpPro
{
public:
	BaiDuTieBa();
	~BaiDuTieBa();
	
	int Process(const char *packet,int action);
	int Is(HttpSession *http_session);
	
private:
	
	int NewPostCW(const char *packet);
	int NewPostM(const char *packet);

	int NewCommentCW(const char *packet);
	int NewCommentM(const char *packet);
	
	int LoginCW(const char *packet);
	int LoginM(const char *packet);
	
	int UploadDeviceInfoM(const char *packet);

	string GetBDUSS();
	string GetUserName(const string &bduss);
	enum
	{
		kNewPostCW = 1,
			kNewPostM,
			kNewCommentCW,
			kNewCommentM,
			kLoginCW,
			kLoginM,
			kUploadDeviceInfoM
	};
	//<BDUSS,USERNAME>
	static map<string,string>bduss_user_;
};
#endif