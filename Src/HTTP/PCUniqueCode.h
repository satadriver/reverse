
#ifndef DEV_UNIQUE_CODE_H_H_H
#define DEV_UNIQUE_CODE_H_H_H

BOOL DealDevUC(HttpSession * pHttpSession ,const char *pPacket);

//YY yuyin
BOOL IsDevUC_YYYuYin(HttpSession * pHttpSession);
BOOL DevUC_YYYuyin(HttpSession * pHttpSession ,const char *pPacket);

//fengxing
BOOL IsDevUC_FengXing(HttpSession * pHttpSession);
BOOL DevUC_FengXing(HttpSession * pHttpSession ,const char *pPacket);

//xunlei
BOOL IsDevUC_XunLei(HttpSession * pHttpSession);
BOOL DevUC_XunLei(HttpSession * pHttpSession ,const char *pPacket);

//baofeng
BOOL IsDevUC_BaoFeng(HttpSession * pHttpSession);
BOOL DevUC_BaoFeng(HttpSession * pHttpSession ,const char *pPacket);

//360 anquanweishi
BOOL IsDevUC_360AQWS(HttpSession * pHttpSession);
BOOL DevUC_360AQWS(HttpSession * pHttpSession ,const char *pPacket);

//qq
BOOL IsDevUC_QQ(HttpSession * pHttpSession);
BOOL DevUC_QQ(HttpSession * pHttpSession ,const char *pPacket);

#endif