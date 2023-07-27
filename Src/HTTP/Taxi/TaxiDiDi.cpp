
#include "StdAfx.h"
#include "TaxiDiDi.h"

map<string,string>taxi::DiDi::imei_phone_ = map<string,string>();

taxi::DiDi::DiDi(HttpSession *http_session, const string &pro_name) :  Taxi(http_session, pro_name)
{
}

taxi::DiDi::~DiDi()
{
	
}

int taxi::DiDi::IsDiDi(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find(".diditaxi.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("phone=")!=-1))
	{
		return kPassengerLogin;
	}

	if((http_session->m_Requestline.m_Host.Find("api.diditaxi.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/v2/p_neworder")!=-1))
	{
		return kNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("api.diditaxi.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/v2/p_preorder")!=-1))
	{
		return kPreOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("api.diditaxi.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/v2/d_register/submit_head")!=-1))
	{
		return kDriverRegister;
	}

	return 0;
}

int taxi::DiDi::Process(const char *packet,int action)
{
	if (action == kPassengerLogin)
	{
		return PassengerLogin(packet);
	}

	if (action == kNewOrder)
	{
		return NewOrder(packet);
	}

	if (action == kPreOrder)
	{
		return PreOrder(packet);
	}

	if (action == kDriverRegister)
	{
		return DriverRegister(packet);
	}

	return 0;
}

int taxi::DiDi::PassengerLogin(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string phone = ms_->GetValueFromMapByKey("phone");
	imei_ = ms_->GetValueFromMapByKey("imei");

	if (/*phone.empty() || */imei_.empty())
	{
		return 0;
	}

	if (imei_.length() > 15)
	{
		imei_ = imei_.substr(0, 15);
	}

	imei_phone_.insert(make_pair(imei_,phone));
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	if (longitude_ == "-1" || latitude_ == "-1")
	{
		return 0;
	}

	if (longitude_ == "0.0" || latitude_ == "0.0")
	{
		return 0;
	}

	output_text_ = 
		"user=" + phone + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=ÉÏÏß\r\n";

	WriteIndexFile();
	return 1;
}

int taxi::DiDi::NewOrder(const char *packet)
{
	if (http_session_->m_Requestline.m_strBoundary.IsEmpty())
	{
		return TextNewOrder(packet);
	}
	else
	{
		return VoiceNewOrder(packet);
	}
	return 0;
}

int taxi::DiDi::TextNewOrder(const char *packet)
{
	string src_uri(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(src_uri,'&','=');

	imei_ = ms_->GetValueFromMapByKey("imei");

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	from_ = ms_->GetValueFromMapByKey("from_name",true);
	to_ = ms_->GetValueFromMapByKey("to_name",true);
	
	if (imei_.empty() || longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}
	if (longitude_ == "-1" || latitude_ == "-1")
	{
		return 0;
	}

	if (longitude_ == "0.0" || latitude_ == "0.0")
	{
		return 0;
	}
	
	map<string,string>::iterator iter = imei_phone_.find(imei_);
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		 phone_ = iter->second;
	}
	
	
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"webname=" + from_ + "\r\n" +
		"contact=" + to_ + "\r\n" +
		"state=½Ð³µ\r\n";

	WriteIndexFile();

	return 1;
}

int taxi::DiDi::VoiceNewOrder(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	
	imei_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n\r\n",boundary);
	
	longitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lng\"\r\n\r\n",boundary);
	latitude_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lat\"\r\n\r\n",boundary);
	from_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"from_name\"\r\n\r\n",boundary,true);
	to_ = "(ÇëÌýÓïÒô)";
	
	if (imei_.empty() || longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}

	if (longitude_ == "-1" || latitude_ == "-1")
	{
		return 0;
	}

	if (longitude_ == "0.0" || latitude_ == "0.0")
	{
		return 0;
	}
	
	map<string,string>::iterator iter = imei_phone_.find(imei_);
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		phone_ = iter->second;
	}
	
	char *stream = strstr(packet,"#!AMR");
	if (stream)
	{
		int file_size = GetRealFileSize(stream,boundary.c_str());
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			string real_file_name = WriteTempRealFile(stream,"didi_taxi_voice.amr",file_size);
			output_text_ = 
				"user=" + phone_ + "\r\n" +
				"imei=" + imei_ + "\r\n" +
				"from=" + longitude_ + "\r\n" +
				"to=" + latitude_ + "\r\n" +
				"webname=" + from_ + "\r\n" +
				"contact=" + to_ + "\r\n" +
				"state=½Ð³µ\r\n";
			WriteIndexFile(real_file_name);
		}
	}	
	return 1;
}

int taxi::DiDi::PreOrder(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	from_ = ms_->GetValueFromMapByKey("from_name",true);
	to_ = ms_->GetValueFromMapByKey("to_name",true);
	string tm = ms_->GetValueFromMapByKey("time",true);

	if (imei_.empty() || longitude_.empty() || latitude_.empty() || from_.empty() || to_.empty())
	{
		return 0;
	}
	if (longitude_ == "-1" || latitude_ == "-1")
	{
		return 0;
	}

	if (longitude_ == "0.0" || latitude_ == "0.0")
	{
		return 0;
	}

	map<string,string>::iterator iter = imei_phone_.find(imei_);
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		phone_ = iter->second;
	}
	
	
	output_text_ = 
		"user=" + phone_ + "\r\n" +
		"imei=" + imei_ + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"webname=" + from_ + "\r\n" +
		"contact=" + to_ + "\r\n" +
		"netmark=" + tm + "\r\n" +
		"state=Ô¤¶©\r\n";

	WriteIndexFile();

	return 1;
}

int taxi::DiDi::DriverRegister(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string name = ms_->GetValueFromMapByKey("name",true);
	string company = ms_->GetValueFromMapByKey("company",true);
	string phone = ms_->GetValueFromMapByKey("phone");
	string license = ms_->GetValueFromMapByKey("license",true);
	string driver_card_no = ms_->GetValueFromMapByKey("driver_card_no");

	if (name.empty() || company.empty() || phone.empty() || license.empty() || driver_card_no.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + phone + "\r\n" +
		"mid=" + name + "\r\n" +
		"oid=" + driver_card_no + "\r\n" +
		"msgtype=" + company + "\r\n" +
		"msg=" + license  + "\r\n" +
		"state=Ë¾»ú×¢²á\r\n";

	WriteIndexFile();

	return 1;

}

int taxi::DiDi::DriverLogin(const char *packet)
{
	return 1;
}

