/*
  Name: MainFrm.cpp
  Copyright: Jesús Oliva 
  Author: Jesús Oliva (jeoliva@telefonica.net)
  Date: 25/09/03 20:19
*/

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FirewallApp.h"

#include "MainFrm.h"
#include "RuleDlg.h"
#include "DefaultActionDlg.h"
#include "FirewallAppDoc.h"
#include "FirewallAppView.h"
#include "SockUtil.h"
#include "rules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTONSTART, OnButtonstart)
	ON_COMMAND(ID_BUTTONADD, OnButtonadd)
	ON_COMMAND(ID_BUTTONDEL, OnButtondel)
	ON_COMMAND(ID_BUTTONSTOP, OnButtonstop)
	ON_UPDATE_COMMAND_UI(ID_BUTTONSTART, OnUpdateButtonstart)
	ON_UPDATE_COMMAND_UI(ID_BUTTONSTOP, OnUpdateButtonstop)
	ON_COMMAND(IDMENU_ADDRULE, OnMenuAddrule)
	ON_COMMAND(IDMENU_DELRULE, OnMenuDelrule)
	ON_COMMAND(ID_MENUSTART, OnMenustart)
	ON_UPDATE_COMMAND_UI(ID_MENUSTART, OnUpdateMenustart)
	ON_COMMAND(ID_MENUSTOP, OnMenustop)
	ON_UPDATE_COMMAND_UI(ID_MENUSTOP, OnUpdateMenustop)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(IDMENU_LOADRULES, OnLoadRules)
	ON_COMMAND(IDMENU_SAVERULES, OnSaveRules)
	ON_COMMAND(IDMENU_SETDEFAULT, OnMenuSetdefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	started = FALSE;
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);


	this->SetWindowText("Firewall PAPI");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;


	// quit the untitled word in window title
	cs.style &= ~ FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnButtonstart() 
{
	CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();
	unsigned int i;
	DWORD result;
	PIP_ADAPTER_INFO pAdapterInfo = NULL, aux;
	IP_ADDR_STRING *localIp;
	unsigned long len = 0;


	// I will send the filter rules to all local ips, because in the rules
	// we don't indicate in what interface the rule will be applied
	GetAdaptersInfo(pAdapterInfo, &len);

	pAdapterInfo = (PIP_ADAPTER_INFO) malloc (len);

	result = GetAdaptersInfo(pAdapterInfo, &len);

	if(result != ERROR_SUCCESS)
	{
		AfxMessageBox("Error getting adapters info.");

		return;
	}

	// Sending rules
	for(i=0;i<doc->nRules;i++)
	{
		// To all adapters
		for(aux=pAdapterInfo;aux != NULL;aux=aux->Next)
		{
			// each ip of a adapter
			for(localIp=&aux->IpAddressList;localIp!=NULL;localIp=localIp->Next)
			{
				pckFilter.AddFilter(CharToIp(localIp->IpAddress.String), 
									ANY_DIRECTION,
									doc->rules[i].sourceIp, 
									doc->rules[i].sourceMask,
									doc->rules[i].destinationIp,
									doc->rules[i].destinationMask,
									doc->rules[i].sourcePort,
									doc->rules[i].destinationPort,
									doc->rules[i].protocol);	
			}
		}

	}
	
	started = TRUE;
}

void CMainFrame::OnButtonstop() 
{
	pckFilter.RemoveAll();

	started = FALSE;
}

void CMainFrame::OnButtonadd() 
{
	CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();
	CRuleDlg dlg;

	// check number of rules
	if(doc->nRules < MAX_RULES )
	{
		dlg.defaultAction = pckFilter.GetDefaultAction();

		if(dlg.DoModal() == IDOK)
		{
			// Add the rule to the rule lists			
			if(doc->AddRule(dlg.srcIp, dlg.srcMask, dlg.srcPort, dlg.dstIp, dlg.dstMask, dlg.dstPort, dlg.protocol, dlg.cAction) != 0)
				AfxMessageBox("Error adding the rule.");

			else
			{
				// Then, Update the rules view
				CFirewallAppView *view = (CFirewallAppView *)GetActiveView();

				view->UpdateList();
			}

		}
	}

	else
		AfxMessageBox("You can't add more rules.");
}

void CMainFrame::OnButtondel() 
{
	CFirewallAppView *view = (CFirewallAppView *)GetActiveView();


	// Get rule position
	POSITION pos = view->m_rules.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("Select a Rule, please.");

		return;
	}

	int position;
	position = view->m_rules.GetNextSelectedItem(pos);

	CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();
	doc->DeleteRule(position);


	// Update rules view
	view->UpdateList();
	
}




