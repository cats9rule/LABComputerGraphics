
// IND_17506.h : main header file for the IND_17506 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND17506App:
// See IND_17506.cpp for the implementation of this class
//

class CIND17506App : public CWinApp
{
public:
	CIND17506App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND17506App theApp;
