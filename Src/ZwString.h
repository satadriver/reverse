/*
  Name: ZwString.h
  Copyright: 山东兆物网络技术有限公司
  Author: 蔡兴强
  Date: 2008-11-12 09:18
  Description: 字符串处理相关的内容
*/

#ifndef _ZWSTRING_H_
#define _ZWSTRING_H_

#include "stdafx.h"

/*! \fn CString COMMAPI StrEscape(CString str)
    \brief 转义字符串中的特殊字符
    \param [IN]str 要处理的字符串
    \return 转义后的结果

*/
CString StrEscape(CString str);

/*! \fn CString COMMAPI StrUnescape(CString str)
    \brief 还原字符串中的转义字符
    \param [IN]str 要还原的字符串
    \return 还原后的结果
*/
CString StrUnescape(CString str);

/*! \fn void COMMAPI StrSplit(CStringArray& res,CString str,char sep)
    \brief 用指定的分隔符拆分字符串
	\param [OUT]res 存放拆分结果的数组
    \param [IN]str 要拆分的字符串
	\param [IN]sep 拆分用的分隔符
    \return 无

*/
void StrSplit(CStringArray& res,CString str,char sep);

/*******************************************************************
	特殊字符是指ASCII码值为1－9的九个字符和'\\'、'\r'、'\n'字符，
	共十二个字符。其中ASCII码值为1－9的九个字符可以作为分隔符，
	'\\'、'\r'、'\n'被保留。

	+-------------------------------------------+
	| ASCII码值 | 一般意义 | 是否可以作为分隔符 |
	+-----------+----------+--------------------+
	| 1         |          | 是                 |
	+-----------+----------+--------------------+
	| 2         |          | 是                 |
	+-----------+----------+--------------------+
	| 3         |          | 是                 |
	+-----------+----------+--------------------+
	| 4         |          | 是                 |
	+-----------+----------+--------------------+
	| 5         |          | 是                 |
	+-----------+----------+--------------------+
	| 6         |          | 是                 |
	+-----------+----------+--------------------+
	| 7         |          | 是                 |
	+-----------+----------+--------------------+
	| 8         |          | 是                 |
	+-----------+----------+--------------------+
	| 9         | 制表符   | 是                 |
	+-----------+----------+--------------------+
	| 10        | 换行     | 否                 |
	+-----------+----------+--------------------+
	| 13        | 回车     | 否                 |
	+-----------+----------+--------------------+
	| 92        | 反斜线   | 否                 |
	+-----------+----------+--------------------+

********************************************************************/

#endif