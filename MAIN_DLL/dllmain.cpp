/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_MAIN_DLL.h"

bool doExit = true;

HWND Inviswnd = NULL;
HWND Splashwnd = NULL;

HANDLE thread = NULL;
HINSTANCE thisinstance = NULL;

LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog



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
	
	if(nFunsterStil != 7)
    {
		Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),Inviswnd,(DLGPROC)SplashDialogProcedure);
		ShowWindow (Splashwnd, SW_SHOW);
    }
	
	MSG messages;
	// Run the message loop. It will run until GetMessage() returns 0 
    while (GetMessage (&messages, NULL, 0, 0) > 0)
    {
        if(!IsDialogMessage(Inviswnd, &messages) && !IsDialogMessage(Splashwnd, &messages))
        {
            // Translate virtual-key messages into character messages 
            TranslateMessage(&messages);
            // Send message to WindowProcedure 
            DispatchMessage(&messages);
        }
    }
    DestroyWindow(Inviswnd);
    Inviswnd=NULL;
    DestroyWindow(Splashwnd);
    Splashwnd=NULL;
    Sleep(50);
    CloseHandle(thread);
	return doExit;
}
