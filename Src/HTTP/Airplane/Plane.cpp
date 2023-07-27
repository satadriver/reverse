#include "StdAfx.h"
#include "Plane.h"
#include "ChinaSouthern.h"
#include "ChinaHainan.h"
#include "ChinaEastern.h"
#include "PlaneAirChina.h"
#include "PlaneJuneYaoAir.h"
#include "PlaneXingFuAirlines.h"
#include "PlaneSiChuanAirlines.h"
#include "PlaneChunQiuAirlines.h"
#include "PlaneShanDongAirlines.h"
#include "PlaneCaptialAirlines.h"
#include "PlaneXiaMenAirlines.h"
#include "PlaneLianHeAirlines.h"
#include "PlaneQingDaoAirlines.h"
#include "PlaneShenZhenAirlines.h"

plane::Airplane::Airplane(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

plane::Airplane::~Airplane()
{
	
}

int plane::Airplane::ProcessAirplane(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//�Ϸ����� 1
		action = ChinaSouthern::IsChinaSouthern(http_session);
		if (action)
		{
			ChinaSouthern chinasouthern(http_session,"plane_chinasouthern");
			return chinasouthern.Process(packet,action);
		}
		//���Ϻ��� 2
		action = ChinaHainan::IsChinaHainan(http_session);
		if (action)
		{
			n=3;
			ChinaHainan chinahainan(http_session,"plane_chinahainan");
			return chinahainan.Process(packet,action);
		}
		//�������� 3
		action = ChinaEastern::IsChinaEastern(http_session);
		if (action)
		{
			ChinaEastern chinaeastern(http_session,"plane_chinaeastern");
			return chinaeastern.Process(packet,action);
		}
		//ɽ������ 4
		action = ShanDongAirlines::IsShanDongAirlines(http_session);
		if (action)
		{
			ShanDongAirlines shandongairlines(http_session,"plane_shandongairlines");
			return shandongairlines.Process(packet,action);
		}
		//���ʺ��� 5
		action = PlaneAirChina::IsPlaneAirChina(http_session);
		if (action)
		{
			PlaneAirChina planeairchina(http_session,"plane_chinaairlines");
			return planeairchina.Process(packet,action);
		}
		//���麽�� 6
		action = PlaneJuneYaoAir::IsPlaneJuneYaoAir(http_session);
		if (action)
		{
			PlaneJuneYaoAir planejuneyaoair(http_session,"plane_juneyaoairlines");
			return planejuneyaoair.Process(packet,action);
		}
		//���ﺽ�� 7
		action = ChunQiuAirlines::IsChunQiuAirlines(http_session);
		if (action)
		{
			ChunQiuAirlines chunqiuairlines(http_session,"plane_chunqiuairlines");
			return chunqiuairlines.Process(packet,action);
		}
		//�Ĵ����� 8
		action = SiChuanAirlines::IsSiChuanAirlines(http_session);
		if (action)
		{
			SiChuanAirlines sichuanairlines(http_session,"plane_sichuanairlines");
			return sichuanairlines.Process(packet,action);
		}
		//�Ҹ����� 9
		action = XingFuAirlines::IsXingFuAirlines(http_session);
		if (action)
		{
			XingFuAirlines xingfuairlines(http_session,"plane_xingfuairlines");
			return xingfuairlines.Process(packet,action);
		}
		//�׶����� 10
		action = CaptalAirLines::IsCaptalAirLines(http_session);
		if (action)
		{
			CaptalAirLines captalairlines(http_session,"plane_captialairlines");
			return captalairlines.Process(packet,action);
		}
		//���ź��� 11
		action = XiaMenAirLines::IsXiaMenAirLines(http_session);
		if (action)
		{
			XiaMenAirLines xiamenairlines(http_session,"plane_xiamenairlines");
			return xiamenairlines.Process(packet,action);
		}
		//���ں��� 12
		action = ShenZhenAirlines::IsShenZhenAirlines(http_session);
		if (action)
		{
			ShenZhenAirlines shenzhenairlines(http_session,"plane_shenzhenairlines");
			return shenzhenairlines.Process(packet,action);
		}
		//�ൺ���� 13
		action = QingDaoAirlines::IsQingDaoAirlines(http_session);
		if (action)
		{
			QingDaoAirlines qingdaoairlines(http_session,"plane_qingdaoairlines");
			return qingdaoairlines.Process(packet,action);
		}
		//���Ϻ��� 14
		action = LianHeAirlines::IsLianHeAirlines(http_session);
		if (action)
		{
			LianHeAirlines lianheairlines(http_session,"plane_lianheairlines");
			return lianheairlines.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessAirplane Err : %d",n);
	}
	
	return 0;
}