// �������ݽ���.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C�������ݽ���App:
// �йش����ʵ�֣������ �������ݽ���.cpp
//

class C�������ݽ���App : public CWinApp
{
public:
	C�������ݽ���App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C�������ݽ���App theApp;