void CMainFrame::OnUpdateButtonstart(CCmdUI* pCmdUI) 
{
	if(started)
		pCmdUI->Enable(FALSE);

	else
		pCmdUI->Enable(TRUE);	
}

void CMainFrame::OnUpdateButtonstop(CCmdUI* pCmdUI) 
{
	if(started)
		pCmdUI->Enable(TRUE);

	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuAddrule() 
{
	OnButtonadd();	
}

void CMainFrame::OnMenuDelrule() 
{
	OnButtondel();	
}



void CMainFrame::OnMenustart() 
{
	OnButtonstart();
}

void CMainFrame::OnUpdateMenustart(CCmdUI* pCmdUI) 
{
	if(started)
		pCmdUI->Enable(FALSE);

	else
		pCmdUI->Enable(TRUE);	
	
}

void CMainFrame::OnMenustop() 
{
	OnButtonstop();
}

void CMainFrame::OnUpdateMenustop(CCmdUI* pCmdUI) 
{
	if(started)
		pCmdUI->Enable(TRUE);

	else
		pCmdUI->Enable(FALSE);
	
}

void CMainFrame::OnAppExit() 
{

}


void CMainFrame::OnLoadRules() 
{
	CFile file;
	CFileException e;
	DWORD nRead;

	CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();

	CFileDialog dg(TRUE,NULL, NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT,"Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dg.DoModal()==IDCANCEL)
		return;
	
	CString nf=dg.GetPathName();

	if(nf.GetLength() == 0)
	{
		AfxMessageBox("This file name isn't valid.");

		return;
	}

	if( !file.Open(nf, CFile::modeRead, &e ) )
	{
		AfxMessageBox("Error openning the file.");

		return;
	}

	doc->ResetRules();
	
	PFFORWARD_ACTION action;
	file.Read(&action, sizeof(PFFORWARD_ACTION));

	if(action != pckFilter.GetDefaultAction())
	{
		pckFilter.RemoveAll();

		pckFilter.SetDefaultAction(action);
		doc->defaultAction = action;
	}
	


	RuleInfo rule;
	
    do
    {
 		nRead = file.Read(&rule, sizeof(RuleInfo));
		
		if(nRead == 0)
			break;

		if(doc->AddRule(rule.sourceIp,
				        rule.sourceMask,
						rule.sourcePort,
						rule.destinationIp,
						rule.destinationMask,
						rule.destinationPort,
						rule.protocol,				   
						1)  != 0)
		{

			AfxMessageBox("Error adding a rule.");

			break;
		}

    }while (1);


		
	// update the rules view
	CFirewallAppView *view = (CFirewallAppView *)GetActiveView();

	view->UpdateList();
}

void CMainFrame::OnSaveRules() 
{
	CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();

	if(doc->nRules == 0)
	{
		AfxMessageBox("There isnt Rules to Save.");

		return;
	}


	CFileDialog dg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT,"Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dg.DoModal()==IDCANCEL)
		return;
	
	CString nf=dg.GetPathName();

	if(nf.GetLength() == 0)
	{
		AfxMessageBox("This file name isn't valid.");

		return;
	}

	CFile file;
	CFileException e;

	if( !file.Open( nf, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		AfxMessageBox("Error openning the file.");

		return;
	}


	PFFORWARD_ACTION action = pckFilter.GetDefaultAction();
	file.Write(&action, sizeof(PFFORWARD_ACTION));

	unsigned int i;

	// Write rules in a file
	for(i=0;i<doc->nRules;i++)
	{
		file.Write(&doc->rules[i], sizeof(RuleInfo));
	}
	
	file.Close();
}


void CMainFrame::OnMenuSetdefault() 
{	
	CDefaultActionDlg dlg;

	dlg.action = pckFilter.GetDefaultAction();

	if(dlg.DoModal() == IDOK)
	{
		if(dlg.action != pckFilter.GetDefaultAction())
		{
			pckFilter.RemoveAll();

			pckFilter.SetDefaultAction(dlg.action);

			CFirewallAppDoc *doc = (CFirewallAppDoc *)GetActiveDocument();
			doc->defaultAction = dlg.action;

			CFirewallAppView *view = (CFirewallAppView *)GetActiveView();
			view->UpdateList();
		}
	}
}
