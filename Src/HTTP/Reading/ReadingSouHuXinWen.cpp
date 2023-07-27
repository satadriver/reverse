
#include "StdAfx.h"

#include "ReadingSouHuXinWen.h"

reading::SouHuXinWen::SouHuXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::SouHuXinWen::~SouHuXinWen()
{
	
}

int reading::SouHuXinWen::IsSouHuXinWen(HttpSession *http_session)
{
	//pc ������	
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("news.sohu.com/") != -1))
	{
		return kAddCommentC;
	}
	//android��������
	if( http_session->m_Requestline.m_Host.Find("api.k.sohu.com") != -1  && 
	    http_session->m_Requestline.m_URI.Find("/api/comment/userComment.go") != -1)	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::SouHuXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentC)
	{
		return AddCommentC(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}

//pc��������
int reading::SouHuXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("pp_login_time");

	vector<string>item_list;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 
	while(1)
	{
		item_end_pos = tmp.find("|",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = tmp.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = tmp.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("|");
		}
		item_list.push_back(item);
	}

	if (item_list.size() > 2)
	{
		user_ = item_list.at(1);
	}
	
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = http_session_->m_Requestline.m_Referer.GetBuffer(0);

	if (content_.empty() || url_.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + user_ + "\r\n" +
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=����\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

int reading::SouHuXinWen::AddCommentM(const char *packet)
{		
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string osType,lng,lat,mac;
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�˺�
	user_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"passport\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);
    //os
	osType = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"osType\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);
    //����
	content_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"cont\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);
    //����
	lng = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"longitude\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);
	//γ��
	lat = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"latitude\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);
	//mac��ַ
	mac = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"macAaddress\"\r\nContent-Type: text/plain; charset=US-ASCII\r\nContent-Transfer-Encoding: 8bit\r\n\r\n",boundary);

	if (user_.empty() || content_.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + user_ + "\r\n" +      //�˺�
		"subject=" + osType + "\r\n" +  //����ϵͳ
		"servername=" + lng + "\r\n" + 	//����
		"appname=" + 	lat + "\r\n" + 	//γ��
		"islogin=" + 	mac + "\r\n" + 	//MAC��ַ
		"msg=" + content_ + "\r\n" +    //����
		"platform=�ֻ�\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}