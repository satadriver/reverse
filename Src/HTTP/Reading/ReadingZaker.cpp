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
	//android 登录
	if(  ( http_session->m_Requestline.m_Host.Find("api.myzaker.com")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("/zakeruser/sync_user_action.php") != -1 ))	
	{
		return kAndroidLogin;
	}
	//android评论内容
	if(  ( http_session->m_Requestline.m_Host.Find("c.myzaker.com")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("/weibo/api_post_article.php?act=reply") != -1 ))	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::Zaker::Process(const char *packet,int action)
{
	//android 登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android 评论
	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}

//android端 登录
int reading::Zaker::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary 直接提取
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//自定义
	string uid,net,lng,lat,city,os,mac;
	//用户id (uid _uid)
	uid = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"uid\"\r\n\r\n",boundary,true);
	//网路类型 (备用)
	net = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_net\"\r\n\r\n",boundary,true);
	//经度
	lng = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lng\"\r\n\r\n",boundary,true);
	//纬度
	lat = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lat\"\r\n\r\n",boundary,true);
	//城市(拼音简写)
	city = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_city\"\r\n\r\n",boundary,true);
	//操作系统
	os = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_os\"\r\n\r\n",boundary,true);
	//mac地址
	mac = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_mac\"\r\n\r\n",boundary,true);

	output_text_ =
		"user=" + 		uid + "\r\n" +	//账号
		"libname=" + 	net + "\r\n" + 	//网络类型
		"servername=" + lng + "\r\n" + 	//经度
		"appname=" + 	lat + "\r\n" + 	//纬度
		"msgtype=" + 	city + "\r\n" +	//城市
		"subject=" + 	os + "\r\n" + 	//操作系统 
		"islogin=" + 	mac + "\r\n" + 	//MAC地址
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//android端 评论
int reading::Zaker::AddCommentM(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary 直接提取
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//自定义
	string uid,net,lng,lat,city,os,mac,content;
	//用户id
	uid = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_uid\"\r\n\r\n",boundary,true);
	//网路类型
	net = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_net\"\r\n\r\n",boundary,true);
	//经度
	lng = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lng\"\r\n\r\n",boundary,true);
	//纬度
	lat = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_lat\"\r\n\r\n",boundary,true);
	//城市(拼音简写)
	city = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_city\"\r\n\r\n",boundary,true);
	//操作系统
	os = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_os\"\r\n\r\n",boundary,true);
	//mac地址
	mac = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"_mac\"\r\n\r\n",boundary,true);
    //评论内容
	content= ms_-> GetValueBetweenBeginEnd(src_data,"name=\"content\"\r\n\r\n",boundary,true);
	output_text_ =
		"user=" + 		uid + "\r\n" +		//账号
		"libname=" + 	net + "\r\n" + 		//网络类型
		"servername=" + lng + "\r\n" + 		//经度
		"appname=" + 	lat + "\r\n" + 		//纬度
		"msgtype=" + 	city + "\r\n" +		//城市
		"subject=" + 	os + "\r\n" + 		//操作系统 
		"islogin=" + 	mac + "\r\n" + 		//MAC地址
		"msg=" + 	  	content + "\r\n" +  //内容
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}
