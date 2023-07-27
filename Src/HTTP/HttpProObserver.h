
#ifndef HTTP_HTTP_PRO_OBSERVER_H_H_H
#define HTTP_HTTP_PRO_OBSERVER_H_H_H


#include "HttpPro.h"

//pro_list

//A
#include "pro/AiQiYi.h"
#include "pro/AnQuanWeiShi360.h"

//B
#include "pro/BaiDuTieBa.h"
#include "pro/Blued.h"

//C

//D
#include "pro/DaZhongDianPing.h"
#include "pro/DianXin.h"
#include "pro/DiDiDaChe.h"
#include "pro/Discuz.h"

//E

//F
#include "pro/FeiXin.h"
#include "pro/FengHuangXinWen.h"

//G

//H

//I

//J
#include "pro/JingDong.h"
#include "pro/JinShanDianChiYiSheng.h"

//K
#include "pro/KuaiDiDaChe.h"
#include "pro/KuaiYong.h"

//L

//M
#include "pro/MaoPu.h"
#include "pro/MBaiDuCom.h"
#include "pro/MeiTuXiuXiu.h"
#include "pro/MeiZuShangDian.h"
#include "pro/MiLiao.h"
#include "pro/MoJiTianQi.h"
#include "pro/MoXiuZhuoMian.h"

//N

//O

//P
#include "pro/PaPa.h"

//Q
#include "pro/QQ.h"

//R

//S
#include "pro/ShouJiWeiShi360.h"
#include "pro/SouGouZhuShou.h"
#include "pro/SouHuXinWen.h"

//T
#include "pro/TalkBox.h"
#include "pro/TaoBao.h"
#include "pro/TengXunShiPin.h"
#include "pro/TengXunWeiBo.h"
#include "pro/TengXunXinWen.h"
#include "pro/TianMao.h"
#include "pro/TianYa.h"

//U

//V
#include "pro/Voxer.h"

//W
#include "pro/WanDouJia.h"
#include "pro/WangYiXinWen.h"
#include "pro/WeiXin.h"

//X
#include "pro/XiaoMiShangDian.h"
#include "pro/XinLangWeiBo.h"
#include "pro/XinLangXinWen.h"

//Y
#include "pro/YiXin.h"
#include "pro/YouKu.h"

//Z
#include "pro/ZhiFuBaoQianBao.h"
#include "pro/ZhuShou91.h"






























//



class HttpProObserver
{
public:
	static HttpProObserver *GetInstance();
	virtual ~HttpProObserver();

	int Process(HttpSession *http_session, const char *packet);
private:
	static HttpProObserver *instance_;
	HttpProObserver();

	void Init();
	void Uninit();

	list<string >useless_;
	list<HttpPro *>pro_list_;
};
#endif