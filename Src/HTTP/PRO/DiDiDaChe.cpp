
#include "StdAfx.h"
#include "DiDiDaChe.h"

map<string,string>DiDiDaChe::imei_phone_ = map<string,string>();

DiDiDaChe::DiDiDaChe()
{
	software_ = "嘀嘀打车";
	pro_ = "ddtaxi";
}

DiDiDaChe::~DiDiDaChe()
{
	
}

int DiDiDaChe::Is(HttpSession *http_session)
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

int DiDiDaChe::Process(const char *packet,int action)
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

int DiDiDaChe::PassengerLogin(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string phone = ms_->GetValueFromMapByKey("phone");
	string imei = ms_->GetValueFromMapByKey("imei");

	if (/*phone.empty() || */imei.empty())
	{
		return 0;
	}

	imei_phone_.insert(make_pair(imei,phone));
	
	string longitude = ms_->GetValueFromMapByKey("lng");
	string latitude = ms_->GetValueFromMapByKey("lat");

	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}

	if (longitude == "-1" || latitude == "-1")
	{
		return 0;
	}

	if (longitude == "0.0" || latitude == "0.0")
	{
		return 0;
	}

	output_text_ = 
		"user=" + phone + "\r\n" +
		"imei=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=上线\r\n";

	WriteIndexFile();
	return 1;
}

int DiDiDaChe::NewOrder(const char *packet)
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

int DiDiDaChe::TextNewOrder(const char *packet)
{
	string src_uri(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(src_uri,'&','=');

	string imei = ms_->GetValueFromMapByKey("imei");

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string longitude = ms_->GetValueFromMapByKey("lng");
	string latitude = ms_->GetValueFromMapByKey("lat");
	string from = ms_->GetValueFromMapByKey("from_name",true);
	string to = ms_->GetValueFromMapByKey("to_name",true);
	
	if (imei.empty() || longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}
	if (longitude == "-1" || latitude == "-1")
	{
		return 0;
	}

	if (longitude == "0.0" || latitude == "0.0")
	{
		return 0;
	}
	
	map<string,string>::iterator iter = imei_phone_.find(imei);
	string phone;
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		 phone = iter->second;
	}
	
	
	output_text_ = 
		"user=" + phone + "\r\n" +
		"imei=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"webname=" + from + "\r\n" +
		"contact=" + to + "\r\n" +
		"state=叫车\r\n";

	WriteIndexFile();

	return 1;
}

int DiDiDaChe::VoiceNewOrder(const char *packet)
{
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	
	string imei = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"imei\"\r\n\r\n",boundary);
	
	string longitude = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lng\"\r\n\r\n",boundary);
	string latitude = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"lat\"\r\n\r\n",boundary);
	string from = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"from_name\"\r\n\r\n",boundary,true);
	string to = "(请听语音)";
	
	if (imei.empty() || longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}

	if (longitude == "-1" || latitude == "-1")
	{
		return 0;
	}

	if (longitude == "0.0" || latitude == "0.0")
	{
		return 0;
	}
	
	map<string,string>::iterator iter = imei_phone_.find(imei);
	string phone;
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		phone = iter->second;
	}
	
	char *stream = strstr(packet,"#!AMR");
	if (stream)
	{
		int file_size = GetRealFileSize(stream,boundary.c_str());
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			string real_file_name = WriteTempRealFile(stream,"didi_taxi_voice.amr",file_size);
			output_text_ = 
				"user=" + phone + "\r\n" +
				"imei=" + imei + "\r\n" +
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

int DiDiDaChe::PreOrder(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string imei = ms_->GetValueFromMapByKey("imei");
	string longitude = ms_->GetValueFromMapByKey("lng");
	string latitude = ms_->GetValueFromMapByKey("lat");
	string from = ms_->GetValueFromMapByKey("from_name",true);
	string to = ms_->GetValueFromMapByKey("to_name",true);
	string tm = ms_->GetValueFromMapByKey("time",true);

	if (imei.empty() || longitude.empty() || latitude.empty() || from.empty() || to.empty())
	{
		return 0;
	}
	if (longitude == "-1" || latitude == "-1")
	{
		return 0;
	}

	if (longitude == "0.0" || latitude == "0.0")
	{
		return 0;
	}

	map<string,string>::iterator iter = imei_phone_.find(imei);
	string phone;
	if (iter == imei_phone_.end())
	{
//		return 0;
	}
	else
	{
		phone = iter->second;
	}
	
	
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

int DiDiDaChe::DriverRegister(const char *packet)
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
		"state=司机注册\r\n";

	WriteIndexFile();

	return 1;

}