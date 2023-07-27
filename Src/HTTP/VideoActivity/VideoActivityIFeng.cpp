#include "StdAfx.h"
#include "VideoActivityIFeng.h"

videoactivity::IFengVideo::IFengVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "�����Ƶ";	
}

videoactivity::IFengVideo::~IFengVideo()
{
	
}

int videoactivity::IFengVideo::IsIFengVideo(HttpSession *http_session)
{
	//��ҳ���� 1
	if((http_session->m_Requestline.m_Host.Find("so.v.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� 2
	if((http_session->m_Requestline.m_Host.Find("so.v.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.action")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ����
	if((http_session->m_Requestline.m_Host.Find("comment.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/postv.php")!=-1)) 
	{
		return kPcWebComment;
	}
	//android����
	if((http_session->m_Requestline.m_Host.Find("vcis.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/searchWeMedia")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android���� 
	if((http_session->m_Requestline.m_Host.Find("comment.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wappost")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::IFengVideo::Process(const char *packet,int action)
{
	//��ҳ����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//��ҳ����
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//android����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//Android���� �ظ�
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}

//��ҳ����
int videoactivity::IFengVideo::PcWebSearch(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("q",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//��ҳ����
int videoactivity::IFengVideo::PcWebComment(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);  
	if (comment.empty())
	{
		return 0;
	}	
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android����
int videoactivity::IFengVideo::AndroidSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//�û�ID ����
	string userID = ms_->GetValueFromMapByKey("userID");
	//����
	string comment = ms_->GetValueFromMapByKey("keyword",true);  
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + 	  	userID + "\r\n" + 			//54	[119] //�û�ID
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android���� 
int videoactivity::IFengVideo::AndroidComment(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//��Ƶ����
	string docName = ms_->GetValueFromMapByKey("docName",true);
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
// 	//�û��� uri �ض�
// 	string username;
// 	string ext2 = ms_->GetValueFromMapByKey("ext2",true);
// 	//ʹ��json ��ȡ����
// 	Json::Reader reader;
// 	Json::Value value;
// 	if (reader.parse(ext2,value))
// 	{	
// 		username = value["nickname"].asString();	
// 	}
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"mid=" + 	  	docName + "\r\n" + 			//56	[121] //��Ƶ����
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=���ۻ�ظ�\r\n";					
	WriteIndexFile();
	return 1;
}
