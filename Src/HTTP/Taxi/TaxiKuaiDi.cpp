
#include "StdAfx.h"
#include "TaxiKuaiDi.h"



taxi::KuaiDi::KuaiDi(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
	
}

taxi::KuaiDi::~KuaiDi()
{
	
}

int taxi::KuaiDi::IsKuaiDi(HttpSession *http_session)
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

int taxi::KuaiDi::Process(const char *packet,int action)
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
		char *hd6 = strstr(packet,"{\"cmd\":20006");
		if (hd6)
		{
			action = kDriverRegister;
			return DriverRegister(hd6);
		}

		char *hd3 = strstr(packet,"{\"cmd\":40001");
		if (hd3)
		{
			action = kDriverWorking;
			return DriverWorking(hd3);
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

int taxi::KuaiDi::PassengerLogin(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	ms_->SplitStringToMap(src_data,',',':');
	
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	
	if ( imei_.empty()/* || imsi_.empty()*/)
	{
		return 0;
	}
	
	
	output_text_ = 
		"imsi=" + imsi_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=上线\r\n";
	
	WriteIndexFile();

	return 1;
}

int taxi::KuaiDi::NewTextOrder(const char *packet)
{
	string src_data(packet);

	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');
	
	phone_ = ms_->GetValueFromMapByKey("simmob");

	imei_ = ms_->GetValueFromMapByKey("imei");
	imei_ = ms_->GetValueFromMapByKey("imsi");
	
	longitude_ = ms_->GetValueFromMapByKey("flng");
	latitude_ = ms_->GetValueFromMapByKey("flat");
	from_ = ms_->GetValueFromMapByKey("from",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	
	
	if (/*phone_.empty() || */imei_.empty() || longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}
	
	ms_->Replace(phone_,"+86","");
	
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"imsi=" + imsi_ + "\r\n" + 
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"webname=" + from_ + "\r\n" +
		"contact=" + to_ + "\r\n" +
		"state=叫车\r\n";
	
	WriteIndexFile();

	return 1;
}


int taxi::KuaiDi::NewVoiceOrder(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);

	phone_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"simmob\"\r\n\r\n",boundary);
	
	imei_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n\r\n",boundary);
	imsi_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n\r\n",boundary);

	longitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"flng\"\r\n\r\n",boundary);
	latitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"flat\"\r\n\r\n",boundary);
	from_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"from\"\r\n\r\n",boundary,true);
	to_ = "(请听语音)";
	
	if (phone_.empty() || imei_.empty() || /*imsi_.empty() ||*/longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}

	ms_->Replace(phone_,"+86","");
	char *stream = strstr(packet,"#!AMR");
	if (stream)
	{
		int file_size = GetRealFileSize(stream,boundary.c_str());
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			string real_file_name = WriteTempRealFile(stream,"kuaidi_taxi_voice.amr",file_size);
			output_text_ = 
				"user=" + phone_ + "\r\n" +
				"imei=" + imei_ + "\r\n" +
				"imsi=" + imei_ + "\r\n" +
				"from=" + longitude_ + "\r\n" +
				"to=" + latitude_ + "\r\n" +
				"webname=" + from_ + "\r\n" +
				"contact=" + to_ + "\r\n" +
				"state=叫车\r\n";
			WriteIndexFile(real_file_name);
		}
	}	
	return 1;
}

int taxi::KuaiDi::PreOrder(const char *packet)
{
	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');

	phone_ = ms_->GetValueFromMapByKey("simmob");
	
	imei_ = ms_->GetValueFromMapByKey("imei");

	longitude_ = ms_->GetValueFromMapByKey("flng");
	latitude_ = ms_->GetValueFromMapByKey("flat");
	from_ = ms_->GetValueFromMapByKey("from",true);
	to_ = ms_->GetValueFromMapByKey("to",true);

	string tm = ms_->GetValueFromMapByKey("stime");
	if (tm.size() > 10)
	{
		tm = tm.substr(0,10);
		time_t t = strtol(tm.c_str(),0,10);
		char tmp[64] = {0};
		strftime(tmp,64,"%Y-%m-%d %H:%M:%S",localtime(&t));
		tm = tmp;
	}


	if (imei_.empty() || longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}

	ms_->Replace(phone_,"+86","");
	
	
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"webname=" + from_ + "\r\n" +
		"contact=" + to_ + "\r\n" +
		"netmark=" + tm + "\r\n" +
		"state=预订\r\n";

	WriteIndexFile();

	return 1;
}

int taxi::KuaiDi::DriverRegister(const char *packet)
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

int taxi::KuaiDi::DriverLogin(const char *packet)
{

	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	ms_->SplitStringToMap(src_data,',',':');
	
	phone_ = ms_->GetValueFromMapByKey("mob");
	string pass = ms_->GetValueFromMapByKey("pwd",true);
	imei_ = ms_->GetValueFromMapByKey("imei");


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

int taxi::KuaiDi::DriverWorking(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	string src_data(packet);
	
	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->Replace(src_data,"\"","");
	
	
	ms_->SplitStringToMap(src_data,',',':');
	
	phone_ = ms_->GetValueFromMapByKey("simmob");
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	string carno = ms_->GetValueFromMapByKey("carno",true);

	if (imei_.empty() || carno.empty())
	{
		return 0;
	}


	output_text_ = 
		"msg=" + carno  + "\r\n" +
		"user=" + phone_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"imsi=" + imei_ + "\r\n" +
		"state=司机上班\n";

	WriteIndexFile();
	return 1;
}