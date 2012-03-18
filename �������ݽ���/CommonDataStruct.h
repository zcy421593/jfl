#ifndef COMMONDATASTRUCT_H
#include "stdafx.h"
#define COMMONDATASTRUCT_H
struct CStockInfo
{
	CString m_strStockName;
	CString m_strDllPath;
	BOOL m_bIsLoad;
	CString m_strDescribtion;
};

//œﬂ–≈œ¢
struct CLineInfo
{
	CString m_strName;
	DWORD m_dwColor;
	int m_iWidth;
	CString m_strDllPath;
};
#endif