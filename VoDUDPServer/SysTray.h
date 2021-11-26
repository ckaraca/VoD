#pragma once

class CSysTray
{
public:
	CSysTray(void);
	int Init(HWND m_hWnd, LPCTSTR pIconName, TCHAR* Text, UINT uID,UINT nMenuID);
	LONG TrayNotification(WPARAM wParam, LPARAM lParam);
	~CSysTray(void);
private:
	CMenu m_PopUp;
	NOTIFYICONDATA m_IconData;
	UINT m_uID;
};
