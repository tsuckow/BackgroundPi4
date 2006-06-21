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

LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog
HINSTANCE thisinstance = NULL;

bool APIENTRY Update(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	HWND Splashwnd = NULL;
	
	//Display Splash Screen If NOT started in minimized
    if(nFunsterStil != 7)
    {
    	Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),NULL,(DLGPROC)SplashDialogProcedure);
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
    MessageBox(NULL,"Done","Update.DLL",MB_OK);
	return false;
}

//Splash Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            PostQuitMessage(0);
            return false;
            break;
        default:
            return false;
    }
    return 0;
}
