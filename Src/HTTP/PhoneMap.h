#ifndef PHONE_MAP_H_H_H
#define PHONE_MAP_H_H_H

BOOL DealPhoneMap(HttpSession *pHttpSession ,const char *pPacket);
//������ڵ�ͼ
int IsDianDaoShiNeiMap(HttpSession *pHttpSession);
BOOL DianDaoShiNeiMap(HttpSession *pHttpSession ,const char *pPacket);
//���Ѷ�λ
int IsPengYouDingWeiMap(HttpSession *pHttpSession);
BOOL PengYouDingWeiMap(HttpSession *pHttpSession ,const char *pPacket);
//�ٶ�API
int IsBaiduApiMap(HttpSession *pHttpSession);
BOOL BaiduApiMap(HttpSession *pHttpSession ,const char *pPacket);
//���ܱ�
int IsYiZHouBianMap(HttpSession *pHttpSession);
BOOL YiZHouBianMap(HttpSession *pHttpSession ,const char *pPacket);
//�ߵ���ѵ���
int IsGaoDeMianFeiDaoHangMap(HttpSession *pHttpSession);
BOOL GaoDeMianFeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//�ٶȵ��� && �ٶȵ���HD
int IsBaiDuDaoHangMap(HttpSession *pHttpSession);
BOOL BaiDuDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//ͼ�ɵ������߰�
int IsTuBaDaoHangLiXianMap(HttpSession *pHttpSession);
BOOL TuBaDaoHangLiXianMap(HttpSession *pHttpSession ,const char *pPacket);
//ͼ�ɵ���
int IsTuBaDaoHangMap(HttpSession *pHttpSession);
BOOL TuBaDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//�����ֻ�����
int IsYouYouDaoHangMap(HttpSession *pHttpSession);
BOOL YouYouDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//��Ѷ��ͼ
int IsTencentMapInfo(HttpSession *pHttpSession);
BOOL TencentMapInfo(HttpSession *pHttpSession ,const char *pPacket);
int IsTencentMap(HttpSession *pHttpSession);
BOOL TencentMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//�ߵµ���
int IsGaoDeDaoHangMap(HttpSession *pHttpSession);
BOOL GaoDeDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//����Ȧ��ͼ
int IsDuShiQuanMap(HttpSession *pHttpSession);
BOOL DuShiQuanMap(HttpSession *pHttpSession ,const char *pPacket);
//������
int IsTianYiDaoHangMap(HttpSession *pHttpSession);
BOOL TianYiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//����Ȯ && ����Ȯ2013
int IsDaoHangQuanMap(HttpSession *pHttpSession);
BOOL DaoHangQuanMap(HttpSession *pHttpSession ,const char *pPacket);
//�ֻ�������ͼ
int IsShouJiDaoHangMap(HttpSession *pHttpSession);
BOOL ShouJiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//̩Ϊ����
int IsTaiWeiDaoHangMap(HttpSession *pHttpSession);
BOOL TaiWeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket);
//�����µ�����԰��
int IsKaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession);
BOOL KaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession ,const char *pPacket);
//�ѹ���ͼ
int IsSouGouMap(HttpSession *pHttpSession);
BOOL SouGouMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
//��·�� && ��·��HD
int IsFeiLuKuaiHDMap(HttpSession *pHttpSession);
BOOL FeiLuKuaiHDMap(HttpSession *pHttpSession ,const char *pPacket);
//E������ά��ͼ
int IsEDuShiMap(HttpSession *pHttpSession);
BOOL EDuShiMap(HttpSession *pHttpSession ,const char *pPacket);
//�ٶȵ�ͼ
int IsBaiDuMap(HttpSession *pHttpSession);
BOOL BaiDuMap(HttpSession *pHttpSession ,const char *pPacket, int nAction);
#endif