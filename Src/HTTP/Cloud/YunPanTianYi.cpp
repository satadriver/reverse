
#include "StdAfx.h"
#include "YunPanTianYi.h"

yunpan::YunPanTianYi::YunPanTianYi(HttpSession *http_session, const string &pro_name)
 : YunPan(http_session,pro_name)
{

}

yunpan::YunPanTianYi::~YunPanTianYi()
{

}

yunpan::YunPanTianYi::IsYunPanTianYi(HttpSession *http_session)
{
	//web���ϴ�
	if( ( http_session->m_Requestline.m_Host.Find("hd02.upload.cloud.189.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v1/DCIWebUploadAction") != -1 ))	
	{
		return kUploadFileCW;
	} 
	//web������ android������ pc������ cloud189-nj.oos-js.ctyunapi.cn hz189cloud.oos-hz.ctyunapi.cn
	if( ( http_session->m_Requestline.m_Host.Find("ctyunapi.cn")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("response-content-disposition") != -1 ))	
	{
		return kDownloadFileCW;
	}
	//web������2 pc������
	if( ( http_session->m_Requestline.m_Host.Find("transfer1.cloud189.21cn.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/objintf/") != -1 ))	
	{
		return kDownloadFileCWSecond;
	}
	//android�ϴ�
	if( ( http_session->m_Requestline.m_Host.Find("api.cloud.189.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/createUploadFile.action") != -1 ))	
	{
		return kUploadFileM_CC;
	}
	return 0;
}

yunpan::YunPanTianYi::Process(const char *packet,int action)
{
	//web���ϴ�
	if (action == kUploadFileCW)
	{
		return UploadFileCW(packet);
	}
	//web������
	if (action == kDownloadFileCW)
	{
		return DownloadFileCW(packet);
	}
    //web������2
	if (action == kDownloadFileCWSecond)
	{
		return DownloadFileCWSecond(packet);
	}
	//android���ϴ�
	if (action == kUploadFileM_CC)
	{
		return UploadFileM_CC(packet);
	}
	return 0;
}

//web���ϴ�
yunpan::YunPanTianYi::UploadFileCW(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);

	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key,path;
	key = bp.GetBoundaryKey("name=\"Filedata\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	boundary = "\r\n--" + boundary;  

	//·��Ŀǰû��
	path=ms_-> GetValueBetweenBeginEnd(src_data,"name=\"path\"\r\n\r\n",boundary,true);
	filename_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);
	string real_file_name = WriteTempRealFile(bv->ptr,filename_.c_str(),bv->len);
	filename_=path + filename_;
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=�ϴ�\r\n";
	WriteIndexFile(real_file_name);
	return 1;
}

//web������
yunpan::YunPanTianYi::DownloadFileCW(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
    url = ms_->UTF8ToGBK(UrlDecode(url));
	//�ļ���
	filename_ = ms_->GetValueBetweenBeginEnd(url,"filename=\"","\"",true);
    if (filename_.empty())
    {
		return 0;
    }
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//web������2
yunpan::YunPanTianYi::DownloadFileCWSecond(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
    url = ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,"&","=");
	//�ļ���
	filename_ = ms_->GetValueFromMapByKey("Name",true);
    if (filename_.empty())
    {
		return 0;
    }
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//android �ϴ�
yunpan::YunPanTianYi::UploadFileM_CC(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);

	uri = ms_->UrlDecode(uri);

	ms_->SplitStringToMap(uri,'&','=');
	//�ļ���
	filename_ = ms_->GetValueFromMapByKey("fileName",true);
	//·��
	string path = ms_->GetValueFromMapByKey("localPath",true);
	//����
	string model = ms_->GetValueFromMapByKey("model",true);
	//filename_= path + filename_;
    filename_ = path;
	if (filename_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=�ϴ�\r\n";
	WriteIndexFile();
	return 1;
}
