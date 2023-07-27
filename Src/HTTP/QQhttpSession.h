#ifndef QQHTTPSESSION_H
#define QQHTTPSESSION_H

#define  QQ_HTTP_LOGIN					1
#define  QQ_HTTP_LOGOUT					2
#define  QQ_HTTP_BUDDY_MSG				3
#define  QQ_HTTP_QUN_MSG				4
#define  QQ_HTTP_SEARCHBUDDY			5
#define  QQ_HTTP_QUN_SRARCHBUDDY		6
#define  QQ_HTTP_ADDBUDDY				7
#define  QQ_HTTP_QUN_ATTACHBUDDY		8
#define	 QQ_HTTP_QUN_CREATE				9	
#define  QQ_HTTP_QUN_FILE_LIST			10
#define  QQ_HTTP_BUDDY_TRANSFILE		11
#define  QQ_HTTP_QUN_DOWNLOADFILE		12
#define  QQ_HTTP_QUN_UPLOADFILE			13
#define  QQ_HTTP_QUN_OWNERINFO			14


int IsQQhttpSession(HttpSession *pclassHttpSession);
int ProcessQQhttpSession( HttpSession *pclassHttpSession,const char* lpcstrPacket,int iValue);

#endif

