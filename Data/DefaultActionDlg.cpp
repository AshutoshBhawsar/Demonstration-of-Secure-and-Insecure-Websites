// DefaultActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "firewallapp.h"
#include "DefaultActionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefaultActionDlg dialog


CDefaultActionDlg::CDefaultActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDefaultActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefaultActionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	action = PF_ACTION_FORWARD;
}


void CDefaultActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefaultActionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefaultActionDlg, CDialog)
	//{{AFX_MSG_MAP(CDefaultActionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefaultActionDlg message handlers

BOOL CDefaultActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(action == PF_ACTION_DROP)
		CheckRadioButton(IDC_RADIOFORWARD, IDC_RADIODROP, IDC_RADIODROP);

	else
		CheckRadioButton(IDC_RADIOFORWARD, IDC_RADIODROP, IDC_RADIOFORWARD);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDefaultActionDlg::OnOK() 
{
	int id = GetCheckedRadioButton(IDC_RADIOFORWARD, IDC_RADIODROP);

	if(id == IDC_RADIODROP)
		action = PF_ACTION_DROP;

	else
		action = PF_ACTION_FORWARD;
	
	CDialog::OnOK();
}
