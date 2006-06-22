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

HWND Splashwnd = NULL;

HANDLE thread = NULL;

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
	PostMessage(Splashwnd,WM_RQUIT,0,0);
}

bool APIENTRY Update(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	
	//Display Splash Screen If NOT started in minimized
    Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),NULL,(DLGPROC)SplashDialogProcedure);
    if(nFunsterStil != 7)
    {
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
    Sleep(50);
    CloseHandle(thread);
	return Update_Updater;
}

//Splash Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Starting Update Thread...");
            thread = CreateThread(NULL,0,UpdateThread,NULL,0,NULL); //Returns handle to thread, may be useful...
            return false;
            break;
        case WM_RQUIT:
			PostQuitMessage(0);
			break;
		case WM_SETLOADTEXT:
            return SetDlgItemText(hwnd,IDC_LOAD,(char *)lParam);
        default:
            return false;
    }
    return 0;
}
