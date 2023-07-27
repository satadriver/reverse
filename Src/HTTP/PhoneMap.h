#ifndef PHONE_MAP_H_H_H
#define PHONE_MAP_H_H_H

BOOL DealPhoneMap(HttpSession *pHttpSession ,const char *pPacket);
//点道室内地图
int IsDianDaoShiNeiMap(HttpSession *pHttpSession);
BOOL DianDaoShiNeiMap(HttpSession *pHttpSession ,const char *pPacket);
//朋友定位
int IsPengYouDingWeiMap(HttpSession *pHttpSession);
BOOL PengYouDingWeiMap(HttpSession *pHttpSession ,const char *pPacket);
//百度API
int IsBaiduApiMap(HttpSession *pHttpSession);
BOOL BaiduApiMap(HttpSession *pHttpSession ,const char *pPacket);
//翼周边
int IsYiZHouBianMap(HttpSession *pHttpSession);
BOOL YiZHouBianMap(HttpSession *pHttpSession ,const char *pPacket);
//高德免费导航
int IsGaoDeMianFeiDaoHangMap(HttpSession *pHttpSession);
BOOL GaoDeMianFeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//百度导航 && 百度导航HD
int IsBaiDuDaoHangMap(HttpSession *pHttpSession);
BOOL BaiDuDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//图吧导航离线版
int IsTuBaDaoHangLiXianMap(HttpSession *pHttpSession);
BOOL TuBaDaoHangLiXianMap(HttpSession *pHttpSession ,const char *pPacket);
//图吧导航
int IsTuBaDaoHangMap(HttpSession *pHttpSession);
BOOL TuBaDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//悠悠手机导航
int IsYouYouDaoHangMap(HttpSession *pHttpSession);
BOOL YouYouDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//腾讯地图
int IsTencentMapInfo(HttpSession *pHttpSession);
BOOL TencentMapInfo(HttpSession *pHttpSession ,const char *pPacket);
int IsTencentMap(HttpSession *pHttpSession);
BOOL TencentMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//高德导航
int IsGaoDeDaoHangMap(HttpSession *pHttpSession);
BOOL GaoDeDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//都市圈地图
int IsDuShiQuanMap(HttpSession *pHttpSession);
BOOL DuShiQuanMap(HttpSession *pHttpSession ,const char *pPacket);
//天翼导航
int IsTianYiDaoHangMap(HttpSession *pHttpSession);
BOOL TianYiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//导航犬 && 导航犬2013
int IsDaoHangQuanMap(HttpSession *pHttpSession);
BOOL DaoHangQuanMap(HttpSession *pHttpSession ,const char *pPacket);
//手机导航地图
int IsShouJiDaoHangMap(HttpSession *pHttpSession);
BOOL ShouJiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//泰为导航
int IsTaiWeiDaoHangMap(HttpSession *pHttpSession);
BOOL TaiWeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//凯立德导航家园版
int IsKaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession);
BOOL KaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession ,const char *pPacket);
//搜狗地图
int IsSouGouMap(HttpSession *pHttpSession);
BOOL SouGouMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//飞路快 && 飞路快HD
int IsFeiLuKuaiHDMap(HttpSession *pHttpSession);
BOOL FeiLuKuaiHDMap(HttpSession *pHttpSession ,const char *pPacket);
//E都市三维地图
int IsEDuShiMap(HttpSession *pHttpSession);
BOOL EDuShiMap(HttpSession *pHttpSession ,const char *pPacket);
//百度地图
int IsBaiDuMap(HttpSession *pHttpSession);
BOOL BaiDuMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
#endif