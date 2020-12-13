#if !defined(AFX_DEFAULTACTIONDLG_H__4D3EEFB2_484C_450A_8C4D_785B1535BBDE__INCLUDED_)
#define AFX_DEFAULTACTIONDLG_H__4D3EEFB2_484C_450A_8C4D_785B1535BBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Fltdefs.h>
// DefaultActionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDefaultActionDlg dialog

class CDefaultActionDlg : public CDialog
{
// Construction
public:
	CDefaultActionDlg(CWnd* pParent = NULL);   // standard constructor
	PFFORWARD_ACTION action;

// Dialog Data
	//{{AFX_DATA(CDefaultActionDlg)
	enum { IDD = IDD_DEFAULTACTIONDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDefaultActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDefaultActionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFAULTACTIONDLG_H__4D3EEFB2_484C_450A_8C4D_785B1535BBDE__INCLUDED_)
