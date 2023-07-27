#include "StdAfx.h"

#include "ForumCSDN.h"

map<string,string>forum::CSDN::map_ = map<string,string>();

forum::CSDN::CSDN(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::CSDN::~CSDN()
{
	
}

int forum::CSDN::IsCSDN(HttpSession *http_session)
{
	
	if( (http_session->m_Requestline.m_Host.Find("ask.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/questions/create_question.json")!=-1))
	{
		return kAddPostCW;
	}

	if((http_session->m_Requestline.m_Host.Find("bbs.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wap/posts")!=-1))
	{
		return kaddCommentM;
	}

	if ((http_session->m_Requestline.m_Host.Find("bbs.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/topics")!=-1))
	{
		
		return kAddPost;
	}

	if((http_session->m_Requestline.m_Host.Find("ask.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/answers")!=-1))
	{
		return kAddCommentCW;
	}

	if((http_session->m_Requestline.m_Host.Find("bbs.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/posts")!=-1))
	{
		return kaddComment;
	}

	if( (http_session->m_Requestline.m_Host.Find("ms.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/login")!=-1))
	{
		return MLoginW;
	}

	if((http_session->m_Requestline.m_Host.Find("ms.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ask/create_question")!=-1))
	{
		return kAddPostM;
	}

	if((http_session->m_Requestline.m_Host.Find("ms.csdn.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("ask/create_answer")!=-1))
	{
		return kAddCommentM;
	}

	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mo/q/apubpost")!=-1))
	{
		return kAddMW;
	}



	return 0;
}

int forum::CSDN::Process(const char *packet,int action)
{
	if (action == kAddPostCW)
	{
		return AddPostCW(packet);
	}

	if (action == kAddCommentCW)
	{
		return AddCommentCW(packet);
	}

	if (action == MLoginW)
	{
		return LoginW(packet);
	}

	if (action == kAddPostM)
	{
		return AddPostM(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}

	if (action == kAddMW)
	{
		return AddMW(packet);
	}
	if (action==kAddPost)
	{
		return AddPost(packet);
	}
	if (action==kaddComment)
	{
		return AddComment(packet);
	}
	if (action==kaddCommentM)
	{
		return addCommentM(packet);
	}
	return 0;
}

//电脑浏览器发帖
int forum::CSDN::AddPostCW(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	title_ = ms_->GetValueFromMapByKey("question[title]",true);
	content_ = ms_->GetValueFromMapByKey("question[body]",true);
	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	
	string username = GetBDUSS();
	WriteCaseInfo(content_);
	
	output_text_ = 
		"user=" + username + "\r\n" + 
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=发帖\r\n";
	WriteIndexFile();
	return 1;
}

int forum::CSDN::AddPost(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	title_ = ms_->GetValueFromMapByKey("topic[title]",true);
	content_ = ms_->GetValueFromMapByKey("topic[body]",true);
	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	string username = GetBDUSS();
	WriteCaseInfo(content_);

	output_text_ = 
		"user=" + username + "\r\n" + 
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=发帖\r\n";
	WriteIndexFile();
	return 1;
}

//电脑浏览器跟帖
int forum::CSDN::AddCommentCW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');	
	content_ = ms_->GetValueFromMapByKey("answer[body]",true);
	if ( content_.empty() )
	{
		return 0;
	}
	string username =GetBDUSS();	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=跟帖\r\n";	
	WriteIndexFile();
	return 1;
}

int forum::CSDN::AddComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	content_=ms_->GetValueFromMapByKey("post[body]",true);
	if (content_.empty())
	{
		return 0;
	}
	string username=GetBDUSS();
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=电脑浏览器\r\n" +
		"state=跟帖\r\n";	
	WriteIndexFile();
	return 1;
}
//手机登录
int forum::CSDN::LoginW(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string pass;
	user_=ms_->GetValueFromMapByKey("username",true);
	pass=ms_->GetValueBetweenBeginEnd(src_data,"password=","&");
	if (user_.empty() || pass.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" +
		"platform=手机客户端\r\n" +
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}
//手机app发帖
int forum::CSDN::AddPostM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	title_=ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("body",true);
	
	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	WriteCaseInfo(content_);
	output_text_ = 
		"msgtype=" + title_ +"\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=手机App\r\n" +
		"state=发帖\r\n";	
	WriteIndexFile();
	return 1;
}
//手机app跟帖
int forum::CSDN::AddCommentM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	content_=ms_->GetValueFromMapByKey("body",true);
	if (content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ = 
		"msg=" + content_ + "\r\n" + 
		"platform=手机App\r\n" +
		"state=跟帖\r\n";
	WriteIndexFile();
	return 1;
}
int forum::CSDN::addCommentM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("post[body]",true);
	if (content_.empty())
	{
		return 0;
	}
	string username=GetBDUSS();
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username +"\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=手机App\r\n" +
		"state=跟帖\r\n";	
	WriteIndexFile();
	return 1;
}
//手机浏览器发帖 / 跟贴
int forum::CSDN::AddMW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	bar_name_ = ms_->GetValueFromMapByKey("word",true);
	//标题
	title_ = ms_->GetValueFromMapByKey("ti",true);
	//信息内容
	content_ = ms_->GetValueFromMapByKey("co",true);
	string post_id = ms_->GetValueFromMapByKey("z");
	string username = GetUserName(GetBDUSS());
	if (bar_name_.empty() || content_.empty())
	{
		return 0;
	}
	if (!title_.empty() && post_id.empty())
	{
		output_text_ = 
			"msgtype=" + title_ + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"platform=手机浏览器\r\n" +
			"state=发帖\r\n";
		WriteIndexFile();
		return 1;
	}
	if (title_.empty() && !post_id.empty())
	{
		//post_id = "http://tieba.baidu.com/p/" + post_id;
		output_text_ = 
			"msg=" + content_ + "\r\n" +
			"platform=手机浏览器\r\n" +
			"state=跟帖\r\n";
		WriteIndexFile();
		return 1;
	}
	return 0;
	
}

//手机浏览器
string forum::CSDN::GetBDUSS()
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie=ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie,';','=');
	return  ms_->GetValueFromMapByKey("UserName");	
}

string forum::CSDN::GetUserName(const string &bduss)
{
	map<string,string>::iterator iter = map_.find(bduss);
	if (iter != map_.end())
	{
		return iter->second;
	}
	return "";
}

void forum::CSDN::ReadBDUSS()
{
	const char *path = "e:\\netspy\\para\\bduss.txt";
	FILE *fp = fopen(path,"r");
	if (fp)
	{
		while ( !feof(fp))
		{
			char tmp[512] = {0};
			fgets(tmp,512,fp);
			string raw(tmp);
			string::size_type pos = raw.find(":");
			if (pos != string::npos)
			{
				string bduss = raw.substr(0,pos);
				string username = raw.substr(pos + 1);
				string::size_type posrn = username.find("\n");
				if (posrn != string::npos)
				{
					username = username.substr(0,posrn);
				}

				map_.insert(std::make_pair(bduss,username));

			}
		}		
		fclose(fp);
	}
}

void forum::CSDN::WriteBDUSS(const string &bduss, const string &username)
{
	const char *path = "e:\\netspy\\para\\bduss.txt";
	FILE *fp = fopen(path,"a+");
	if (fp)
	{
		string tmp = bduss + ":" + username + "\n";
		fwrite(tmp.c_str(),1,tmp.size(),fp);
		fclose(fp);
	}
}