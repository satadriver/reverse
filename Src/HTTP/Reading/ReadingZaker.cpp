#include "StdAfx.h"
#include "ReadingZaker.h"

reading::Zaker::Zaker(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::Zaker::~Zaker()
{
	
}

int reading::Zaker::IsZaker(HttpSession *http_session)
{
	//android ��¼
	if(  ( http_session->m_Requestline.m_Host.Find("api.myzaker.com")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("/zakeruser/sync_user_action.php") != -1 ))	
	{
		return kAndroidLogin;
	}
	//android��������
	if(  ( http_session->m_Requestline.m_Host.Find("c.myzaker.com")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("/weibo/api_post_article.php?act=reply") != -1 ))	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::Zaker::Process(const char *packet,int action)
{
	//android ��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android ����
	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}

//android�� ��¼
int reading::Zaker::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//�Զ���
	string uid,net,lng,lat,city,os,mac;
	//�û�id (uid _uid)
	uid = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"uid\"\r\n\r\n",boundary,true);
	//��·���� (����)
	net = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_net\"\r\n\r\n",boundary,true);
	//����
	lng = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lng\"\r\n\r\n",boundary,true);
	//γ��
	lat = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lat\"\r\n\r\n",boundary,true);
	//����(ƴ����д)
	city = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_city\"\r\n\r\n",boundary,true);
	//����ϵͳ
	os = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_os\"\r\n\r\n",boundary,true);
	//mac��ַ
	mac = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_mac\"\r\n\r\n",boundary,true);

	output_text_ =
		"user=" + 		uid + "\r\n" +	//�˺�
		"libname=" + 	net + "\r\n" + 	//��������
		"servername=" + lng + "\r\n" + 	//����
		"appname=" + 	lat + "\r\n" + 	//γ��
		"msgtype=" + 	city + "\r\n" +	//����
		"subject=" + 	os + "\r\n" + 	//����ϵͳ 
		"islogin=" + 	mac + "\r\n" + 	//MAC��ַ
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//android�� ����
int reading::Zaker::AddCommentM(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//�Զ���
	string uid,net,lng,lat,city,os,mac,content;
	//�û�id
	uid = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_uid\"\r\n\r\n",boundary,true);
	//��·����
	net = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_net\"\r\n\r\n",boundary,true);
	//����
	lng = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lng\"\r\n\r\n",boundary,true);
	//γ��
	lat = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lat\"\r\n\r\n",boundary,true);
	//����(ƴ����д)
	city = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_city\"\r\n\r\n",boundary,true);
	//����ϵͳ
	os = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_os\"\r\n\r\n",boundary,true);
	//mac��ַ
	mac = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_mac\"\r\n\r\n",boundary,true);
    //��������
	content= ms_-> GetValueBetweenBeginEnd(src_data,"name=\"content\"\r\n\r\n",boundary,true);
	output_text_ =
		"user=" + 		uid + "\r\n" +		//�˺�
		"libname=" + 	net + "\r\n" + 		//��������
		"servername=" + lng + "\r\n" + 		//����
		"appname=" + 	lat + "\r\n" + 		//γ��
		"msgtype=" + 	city + "\r\n" +		//����
		"subject=" + 	os + "\r\n" + 		//����ϵͳ 
		"islogin=" + 	mac + "\r\n" + 		//MAC��ַ
		"msg=" + 	  	content + "\r\n" +  //����
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
