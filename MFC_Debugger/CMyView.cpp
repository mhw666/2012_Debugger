#include "pch.h"
#include "CMyView.h"

CMyView::CMyView(CDialogEx* wMain)
	:mDLG_MEM(0), mLS_Mem(0), mLS_Stack(0), mSecond(0)
{
	m_StatusBar.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), wMain, 0);
	CRect rect;
	wMain->GetWindowRect(rect);
	int strPartDim[3] = { rect.Width() / 4, rect.Width() / 3 * 2,rect.Width() / 3 * 3 }; //分割数量
	m_StatusBar.SetParts(3, strPartDim);

	m_StatusBar.SetText(L"就绪", 0, 0);
	m_StatusBar.SetText(L"当前范围", 1, 0);
	m_StatusBar.SetText(L"时间", 2, 0);
	this->mMain = wMain;
	this->mh_Wind = wMain->GetSafeHwnd();

	mLS_Main = (CListCtrl*)wMain->GetDlgItem(IDC_LISTM1);
	for (int i = 0; i < defNum_MAX_断点; i++)
	{
		mLS_Main->InsertColumn(0, gszBreakPoring[i], 100, 150);
	}
}

CMyView::~CMyView()
{
	delete mDLG_MEM;
}

BOOL CMyView::InitView()
{
	RECT rect;
	HWND hWnd = gINFO_mWind.hwCON;
	::GetWindowRect(this->mh_Wind, &rect);

	//设置控制台
	::MoveWindow(hWnd, 0, 55, rect.right-rect.left-24, rect.bottom-rect.top /*- 120*/, 1);
	::UpdateWindow(hWnd);
	//::SetWindowText(hWnd, L"等待调试进程建立。");

	//初始化内存子窗口
	if (gINFO_mWind.hwMEM && !::IsWindow(gINFO_mWind.hwMEM))
		gINFO_mWind.hwMEM = 0;
	if (gINFO_mWind.hwMEM == 0)
	{
		//mDLG_MEM = new CDLG_MEM();
		//mDLG_MEM->Create(IDD_DIALOG1, this->mMain);
		//mDLG_MEM->ShowWindow(SW_SHOW);
		//mLS_Mem = (CListCtrl*)mDLG_MEM->GetDlgItem(IDC_LIST1);
		//mLS_Stack = (CListCtrl*)mDLG_MEM->GetDlgItem(IDC_LIST2);

		//mLS_Mem->InsertItem(0, L"0x0080D000");
	}
	return true;
}

void CMyView::SetTime()
{
	t = CTime::GetCurrentTime();
	int second = t.GetSecond();
	if (this->mSecond != second)
	{
		this->mSecond = second;
		mstr.Format(L"%04d/%02d/%02d  %02d:%02d:%02d",
			t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), second);
		m_StatusBar.SetText(mstr, 2, 0);
	}
	if (gDATA.CDEBUG)
	{
		DWORD i;
		if (this->mLS_Mem && ReadProcessMemory(gDATA.PS.hProcess, (LPVOID)0x0080D000, &i, 4, 0)) {
			mstr.Format(L"%lu", i);
			this->mLS_Mem->SetItemText(0, 1, mstr);
		}
	}
}
