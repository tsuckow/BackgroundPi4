/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_MAIN_DLL.h"

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
