
#ifndef HTTP_CHATING_CHATINGYIXIN_H_H_H
#define HTTP_CHATING_CHATINGYIXIN_H_H_H

#include "Chating.h"


struct YiXinFileBuff
{
	YiXinFileBuff()
	{
		offset = 0;
		length = 0;
		data = NULL;
	}
	
	~YiXinFileBuff()
	{
		if (data != NULL)
		{
			delete [] data;
		}
	}
	int offset;
	int length;
	char *data;
};

struct YiXinFileBuffList
{
	YiXinFileBuffList()
	{
		ct = 0;
		complete = false;
	}
	~YiXinFileBuffList()
	{
		
	}
	time_t ct;
	bool complete;
	string uid;
	string buffType;
	string uploadType;
	list<YiXinFileBuff *>buffs;
};


namespace chating
{


	class YiXin : public Chating
	{
	public:
		YiXin(HttpSession *http_session, const string &pro_name);
		~YiXin();
		static int IsYiXin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int Login(const char *packet);
		int SendVoiceMessageM(const char *packet);
		int UploadVoiceMI(const char *packet);
		int UploadPic(const char *packet);
		int UploadFile(const char *packet);
		int CacheUploadFile(const char *packet);
		enum
		{
			kLogin = 1,
			kUploadFile,
			kCacheUploadFile,
			kSendVoiceMessageM,
			kUploadVoiceMI,
			kUploadPic,
			kReassembleFileBuff,
		};

		string GetAccount(const string &account);
		int InsertVoiceBuffList(YiXinFileBuffList *vbl,YiXinFileBuff *vb);
		int ReassembleFileBuff();
		static map<string, YiXinFileBuffList *>file_buffer_;
	};
}
/*
namespace std
{
	
	struct greater<YiXinVoiceBuff *> : binary_function<const YiXinVoiceBuff *, const YiXinVoiceBuff *, bool>
	{
		bool operator()(const YiXinVoiceBuff *&t1, const YiXinVoiceBuff *&t2)
		{	
			return t1->offset < t2->offset;
		}
	};
}
*/

#endif