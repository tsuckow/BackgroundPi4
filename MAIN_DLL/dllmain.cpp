/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_MAIN_DLL.h"
#include <windows.h>
#include "exresource.h"

#define WM_RQUIT 		(WM_APP+0)
#define WM_NOTIFYICON 	(WM_APP+1)

#define ID_MENU_EXIT    (WM_USER+0)

HWND Inviswnd = NULL;

HANDLE thread = NULL;

LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog

HINSTANCE thisinstance = NULL;

TrayIcon * hGlobalIcon = NULL;


//Tray Icon
TrayIcon::TrayIcon(HINSTANCE instance)
{
	iconinstance=instance;
	tiptext = "";
	Add();
}

TrayIcon::~TrayIcon()
{
	Remove();
}

void TrayIcon::Add()
{
	lstrcpyn(tnd.szTip, (LPCTSTR)tiptext, sizeof(tnd.szTip));
	tnd.uFlags = NIF_ICON | NIF_TIP|NIF_MESSAGE;
	tnd.uID =(UINT)IDI_WINLOGO;
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = Inviswnd;
    tnd.uCallbackMessage = WM_NOTIFYICON;
    tnd.hIcon = LoadIcon(iconinstance,"A");
    Shell_NotifyIcon(NIM_ADD, &tnd);
}
void TrayIcon::Remove()
{
    Shell_NotifyIcon(NIM_DELETE, &tnd);
}

DWORD WINAPI MainThread(void*)
{
	Sleep(999999999);
	//hGlobalIcon->Remove();
	//Todo
	PostMessage(Inviswnd,WM_RQUIT,0,0);
}

bool APIENTRY Main(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	
	Inviswnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_INVIS),NULL,(DLGPROC)InvisDialogProcedure);
	//ShowWindow (Inviswnd, SW_SHOW);
	
	TrayIcon hIcon(thisinstance);
	hGlobalIcon = &hIcon;
	
	MSG messages;
	// Run the message loop. It will run until GetMessage() returns 0 
    while (GetMessage (&messages, NULL, 0, 0) > 0)
    {
        if(!IsDialogMessage(Inviswnd, &messages))
        {
            // Translate virtual-key messages into character messages 
            TranslateMessage(&messages);
            // Send message to WindowProcedure 
            DispatchMessage(&messages);
        }
    }
    DestroyWindow(Inviswnd);
    Inviswnd=NULL;
    Sleep(50);
    CloseHandle(thread);
	return true;
}


//Invis Dialog
LRESULT CALLBACK InvisDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            thread = CreateThread(NULL,0,MainThread,NULL,0,NULL); //Returns handle to thread, may be useful...
            return false;
            break;
        case WM_NOTIFYICON:
			if ((UINT)lParam == WM_RBUTTONUP){
            	POINT cord;
            	GetCursorPos(&cord);
            	HMENU menu;
            	menu = CreatePopupMenu();
            	AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_EXIT,"Exit");
            	SetForegroundWindow(hwnd);
            	TrackPopupMenu(menu,TPM_RIGHTALIGN,cord.x,cord.y,0,hwnd,NULL);
            	DestroyMenu(menu);
            	PostMessage(hwnd, WM_NULL, 0, 0);
        	}
        	else if ((UINT)lParam == WM_LBUTTONDBLCLK)
        	{/*
            	if ( !IsWindow( Statuswin ) ) 
            	{
                	Statuswin = CreateDialog(hGlobalInstance,MAKEINTRESOURCE(IDD_DLG_STAT), NULL,(DLGPROC)StatusWindowProcedure);
                	ShowWindow(Statuswin,SW_SHOW);
                	//UpdateWindow(Statuswin);
            	} 
            	SetForegroundWindow(Statuswin);
        	*/}    
            return 0;
            break;
        case WM_RQUIT:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
            switch(LOWORD(wParam))
            {
				case ID_MENU_EXIT:
					PostQuitMessage (0);
                    break;
			}
			return 0;
            break;
        default:
            return false;
    }
    return 0;
}
