
#ifndef HTTP_FENGHUANGXINWEN_H_H_H
#define HTTP_FENGHUANGXINWEN_H_H_H

#include "../HttpPro.h"

class FengHuangXinWen : public HttpPro
{
public:
	FengHuangXinWen();
	~FengHuangXinWen();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int AddCommentC(const char *packet);
	int AddCommentM(const char *packet);
	int UploadDeviceInfoM(const char *packet);
	enum 
	{
		kAddCommentC = 1, //2014-11-14
		kAddCommentM = 2, //2014-11-14
		kUploadDeviceInfoM
	};

	string GetOutputText(int platform);
	
	/*
	string user_;
	
	string title_;
	string content_;
	string url_;

	string imei_;
	string imsi_;
	string os_;
	string version_;
	string model_;
	*/
	enum 
	{
		kC = 1,
			kM = 2
		};
};


#endif