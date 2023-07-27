
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "HttpProObserver.h"

HttpProObserver *HttpProObserver::instance_ = NULL;

HttpProObserver *HttpProObserver::GetInstance()
{
	if (instance_ == NULL)
	{
		instance_ = new HttpProObserver();
	}
	return instance_;
}

HttpProObserver::HttpProObserver()
{
	Init();
}

HttpProObserver::~HttpProObserver()
{

}

void HttpProObserver::Init()
{
	pro_list_.push_back(new FeiXin());
	pro_list_.push_back(new WeiXin());


	pro_list_.push_back(new BaiDuTieBa());
	pro_list_.push_back(new MaoPu());
	pro_list_.push_back(new TianYa());
	pro_list_.push_back(new Discuz());
	pro_list_.push_back(new DiDiDaChe());
	pro_list_.push_back(new KuaiDiDaChe());

	pro_list_.push_back(new FengHuangXinWen());
	pro_list_.push_back(new SouHuXinWen());
	pro_list_.push_back(new TengXunXinWen());
	pro_list_.push_back(new WangYiXinWen());
	pro_list_.push_back(new XinLangXinWen());

	pro_list_.push_back(new TengXunWeiBo());
	pro_list_.push_back(new XinLangWeiBo());
	pro_list_.push_back(new PaPa());

	pro_list_.push_back(new TaoBao());
	pro_list_.push_back(new TianMao());
	pro_list_.push_back(new JingDong());

	pro_list_.push_back(new AnQuanWeiShi360());
	pro_list_.push_back(new WanDouJia());
	pro_list_.push_back(new ShouJiWeiShi360());
	pro_list_.push_back(new KuaiYong());
	pro_list_.push_back(new XiaoMiShangDian());
	pro_list_.push_back(new MeiZuShangDian());
	pro_list_.push_back(new ZhuShou91());


}

void HttpProObserver::Uninit()
{
	
}

int HttpProObserver::Process(HttpSession *http_session, const char *packet)
{
	for (std::list<string >::iterator iter1 = useless_.begin(); iter1 != useless_.end();++iter1)
	{

	}
	bool b = false;
	int action = -1;
	for (std::list<HttpPro *>::iterator iter2 = pro_list_.begin(); iter2 != pro_list_.end();++iter2)
	{
		action = (*iter2)->Is(http_session);
		if (action)
		{
			(*iter2)->SetHttpSession(http_session);
			if ((*iter2)->Process(packet,action))
			{
				b = true;
				break;
			}
		}
	}


	return 0;
}