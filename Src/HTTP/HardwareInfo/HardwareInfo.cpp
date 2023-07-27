
#include "StdAfx.h"

#include "HardwareInfo.h"
#include "hardware360.h"
#include "hardwaretaobao.h"
#include "hardwarebaiduditu.h"
#include "hardwarejumeiyoupin.h"
#include "hardwaremangguo.h"
#include "hardwareweibo.h"
#include "hardwarekugou.h"
#include "hardwarebaiduyunpan.h"
#include "hardwareiqiyi.h"
#include "hardwareduba.h"
#include "hardwarexunlei.h"
#include "hardwareliantong.h"
#include "hardwareimooc.h"
#include "hardwarebaiduchuanke.h"
#include "hardwaretengxunshipin.h"
#include "hardwareboke.h"

hardwareinfo::HardwareInfo::HardwareInfo(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

hardwareinfo::HardwareInfo::~HardwareInfo()
{
	
}

int hardwareinfo::HardwareInfo::ProcessHardwareInfo(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//360唯值 1
		action = HardWare360::IsHardWare360(http_session);
		if (action)
		{
			HardWare360 hardware360(http_session,"HARDWARE_CODE");
			return hardware360.Process(packet,action);
		}
		//手机淘宝唯值 2
		action = HardWareTaoBao::IsHardWareTaoBao(http_session);
		if (action)
		{
			HardWareTaoBao hardwaretaobao(http_session,"HARDWARE_CODE");
			return hardwaretaobao.Process(packet,action);
		}
		//百度地图唯值 3
		action = HardWareBaiduDitu::IsHardWareBaiduDitu(http_session);
		if (action)
		{
			HardWareBaiduDitu hardwarebaiduditu(http_session,"HARDWARE_CODE");
			return hardwarebaiduditu.Process(packet,action);
		}
		//聚美优品唯值 4
		action = HardWareJumeiYoupin::IsHardWareJumeiYoupin(http_session);
		if (action)
		{
			HardWareJumeiYoupin hardwarejumeiyoupin(http_session,"HARDWARE_CODE");
			return hardwarejumeiyoupin.Process(packet,action);
		}
		//芒果旅游唯值 5
		action = HardWareMangGuo::IsHardWareMangGuo(http_session);
		if (action)
		{
			HardWareMangGuo hardwaremangguo(http_session,"HARDWARE_CODE");
			return hardwaremangguo.Process(packet,action);
		}
		//新浪微博唯值 6
		action = HardWareWeiBo::IsHardWareWeiBo(http_session);
		if (action)
		{
			HardWareWeiBo hardwareweibo(http_session,"HARDWARE_CODE");
			return hardwareweibo.Process(packet,action);
		}
		//新浪博客 7
		action = HardWareBoKe::IsHardWareBoKe(http_session);
		if (action)
		{
			HardWareBoKe hardwareboke(http_session,"HARDWARE_CODE");
			return hardwareboke.Process(packet,action);
		}
		//手机酷狗音乐唯值 8
		action = HardWareKuGou::IsHardWareKuGou(http_session);
		if (action)
		{
			HardWareKuGou hardwarekugou(http_session,"HARDWARE_CODE");
			return hardwarekugou.Process(packet,action);
		}
		//电脑百度云盘唯值 9
		action = HardWareBaiDuYunPan::IsHardWareBaiDuYunPan(http_session);
		if (action)
		{
			HardWareBaiDuYunPan hardwarebaiduyunpan(http_session,"HARDWARE_CODE");
			return hardwarebaiduyunpan.Process(packet,action);
		}
		//爱奇艺唯值 10
		action = HardWareIqiYi::IsHardWareIqiYi(http_session);
		if (action)
		{
			HardWareIqiYi hardwareiqiYi(http_session,"HARDWARE_CODE");
			return hardwareiqiYi.Process(packet,action);
		}
		//毒霸唯值 11
		action = HardWareDuBa::IsHardWareDuBa(http_session);
		if (action)
		{
			HardWareDuBa hardwareduba(http_session,"HARDWARE_CODE");
			return hardwareduba.Process(packet,action);
		}
		//迅雷唯值 12
		action = HardWareXunLei::IsHardWareXunLei(http_session);
		if (action)
		{
			HardWareXunLei hardwarexunlei(http_session,"HARDWARE_CODE");
			return hardwarexunlei.Process(packet,action);
		}
		//联通营业厅 13
		action = HardWareLianTong::IsHardWareLianTong(http_session);
		if (action)
		{
			HardWareLianTong hardwareliantong(http_session,"HARDWARE_CODE");
			return hardwareliantong.Process(packet,action);
		}
		//爱慕课 14
		action = HardWareImooc::IsHardWareImooc(http_session);
		if (action)
		{
			HardWareImooc hardwareimooc(http_session,"HARDWARE_CODE");
			return hardwareimooc.Process(packet,action);
		}
		//百度传课 15
		action = HardWareBaiduChuanKe::IsHardWareBaiduChuanKe(http_session);
		if (action)
		{
			HardWareBaiduChuanKe hardwarebaiduchuanke(http_session,"HARDWARE_CODE");
			return hardwarebaiduchuanke.Process(packet,action);
		}		 
		//腾讯视频 16
		action = HardWareTengxunShipin::IsHardWareTengxunShipin(http_session);
		if (action)
		{
			HardWareTengxunShipin hardwaretengxunshipin(http_session,"HARDWARE_CODE");
			return hardwaretengxunshipin.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessHardwareInfo Err : %d",n);
	}
	
	return 0;
}