
#ifndef HTTP_EMAIL_EMAIL_H_H_H
#define HTTP_EMAIL_EMAIL_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
#include "../../gstdh.h"	
#include "../json/json.h"	

typedef struct _EMailAttachmentInfo
{
	//AuthenticationInfo authInfo;
	time_t createdTimestamp;
	string user;
	string attachmentName;
	string attachmentPath;
}EMailAttachmentInfo;

typedef struct _UploadFileBuffer
{
	_UploadFileBuffer()
	{
		offset = 0;
		length = 0;
		data = NULL;
	}
	
	~_UploadFileBuffer()
	{
		if (data != NULL)
		{
			delete [] data;
		}
	}
	int offset;
	int length;
	char *data;
}UploadFileBuffer;

typedef struct _UploadFileBufferList
{
	_UploadFileBufferList()
	{
		lastTime = 0;
		complete = false;
		fileSize = 0;
		recieve_size = 0;
	}
	~_UploadFileBufferList()
	{
		
	}
	
	AuthInfo authInfo;
	time_t lastTime;
	bool complete;
	string uid;
	string fileName;
	int fileSize;	
	int recieve_size; // add by cxl
	string operationType;
	list<UploadFileBuffer *>buffers;

} UploadFileBufferList;

namespace email
{
	class EMail : public HttpPro
	{
	public:
		EMail(HttpSession *http_session, const string &pro_name);
		virtual ~EMail();
		
		static int ProcessEMail(HttpSession *http_session ,const char *packet);
		
	protected:
		
		void ConstructSendMailOutputString();
		void ConstructUploadAttachmentOuptputString();
//		virtual string GetOutputText(int state);
//		int GetRealFileSize(const char *stream_begin,const char *stream_end);
		string user_;
		string from_;
		string to_;
		string title_;
		string content_;
		string attachment_;	
	};
}
#endif