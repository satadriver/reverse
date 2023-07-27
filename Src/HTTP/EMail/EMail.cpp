#include "StdAfx.h"
#include "EMail.h"
#include "EMailQQ.h"
#include "EMail163.h"
#include "EMail126.h"
#include "EMail139.h"
#include "EMailSina.h"
#include "EMail21CN.h"
#include "EMailSohu.h"
#include "EMailYeah.h"
#include "EMailwo.h"
#include "Email189.h"
#include "EMailTom.h"
#include "EMail188.h"

email::EMail::EMail(HttpSession *http_session, const string &pro_name):HttpPro(http_session, pro_name)
{
	
}

email::EMail::~EMail()
{
	
}

int email::EMail::ProcessEMail(HttpSession *http_session ,const char *packet)
{
	
	int action = -1;
	int n = 0;
	try
	{
		//QQ邮箱
		action = EMailQQ::IsEMailQQ(http_session);
		if (action)
		{
			n = 1;
			EMailQQ email_qq(http_session,"email_qq");
			return email_qq.Process(packet,action);
		}
		//163邮箱 163vip邮箱
		action =  EMail163::IsEMail163(http_session);
		if (action)
		{
			n = 2;
			EMail163 email_163(http_session,"email_163");
			email_163.Process(packet,action);
		}
		//126邮箱 126vip邮箱
		action =  EMail126::IsEMail126(http_session);
		if (action)
		{
			n = 3;
			EMail126 email_126(http_session,"email_126");
			email_126.Process(packet,action);
		}
		//139邮箱
		action =  EMail139::IsEMail139(http_session);
		if (action)
		{
			n = 4;
			EMail139 email_139(http_session,"email_139");
			return email_139.Process(packet,action);
		}
		//新浪邮箱
		action = EMailSina::IsEMailSina(http_session);
		if (action)
		{
			n = 5;
			EMailSina email_sina(http_session,"email_sina");
			return email_sina.Process(packet,action);
		}
		//21CN邮箱
		action = EMail21CN::IsEMail21CN(http_session);
		if (action)
		{
			n = 6;
			EMail21CN email_21cn(http_session,"email_21cn");
			return email_21cn.Process(packet,action);
		}
		//Yean邮箱
		action = EMailYeah::IsEMailYeah(http_session);
		if (action)
		{
			n = 7;
			EMailYeah yeah(http_session,"email_yeah");
			yeah.Process(packet,action);
		}
        //搜狐邮箱 (2017-06-02 https 无法解析)
		action = EMailSohu::IsEMailSohu(http_session);
		if (action)
		{
			n = 8;
			EMailSohu sohu(http_session,"email_sohu");
			return sohu.Process(packet,action);
		}
		//wo邮箱
		action = EMailWo::IsEMailWo(http_session);
		if (action)
		{
			n = 9;
			EMailWo wo(http_session,"email_wo");
			return wo.Process(packet,action);
		}	
		//189邮箱
		action = EMail_189::IsEMail_189(http_session);
		if (action)
		{
			n = 10;
			EMail_189 email_189(http_session,"email_189");
			return email_189.Process(packet,action);
		}	
        //188财富邮邮箱
		action = EMail188::IsEMail188(http_session);
		if (action)
		{
			n = 11;
			EMail188 email_188(http_session,"email_188");
			return email_188.Process(packet,action);
		}		
		//Tom邮箱
		action = EMailTom::IsEMailTom(http_session);
		if (action)
		{
			n = 12;
			EMailTom email_tom(http_session,"email_tom");
			return email_tom.Process(packet,action);
		}	
	}
	catch(...)
	{
		WriteLog("dce2","ProcessEMail Err : %d",n);
	}
	
	return 0;
}

void email::EMail::ConstructSendMailOutputString()
{
	if (attachment_.empty())					
	{
		output_text_ = 
			"user=" + from_ + "\r\n" +			//邮箱账号
			"from=" + from_ + "\r\n" +          //发件人
			"to=" +   to_ + "\r\n" +			//收件人
			"nick=" + title_ + "\r\n" +         //主题
			"msg=" +  content_ + "\r\n" +       //内容
			"state=发送邮件" + "\r\n";
	}
	else										
	{
		output_text_ = 
			"user=" + from_ + "\r\n" +			//邮箱账号
			"from=" + from_ + "\r\n" +			//发件人
			"to=" +   to_ + "\r\n" +			//收件人
			"nick=" + title_ + "\r\n" +			//主题
			"msg=" +  content_ + "\r\n" +		//内容
			"onick=" +attachment_ + "\r\n" +	//附件名
			"state=发送邮件" + "\r\n";
	}		
}

void email::EMail::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"from=" +	from_ + "\r\n" + 
		"onick=" +	attachment_ + "\r\n" + 
		"state=上传附件\r\n";
}

/*
int email::EMail::GetRealFileSize(const char *stream_begin,const char *stream_end)
{
	int len = 0;
	while(1)
	{
		if (memcmp(stream_begin,stream_end,strlen(stream_end)) == 0)
		{
			return len;
		}
		else
		{
			len++;
			stream_begin++;
			if (len > real_file_max_size_)
			{
				return 0;
			}
		}
	}
	return 0;
}
*/









