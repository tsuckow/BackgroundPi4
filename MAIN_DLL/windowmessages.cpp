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
            //thread = CreateThread(NULL,0,MainThread,NULL,0,NULL); //Returns handle to thread, may be useful...
            return false;
            break;
        case WM_NOTIFYICON:
			if ((UINT)lParam == WM_RBUTTONUP){
            	POINT cord;
            	GetCursorPos(&cord);
            	HMENU menu;
            	menu = CreatePopupMenu();
            	AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_STATUS,"&Status");
            	AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_EXIT,"&Exit");
            	SetForegroundWindow(hwnd);
            	TrackPopupMenu(menu,TPM_RIGHTALIGN,cord.x,cord.y,0,hwnd,NULL);
            	DestroyMenu(menu);
            	PostMessage(hwnd, WM_NULL, 0, 0);
        	}
        	else if ((UINT)lParam == WM_LBUTTONDBLCLK)
        	{
				PostMessage(hwnd,WM_COMMAND,ID_MENU_STATUS,0);
			}    
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
                case ID_MENU_STATUS:
					Stat.Create();
					break;
			}
			return 0;
            break;
        default:
            return false;
    }
    return 0;
}

//Status
LRESULT CALLBACK StatusDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            SetDlgItemText(hwnd,IDC_MODE,"Loading...");
            SetDlgItemText(hwnd,IDC_PERC,"N/A");
            SetDlgItemText(hwnd,IDC_ELAP,"N/A");
            SetDlgItemText(hwnd,IDC_TIME,"N/A");
            SetDlgItemText(hwnd,IDC_ITERN,"N/A");
            SetDlgItemText(hwnd,IDC_ITERO,"N/A");
            SetDlgItemText(hwnd,IDC_CALC,"N/A");
            
            //_beginthread(StatusUpdate, 0, NULL);
            
            return false;
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            Stat.Statuswnd=NULL;
            return 0;
            break;
        default:                      /* for messages that we don't deal with */
            //return DefWindowProc (hwnd, message, wParam, lParam);
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
        case WM_CLOSE:
			DestroyWindow(hwnd);
    		Splashwnd = NULL;
			break;
        default:
            return false;
    }
    return 0;
}
