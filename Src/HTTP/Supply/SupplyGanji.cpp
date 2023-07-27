#include "StdAfx.h"
#include "SupplyGanji.h"


supply::SupplyGanJi::SupplyGanJi(HttpSession *http_session, const string &pro_name):Supply(http_session,pro_name)
{
	app_ = "赶集";
}

supply::SupplyGanJi::~SupplyGanJi()
{

}

supply::SupplyGanJi::IsSupplyGanJi(HttpSession *http_session)
{
	//发布招聘信息
	if( ( http_session->m_Requestline.m_Host.Find("www.ganji.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/pub/pub.php?cid=2") != -1 ))	
	{
		return kWebPublic;
	}
	//兼职招聘

	if( ( http_session->m_Requestline.m_Host.Find("www.ganji.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/pub/pub.php?cid=3") != -1 ))	
	{
		return kWebPublicParttime;
	}
	return 0;
}

supply::SupplyGanJi::Process(const char *packet,int action)
{
	if (action == kWebPublic)
	{
		return WebPublic(packet);
	}

	if (action == kWebPublicParttime)
	{
		return WebPublicParttime(packet);
	}
	return 0;
}

int supply::SupplyGanJi::WebPublic(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	//职位名称
	key = bp.GetBoundaryKey("name=\"title\"");          
	bv = bp.GetBoundaryValue(key);	

	string title = ms_->GetValueBetweenBeginEnd(src_data,"name=\"title\"\r\n\r\n",boundary);
	ms_->Replace(title,"--","");
	ms_->Replace(title,"\r\n","");
	//招聘人数
	key = bp.GetBoundaryKey("name=\"need_num\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string num = ms_->GetValueBetweenBeginEnd(src_data,"name=\"need_num\"\r\n\r\n",boundary);
	ms_->Replace(num,"--","");
	ms_->Replace(num,"\r\n","");
	//地址
	key = bp.GetBoundaryKey("name=\"new_work_address\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string address = ms_->GetValueBetweenBeginEnd(src_data,"name=\"new_work_address\"",boundary);
	ms_->Replace(address,"--","");
	ms_->Replace(address,"\r\n","");
	//联系人
	key = bp.GetBoundaryKey("name=\"person\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string name = ms_->GetValueBetweenBeginEnd(src_data,"name=\"person\"",boundary);
	ms_->Replace(name,"--","");
	ms_->Replace(name,"\r\n","");
	//电话
	key = bp.GetBoundaryKey("name=\"phone\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string phone = ms_->GetValueBetweenBeginEnd(src_data,"name=\"phone\"",boundary);
	ms_->Replace(phone,"--","");
	ms_->Replace(phone,"\r\n","");
	//邮箱
	key = bp.GetBoundaryKey("name=\"email\"");
	bv = bp.GetBoundaryValue(key);

	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string email = ms_->GetValueBetweenBeginEnd(src_data,"name=\"email\"",boundary);
	ms_->Replace(email,"--","");
	ms_->Replace(email,"\r\n","");
	if (email.empty())
	{
		email = "null";
	}
	output_text_ = 
		"user=" + 		name + "\r\n" +			            //联系人
		"islogin=" + 	phone + "\r\n" + 					//电话
		"oid=" + 	  	email + "\r\n" + 					//邮箱
		"fileurl=" + 	title + "\r\n" + 					//职位名称
		"msgtype=" + 	 num  + "\r\n" +					//招聘人数
		"mid=" + 	  	address + "\r\n" + 					//地址
		"netmark=" +	app_ + "\r\n" + 					//来源
		"clientmsg=" +  platformWeb + "\r\n" +				//操作平台
		"state=招聘信息发布\r\n";							//状态
	WriteIndexFile();
	return 1;
}

int supply::SupplyGanJi::WebPublicParttime(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	//职位名称
	key = bp.GetBoundaryKey("name=\"title\"");          
	bv = bp.GetBoundaryValue(key);	
	
	string title = ms_->GetValueBetweenBeginEnd(src_data,"name=\"title\"\r\n\r\n",boundary);
	ms_->Replace(title,"--","");
	ms_->Replace(title,"\r\n","");
	//招聘人数
	key = bp.GetBoundaryKey("name=\"need_num\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string num = ms_->GetValueBetweenBeginEnd(src_data,"name=\"need_num\"\r\n\r\n",boundary);
	ms_->Replace(num,"--","");
	ms_->Replace(num,"\r\n","");
	//职位描述
	key = bp.GetBoundaryKey("name=\"description\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string description = ms_->GetValueBetweenBeginEnd(src_data,"name=\"description\"\r\n\r\n",boundary);
	ms_->Replace(description,"--","");
	ms_->Replace(description,"\r\n","");
	//地址
	key = bp.GetBoundaryKey("name=\"new_work_address\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string address = ms_->GetValueBetweenBeginEnd(src_data,"name=\"new_work_address\"",boundary);
	ms_->Replace(address,"--","");
	ms_->Replace(address,"\r\n","");
	//联系人
	key = bp.GetBoundaryKey("name=\"person\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string name = ms_->GetValueBetweenBeginEnd(src_data,"name=\"person\"",boundary);
	ms_->Replace(name,"--","");
	ms_->Replace(name,"\r\n","");
	//电话
	key = bp.GetBoundaryKey("name=\"phone\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string phone = ms_->GetValueBetweenBeginEnd(src_data,"name=\"phone\"",boundary);
	ms_->Replace(phone,"--","");
	ms_->Replace(phone,"\r\n","");
	//邮箱
	key = bp.GetBoundaryKey("name=\"email\"");
	bv = bp.GetBoundaryValue(key);
	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string email = ms_->GetValueBetweenBeginEnd(src_data,"name=\"email\"",boundary);
	ms_->Replace(email,"--","");
	ms_->Replace(email,"\r\n","");
	if (email.empty())
	{
		email = "null";
	}
	output_text_ = 
		"user=" + 		name + "\r\n" +			            //联系人
		"islogin=" + 	phone + "\r\n" + 					//电话
		"oid=" + 	  	email + "\r\n" + 					//邮箱
		"fileurl=" + 	title + "\r\n" + 					//职位名称
		"msgtype=" + 	num  + "\r\n" +						//数量
		"clientmac=" + 	description + "\r\n" + 				//兼职内容	
		"mid=" + 	  	address + "\r\n" + 					//地址
		"netmark=" +	app_ + "\r\n" + 					//来源
		"clientmsg=" +  platformWeb + "\r\n" +				//操作平台
		"state=兼职信息发布\r\n";							//状态
	WriteIndexFile();
	return 1;
}