#include "StdAfx.h"
#include "Employ58TongCheng.h"


employ::TongCheng58::TongCheng58(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "58同城";	
}

employ::TongCheng58::~TongCheng58()
{
	
}

int employ::TongCheng58::IsTongCheng58(HttpSession *http_session)
{
	//全职简历基本信息 
	if((http_session->m_Requestline.m_Host.Find("jianli.58.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addnewresumeV2")!=-1) )
	{
		return kPcWebPaMain;
	}
	//兼职简历基本信息
	if (http_session->m_Requestline.m_Host.Find("jianli.58.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/addnewresume") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/addnewresumeV2") == -1)
	{
		return kPcWebPaMain2;
	}
	//android基本信息(已不可解) 2017-05-12 更新
	if((http_session->m_Requestline.m_Host.Find("jlwebapp.58.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addnewresume")!=-1) )
	{
		return kAndroidPaMain;
	}
	return 0;
}

int employ::TongCheng58::Process(const char *packet,int action)
{
	//全职简历 新建 修改基本信息
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//兼职简历 新建 修改基本信息
	if (action == kPcWebPaMain2)
	{
		return PcWebPaMain2(packet);
	}
	//Android基本信息
	if (action == kAndroidPaMain)
	{
		return AndroidPaMain(packet);
	}
	return 1;
}

//全职简历 新建 修改基本信息
int employ::TongCheng58::PcWebPaMain(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string operate = ms_->GetValueBetweenBeginEnd(src_data,"<operate>","</operate>",true);
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (resumeid.empty() && operate == "resumeinfo")
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		
		output_text_ =				
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"netmark=" + app_ + "\r\n" +    	//来源
			"state=全职简历新建基本信息\r\n";
		WriteIndexFile();    
	}
	else if (!resumeid.empty() && operate == "resumeinfo")
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//现居住地		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<usercity>","</usercity>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//电子邮箱
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "无";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期			
			"oid=" + 	  	mobile + "\r\n" + 	//手机号	
			"netmark=" + app_ + "\r\n" + 		//来源
			"state=全职简历修改基本信息\r\n";
		WriteIndexFile();    
	}
	return 1;
}

//兼职简历 新建 修改 基本信息
int employ::TongCheng58::PcWebPaMain2(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string operate = ms_->GetValueBetweenBeginEnd(src_data,"<operate>","</operate>",true);
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (operate == "partresumeinfo" && resumeid == "0")
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		output_text_ =				
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"netmark=" + app_ + "\r\n" +    	//来源
			"state=兼职简历新建基本信息\r\n";
		WriteIndexFile();    
	}
	else if (operate == "partresumeinfo" && resumeid != "0") 
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//现居住地		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<usercity>","</usercity>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//电子邮箱
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "无";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期			
			"oid=" + 	  	mobile + "\r\n" + 	//手机号	
			"netmark=" + app_ + "\r\n" + 		//来源
			"state=兼职简历修改基本信息\r\n";
		WriteIndexFile();    
	}
	return 0;
}
//android基本信息
int employ::TongCheng58::AndroidPaMain(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //判断 新增 修改	
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (resumeid == "0")
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"netmark=" + app_ + "\r\n" + 		//来源
			"state=新建基本信息\r\n";
		WriteIndexFile();    
	}
	else if (!resumeid.empty() && resumeid != "0")
	{
		//姓名
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//性别
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "男";
		}
		if (gender == "1")
		{
			gender = "女";
		}
		//出生年月		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//手机号码
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//现居住地		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<native>","</native>",true);
		//电子邮箱
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "无";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//姓名	       
			"sex=" + 	  	gender + "\r\n" + 	//性别		
			"msg=" + 	  	birth + "\r\n" + 	//出生日期	
			"nick="	 + 	  	address + "\r\n" + 	//地址		
			"oid=" + 	  	mobile + "\r\n" + 	//手机号
			"mid=" + 	  	email + "\r\n" + 	//邮箱	
			"netmark=" + app_ + "\r\n" + 	    //来源
			"state=修改基本信息\r\n";
		WriteIndexFile();    
	}
	return 1;
}

