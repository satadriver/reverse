#ifndef HTTP_PRO_H_H_H
#define HTTP_PRO_H_H_H

#include "http.h"
#include "HttpSession.h"
#include "StringManipulation.h"
#include "ProtocolConfig.h"

class HttpPro
{
public:
	HttpPro(HttpSession *http_session, const string &pro_name);
	HttpPro();
	virtual ~HttpPro();
	virtual int Process(const char *packet,int action) = 0;
	int Process2();
	virtual int Is(HttpSession *http_session);
	void SetHttpSession(HttpSession *http_session)
	{
		http_session_ = http_session;
	}
protected:	
	void WriteIndexFile(const string &real_file_name = "");
	void WriteIndexFile1();
	string WriteTempRealFile(const char *stream,const char *file_name,int file_size);
	void static WriteLog(const char *module,const char *txt,...);
	void WriteVirtualIdentity(const string &username,const string &state);
	void WriteCaseInfo(const string &content);
	bool Unrepeated(map<string, time_t>&unreapted, const string &content);
	string GetPostData_tmp(const char *packet);
	int GetRealFileSize(const char *stream_begin,const char *stream_end);
protected:
	ProtocolConfig *pro_con_;
	StringManipulation *ms_;
	HttpSession *http_session_;
	int real_file_max_size_;	//限制文件的最大为5M
	int unrepeated_time_;
	string pro_name_;
	string output_text_;
	string pro_;
	string software_;
};
#endif