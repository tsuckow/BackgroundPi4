/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Update.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_UPDATE_DLL.h"
#include <windows.h>
#include "exresource.h"

#define WM_RQUIT (WM_APP+0)
#define WM_SETLOADTEXT (WM_APP+1)

bool Update_Updater = false;

HWND Inviswnd = NULL;
HWND Splashwnd = NULL;

HANDLE thread = NULL;

LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog

HINSTANCE thisinstance = NULL;

DWORD WINAPI UpdateThread(void*)
{
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Connecting To Server...");
	Sleep(1000);
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Checking For Updates...");
	Sleep(1000);
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"None, Exiting...");
	Sleep(1000);
	//Update_Updater = true;
	PostMessage(Inviswnd,WM_RQUIT,0,0);
}

bool APIENTRY Update(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	
	//Display Splash Screen If NOT started in minimized
	Inviswnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_INVIS),NULL,(DLGPROC)InvisDialogProcedure);
    
    if(nFunsterStil != 7)
    {
		Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),Inviswnd,(DLGPROC)SplashDialogProcedure);
		ShowWindow (Splashwnd, SW_SHOW);
    }
	
	MSG messages;
	// Run the message loop. It will run until GetMessage() returns 0 
    while (GetMessage (&messages, NULL, 0, 0) > 0)
    {
        if(!IsDialogMessage(Splashwnd, &messages))
        {
            // Translate virtual-key messages into character messages 
            TranslateMessage(&messages);
            // Send message to WindowProcedure 
            DispatchMessage(&messages);
        }
    }
    DestroyWindow(Splashwnd);
    Splashwnd=NULL;
    DestroyWindow(Inviswnd);
    Inviswnd=NULL;
    Sleep(50);
    CloseHandle(thread);
	return Update_Updater;
}

//Invis Dialog
LRESULT CALLBACK InvisDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
			thread = CreateThread(NULL,0,UpdateThread,NULL,0,NULL); //Returns handle to thread, may be useful...
            return false;
            break;
        case WM_RQUIT:
			PostQuitMessage(0);
			break;
        default:
            return false;
    }
    return 0;
}

//Splash Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            SetDlgItemText(hwnd,IDC_LOAD,(char *)"Init...");
            return false;
            break;
		case WM_SETLOADTEXT:
            return SetDlgItemText(hwnd,IDC_LOAD,(char *)lParam);
        default:
            return false;
    }
    return 0;
}
