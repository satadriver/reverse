
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
		//360Ψֵ 1
		action = HardWare360::IsHardWare360(http_session);
		if (action)
		{
			HardWare360 hardware360(http_session,"HARDWARE_CODE");
			return hardware360.Process(packet,action);
		}
		//�ֻ��Ա�Ψֵ 2
		action = HardWareTaoBao::IsHardWareTaoBao(http_session);
		if (action)
		{
			HardWareTaoBao hardwaretaobao(http_session,"HARDWARE_CODE");
			return hardwaretaobao.Process(packet,action);
		}
		//�ٶȵ�ͼΨֵ 3
		action = HardWareBaiduDitu::IsHardWareBaiduDitu(http_session);
		if (action)
		{
			HardWareBaiduDitu hardwarebaiduditu(http_session,"HARDWARE_CODE");
			return hardwarebaiduditu.Process(packet,action);
		}
		//������ƷΨֵ 4
		action = HardWareJumeiYoupin::IsHardWareJumeiYoupin(http_session);
		if (action)
		{
			HardWareJumeiYoupin hardwarejumeiyoupin(http_session,"HARDWARE_CODE");
			return hardwarejumeiyoupin.Process(packet,action);
		}
		//â������Ψֵ 5
		action = HardWareMangGuo::IsHardWareMangGuo(http_session);
		if (action)
		{
			HardWareMangGuo hardwaremangguo(http_session,"HARDWARE_CODE");
			return hardwaremangguo.Process(packet,action);
		}
		//����΢��Ψֵ 6
		action = HardWareWeiBo::IsHardWareWeiBo(http_session);
		if (action)
		{
			HardWareWeiBo hardwareweibo(http_session,"HARDWARE_CODE");
			return hardwareweibo.Process(packet,action);
		}
		//���˲��� 7
		action = HardWareBoKe::IsHardWareBoKe(http_session);
		if (action)
		{
			HardWareBoKe hardwareboke(http_session,"HARDWARE_CODE");
			return hardwareboke.Process(packet,action);
		}
		//�ֻ��ṷ����Ψֵ 8
		action = HardWareKuGou::IsHardWareKuGou(http_session);
		if (action)
		{
			HardWareKuGou hardwarekugou(http_session,"HARDWARE_CODE");
			return hardwarekugou.Process(packet,action);
		}
		//���԰ٶ�����Ψֵ 9
		action = HardWareBaiDuYunPan::IsHardWareBaiDuYunPan(http_session);
		if (action)
		{
			HardWareBaiDuYunPan hardwarebaiduyunpan(http_session,"HARDWARE_CODE");
			return hardwarebaiduyunpan.Process(packet,action);
		}
		//������Ψֵ 10
		action = HardWareIqiYi::IsHardWareIqiYi(http_session);
		if (action)
		{
			HardWareIqiYi hardwareiqiYi(http_session,"HARDWARE_CODE");
			return hardwareiqiYi.Process(packet,action);
		}
		//����Ψֵ 11
		action = HardWareDuBa::IsHardWareDuBa(http_session);
		if (action)
		{
			HardWareDuBa hardwareduba(http_session,"HARDWARE_CODE");
			return hardwareduba.Process(packet,action);
		}
		//Ѹ��Ψֵ 12
		action = HardWareXunLei::IsHardWareXunLei(http_session);
		if (action)
		{
			HardWareXunLei hardwarexunlei(http_session,"HARDWARE_CODE");
			return hardwarexunlei.Process(packet,action);
		}
		//��ͨӪҵ�� 13
		action = HardWareLianTong::IsHardWareLianTong(http_session);
		if (action)
		{
			HardWareLianTong hardwareliantong(http_session,"HARDWARE_CODE");
			return hardwareliantong.Process(packet,action);
		}
		//��Ľ�� 14
		action = HardWareImooc::IsHardWareImooc(http_session);
		if (action)
		{
			HardWareImooc hardwareimooc(http_session,"HARDWARE_CODE");
			return hardwareimooc.Process(packet,action);
		}
		//�ٶȴ��� 15
		action = HardWareBaiduChuanKe::IsHardWareBaiduChuanKe(http_session);
		if (action)
		{
			HardWareBaiduChuanKe hardwarebaiduchuanke(http_session,"HARDWARE_CODE");
			return hardwarebaiduchuanke.Process(packet,action);
		}		 
		//��Ѷ��Ƶ 16
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