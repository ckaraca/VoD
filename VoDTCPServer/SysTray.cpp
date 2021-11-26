#include "StdAfx.h"
#include "systray.h"

CSysTray::CSysTray(void)
{
	ZeroMemory(&m_IconData,sizeof(NOTIFYICONDATA));
}

CSysTray::~CSysTray(void)
{
	Shell_NotifyIcon(NIM_DELETE,&m_IconData);
}

int CSysTray::Init(HWND m_hWnd, LPCTSTR pIconName, TCHAR* Text, UINT uID,UINT nMenuID)
{
	m_IconData.cbSize = sizeof(NOTIFYICONDATA);
	m_IconData.hWnd = m_hWnd;
	m_IconData.hIcon = LoadIcon(AfxGetApp()->m_hInstance,pIconName);		
	strcpy(m_IconData.szTip,Text);
	m_IconData.uCallbackMessage = WM_SYSTRAY;
	m_IconData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_IconData.uID = uID;
	m_uID = uID;

	int error = Shell_NotifyIcon(NIM_ADD,&m_IconData);
	
	if(!m_PopUp.LoadMenu(nMenuID))
		return 1;
	return error;
}

LONG CSysTray::TrayNotification(WPARAM wParam, LPARAM lParam)
{
	//This function recieves information when the user clicks on the icon
	UINT uID; 
    UINT uMouseMsg; 
 
    uID = (UINT)wParam; 
    uMouseMsg = (UINT)lParam; 
	
	//First make sure it is our icon, and not somebody elses, that has been clicked
	if(uID == m_uID)
	{
		if(uMouseMsg == WM_RBUTTONDOWN)
		{
			//Now it is safe to pop-up our menu.			
			//First we extract the sub menu
			CMenu *submenu;
			if(!(submenu = m_PopUp.GetSubMenu(0)))
				return 1;
			//We set the first thing in the menu to the default
			::SetMenuDefaultItem(submenu->m_hMenu, 0, TRUE);
			
			//Then we set the focus to the menu
			::SetForegroundWindow(m_IconData.hWnd);

			//Now we get the position of the mouse and display the menu there
			POINT pos;
			GetCursorPos(&pos);
			//This notifies the parent window that the user is using the menu
			::TrackPopupMenu(submenu->m_hMenu,TPM_LEFTBUTTON|TPM_CENTERALIGN,pos.x,pos.y,0,m_IconData.hWnd,NULL);
			
		}
		else
			if(uMouseMsg == WM_LBUTTONDBLCLK)
			{
				//double click received, the default action is to execute first menu item
				::SetForegroundWindow(m_IconData.hWnd);

				CMenu *submenu;
				if(!(submenu = m_PopUp.GetSubMenu(0)))
					return 1;
				//This notifies the parent window that the user is using the menu
				::SendMessage(m_IconData.hWnd, WM_COMMAND, submenu->GetMenuItemID(2), 0);
			}
		 
	}

	return 0;
}
