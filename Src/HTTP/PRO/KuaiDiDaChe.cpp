
#include "StdAfx.h"
#include "KuaiDiDaChe.h"

KuaiDiDaChe::KuaiDiDaChe()
{
	pro_ = "kuaidi";
	software_ = "快的打车";
}

KuaiDiDaChe::~KuaiDiDaChe()
{
	
}

int KuaiDiDaChe::Is(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("c2.kuaidadi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/taxi/a/js.do")!=-1))
	{
		return kCMD_a;
	}

	if((http_session->m_Requestline.m_Host.Find("c2.kuaidadi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/taxi/p/js.do")!=-1))
	{
		return kCMD_p;
	}

	if((http_session->m_Requestline.m_Host.Find("c2.kuaidadi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/taxi/d/js.do")!=-1))
	{
		return kCMD_d;
	}

	if((http_session->m_Requestline.m_Host.Find("c2.kuaidadi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/taxi/p/form.do")!=-1))
	{
		return kNewVoiceOrder;
	}


	return 0;
}

int KuaiDiDaChe::Process(const char *packet,int action)
{
	if (action == kCMD_p)
	{
		char *hd2 = strstr(packet,"{\"cmd\":30001");
		if (hd2)
		{
			action = kNewTextOrder;
			return NewTextOrder(hd2);
		}

		char *hd5 = strstr(packet,"{\"cmd\":30015");
		if (hd5)
		{
			action = kNewTextOrder;
			return PreOrder(hd5);
		}
		return 0;
	}

	if (action == kCMD_d)
	{
		char *hd3 = strstr(packet,"{\"cmd\":20006");
		if (hd3)
		{
			action = kDriverRegister;
			return DriverRegister(hd3);
		}
	}

	if (action == kCMD_a)
	{
		char *hd4 = strstr(packet,"{\"cmd\":10004");
		if (hd4)
		{
			action = kDriverLogin;
			return DriverLogin(hd4);
		}
		char *hd1 = strstr(packet,"{\"cmd\":50331");
		if (hd1)
		{
			action = kPassengerLogin;
			return PassengerLogin(hd1);
		}
		return 0;
	}

	if (action == kNewVoiceOrder)
	{
		return NewVoiceOrder(packet);
	}

	if (action == kPreOrder)
	{
		return PreOrder(packet);
	}


	return 0;
}

int KuaiDiDaChe::PassengerLogin(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	ms_->SplitStringToMap(src_data,',',':');
	
	string imsi = ms_->GetValueFromMapByKey("imsi");
	string imei = ms_->GetValueFromMapByKey("imei");
	
	string longitude = ms_->GetValueFromMapByKey("lng");
	string latitude = ms_->GetValueFromMapByKey("lat");

	
	if ( imei.empty()/* || imsi_.empty()*/)
	{
		return 0;
	}
	
	
	output_text_ = 
		"imsi=" + imsi + "\r\n" +
		"imei=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=上线\r\n";
	
	WriteIndexFile();

	return 1;
}

int KuaiDiDaChe::NewTextOrder(const char *packet)
{
	string src_data(packet);

	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');
	
	string phone = ms_->GetValueFromMapByKey("simmob");

	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	
	string longitude = ms_->GetValueFromMapByKey("flng");
	string latitude = ms_->GetValueFromMapByKey("flat");
	string from = ms_->GetValueFromMapByKey("from",true);
	string to = ms_->GetValueFromMapByKey("to",true);
	
	
	if (/*phone_.empty() || */imei.empty() || longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}
	
	ms_->Replace(phone,"+86","");
	
	output_text_ = 
		"user=" + phone + "\r\n" +
		"imei=" + imei + "\r\n" +
		"imsi=" + imsi + "\r\n" + 
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"webname=" + from + "\r\n" +
		"contact=" + to + "\r\n" +
		"state=叫车\r\n";
	
	WriteIndexFile();

	return 1;
}


int KuaiDiDaChe::NewVoiceOrder(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);

	string phone = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"simmob\"\r\n\r\n",boundary);
	
	string imei = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n\r\n",boundary);
	string imsi = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imsi\"\r\n\r\n",boundary);

	string longitude = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"flng\"\r\n\r\n",boundary);
	string latitude = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"flat\"\r\n\r\n",boundary);
	string from = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"from\"\r\n\r\n",boundary,true);
	string to = "(请听语音)";
	
	if (/*phone.empty() || */imei.empty() || /*imsi_.empty() ||*/longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}

	ms_->Replace(phone,"+86","");
	char *stream = strstr(packet,"#!AMR");
	if (stream)
	{
		int file_size = GetRealFileSize(stream,boundary.c_str());
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			string real_file_name = WriteTempRealFile(stream,"kuaidi_taxi_voice.amr",file_size);
			output_text_ = 
				"user=" + phone + "\r\n" +
				"imei=" + imei + "\r\n" +
				"imsi=" + imsi + "\r\n" +
				"from=" + longitude + "\r\n" +
				"to=" + latitude + "\r\n" +
				"webname=" + from + "\r\n" +
				"contact=" + to + "\r\n" +
				"state=叫车\r\n";
			WriteIndexFile(real_file_name);
		}
	}	
	return 1;
}

int KuaiDiDaChe::PreOrder(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');

	string phone = ms_->GetValueFromMapByKey("simmob");
	
	string imei = ms_->GetValueFromMapByKey("imei");

	string longitude = ms_->GetValueFromMapByKey("flng");
	string latitude = ms_->GetValueFromMapByKey("flat");
	string from = ms_->GetValueFromMapByKey("from",true);
	string to = ms_->GetValueFromMapByKey("to",true);

	string tm = ms_->GetValueFromMapByKey("stime");
	if (tm.size() > 10)
	{
		tm = tm.substr(0,10);
		time_t t = strtol(tm.c_str(),0,10);
		char tmp[64] = {0};
		strftime(tmp,64,"%Y-%m-%d %H:%M:%S",localtime(&t));
		tm = tmp;
	}


	if (imei.empty() || longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}

	ms_->Replace(phone,"+86","");
	
	
	output_text_ = 
		"user=" + phone + "\r\n" +
		"imei=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"webname=" + from + "\r\n" +
		"contact=" + to + "\r\n" +
		"netmark=" + tm + "\r\n" +
		"state=预订\r\n";

	WriteIndexFile();

	return 1;
}

int KuaiDiDaChe::DriverRegister(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');
	
	string name = ms_->GetValueFromMapByKey("name",true);
	string company = ms_->GetValueFromMapByKey("company",true);

	string carno = ms_->GetValueFromMapByKey("carno",true);
	string dno = ms_->GetValueFromMapByKey("dno");

	if (name.empty() || company.empty() ||  carno.empty() || dno.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"mid=" + name + "\r\n" +
		"oid=" + dno + "\r\n" +
		"msgtype=" + company + "\r\n" +
		"msg=" + carno  + "\r\n" +
		"state=司机注册\r\n";
	WriteIndexFile();
	return 1;
}

int KuaiDiDaChe::DriverLogin(const char *packet)
{

	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	ms_->SplitStringToMap(src_data,',',':');
	
	string phone_ = ms_->GetValueFromMapByKey("mob");
	string pass = ms_->GetValueFromMapByKey("pwd",true);
	string imei_ = ms_->GetValueFromMapByKey("imei");


	if (phone_.empty() || pass.empty() ||  imei_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"pass=" + pass + "\r\n" + 
		"imei=" + imei_ + "\r\n" +
		"state=司机上线\r\n";

	WriteIndexFile();
	return 1;
}

