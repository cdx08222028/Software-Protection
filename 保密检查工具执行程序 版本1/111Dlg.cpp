// 111Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "111.h"
#include "111Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy111Dlg dialog

CMy111Dlg::CMy111Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy111Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy111Dlg)
	m_out = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy111Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy111Dlg)
	DDX_Text(pDX, IDC_OUT, m_out);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy111Dlg, CDialog)
	//{{AFX_MSG_MAP(CMy111Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_EXEC, OnExec)
	ON_BN_CLICKED(IDC_LOGOUT, OnLogout)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy111Dlg message handlers

BOOL CMy111Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	manager=LoadLibrary("NT77Mgr.dll");
	applic=LoadLibrary("NT77.dll");

	if(manager==NULL || applic==NULL) SendMessage(WM_CLOSE,0,0);

	NTFindAll=(NTFINDALL)GetProcAddress(manager,"NTFindAll");
	NTGetDevCaps=(NTGETDEVCAPS)GetProcAddress(manager,"NTGetDevCaps");
	NTRead=(NTREAD)GetProcAddress(manager,"NTRead");
	NTGetHardwareID=(NTGETHARDWAREID)GetProcAddress(applic,"NTGetHardwareID");
	NTLogin=(NTLOGIN)GetProcAddress(applic,"NTLogin");
	NTLogout=(NTLOGOUT)GetProcAddress(applic,"NTLogout");
	NTFindFirst=(NTFINDFIRST)GetProcAddress(applic,"NTFindFirst");
	NTRead=(NTREAD)GetProcAddress(applic,"NTRead");
	
	char temp[128];
	if(0!=NTFindAll(NTHandle,&nKeyNum) || nKeyNum==0) 
	{
		MessageBox("δ����������!");
		SendMessage(WM_CLOSE,0,0);
		return TRUE;
	}
	long keyMode,keyVersion,e2size,ramSize,productionTime;
	NTGetDevCaps(NTHandle[0],&keyMode,&keyVersion,&e2size,&ramSize,&productionTime);
	m_out="";
	m_out+="�������ͺ�:";
	sprintf(temp,"%d",keyMode);
	m_out+=temp;
	if(keyMode==0x50) m_out+="(NT77)";
	m_out+="\r\nӲ���汾:";
	sprintf(temp,"%d",keyVersion);
	m_out+=temp;
	m_out+="\r\n�洢����С:";
	sprintf(temp,"%d",e2size);
	m_out+=temp;
	m_out+="\r\n�ڴ�����С:";
	sprintf(temp,"%d",ramSize);
	m_out+=temp;
	m_out+="\r\n��������:";
	sprintf(temp,"%d��%d��%d��",(productionTime&0xFF)+2000,(productionTime>>8)&0xFF,(productionTime>>16)&0xFF);
	m_out+=temp;

	UpdateData(FALSE);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy111Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy111Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy111Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy111Dlg::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	int nRet;
	GetDlgItem(IDC_EDIT4)->GetWindowText(str);
	nRet=NTFindFirst((LPTSTR)(LPCTSTR)str);
	if(0!=nRet) 
	{
		MessageBox("��¼ʧ��!");
		return;
	}

	GetDlgItem(IDC_EDIT)->GetWindowText(str);
	nRet=NTLogin((LPTSTR)(LPCTSTR)str);
	if(0!=nRet) 
	{
		MessageBox("��¼ʧ��");
		if(nRet==32) 
		{
			MessageBox("�����������������²��");
		}
		return;
	}

	char temp[128];
	m_out+="\r\nӲ�����к�:";
	NTGetHardwareID(temp);
	m_out+=temp;
	m_out+="\r\n\r\n";
	UpdateData(FALSE);

	GetDlgItem(IDC_EXEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
}

void CMy111Dlg::FindDevice()
{
	char temp[128];
	if(0!=NTFindAll(NTHandle,&nKeyNum) || nKeyNum==0) 
	{
		MessageBox("δ����������!");
		SendMessage(WM_CLOSE,0,0);
		return;
	}
	long keyMode,keyVersion,e2size,ramSize,productionTime;
	NTGetDevCaps(NTHandle[0],&keyMode,&keyVersion,&e2size,&ramSize,&productionTime);
	m_out="";
	m_out+="�������ͺ�:";
	sprintf(temp,"%d",keyMode);
	m_out+=temp;
	if(keyMode==0x50) m_out+="(NT77)";
	m_out+="\r\nӲ���汾:";
	sprintf(temp,"%d",keyVersion);
	m_out+=temp;
	m_out+="\r\n�洢����С:";
	sprintf(temp,"%d",e2size);
	m_out+=temp;
	m_out+="\r\n�ڴ�����С:";
	sprintf(temp,"%d",ramSize);
	m_out+=temp;
	m_out+="\r\n��������:";
	sprintf(temp,"%d��%d��%d��",(productionTime&0xFF)+2000,(productionTime>>8)&0xFF,(productionTime>>16)&0xFF);
	m_out+=temp;
	
	UpdateData(FALSE);
}

void CMy111Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DWORD dwResult;
	GetExitCodeThread(pi.hThread,&dwResult);
	if(dwResult==STILL_ACTIVE)
	{
		MessageBox("���ܼ�鹤�߻������У����ȹر�!");
		return;
	}
	
	DeleteFile("src.exe");
	NTLogout();	
	FreeLibrary(manager);
	FreeLibrary(applic);

	CDialog::OnClose();
}

void CMy111Dlg::OnExec() 
{
	// TODO: Add your control notification handler code here
	unsigned char Buf[0x400];
	char sig[]="NanJing University Security inspection tool ";

	GetDlgItem(IDC_EXEC)->EnableWindow(FALSE);
	if(0!=NTRead(0x00,0x400,Buf)) 
	{
		MessageBox("��ȡʧ��!");
		return;
	}
	if(0!=memcmp(Buf,sig,strlen(sig)))
		MessageBox("�Ƿ�������!");

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	ZeroMemory(&pi,sizeof(pi));

	CopyFile("1.exe","src.exe",NULL);
	DestFile.Open("src.exe",CFile::modeWrite);
	DestFile.Seek(0x400,CFile::begin);
	DestFile.Write(Buf+0x200,0x200);
	DestFile.Close();

	CreateProcess(NULL,"1.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
//	CreateProcess(NULL,"src.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
	
}

void CMy111Dlg::OnLogout() 
{
	// TODO: Add your control notification handler code here
	NTLogout();
	GetDlgItem(IDC_EXEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
}


void CMy111Dlg::OnFind() 
{
	// TODO: Add your control notification handler code here
	FindDevice();
}
