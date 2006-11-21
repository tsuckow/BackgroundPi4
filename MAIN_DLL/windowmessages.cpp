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
	static UINT s_uTaskbarRestart;
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            s_uTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
            
            return false;
            break;
        case WM_NOTIFYICON:
			if ((UINT)lParam == WM_RBUTTONUP){
            	POINT cord;
            	GetCursorPos(&cord);
            	HMENU menu;
            	menu = CreatePopupMenu();
            	AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_ABOUT,"&About");
				AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_HELP,"&Help");
            	AppendMenu(menu,MF_ENABLED|MF_STRING, ID_MENU_SETT,"Se&ttings");
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
        case WM_CLOSE:
        case WM_RQUIT:
			Windowlessquit = true;
			PostQuitMessage(0);
			break;
		case WM_QUERYENDSESSION:
			return true;
		case WM_ENDSESSION:
			Windowlessquit = true;
			PostQuitMessage(0);
			return 0;
		case WM_COMMAND:
            switch(LOWORD(wParam))
            {
				case ID_MENU_EXIT:
					Windowlessquit = true;
					PostQuitMessage (0);
                    break;
                case ID_MENU_STATUS:
					Stat.Create();
					break;
				case ID_MENU_SETT:
					Sett.Create();
					break;
				case ID_MENU_HELP:
                    WinHelp(hwnd,"HELP.HLP",HELP_FINDER,0);
                    break;
                case ID_MENU_ABOUT:
					if (!IsWindow(Aboutwnd)) 
                	{
                    	Aboutwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_ABOUT),NULL,(DLGPROC)AboutDialogProcedure);
                    	ShowWindow(Aboutwnd,SW_SHOW);
                	}    
                	SetForegroundWindow(Aboutwnd);
                	break;
			}
			return 0;
            break;
        default:
			if( message == s_uTaskbarRestart)
			{
				hGlobalIcon->Remove();
				Sleep(500);
				hGlobalIcon->Add();
				break;
			}
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

//Settings
LRESULT CALLBACK SettingDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    switch (message)/* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            
            {
				SendMessage(GetDlgItem(hwnd,IDC_ARCH),CB_ADDSTRING,0,(LPARAM)(LPCTSTR)Conf.Arch);
				std::ifstream AList;
				AList.open("Arch.txt",std::ios::in);
				if(AList.is_open())
				{
					char Buffer[1001];
					while(!AList.eof())
					{
						AList.getline(Buffer,1000);
            			if(Conf.Arch != Buffer) SendMessage(GetDlgItem(hwnd,IDC_ARCH),CB_ADDSTRING,0,(LPARAM)(LPCTSTR)Buffer);
					}
					AList.close();
				}
				//SendMessage(GetDlgItem(hwnd,IDC_ARCH),CB_SETCURSEL,1,0);
				SendMessage(GetDlgItem(hwnd,IDC_ARCH),CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)Conf.Arch);
			}
            return true;      
            break;
        case WM_CLOSE:
			if(Sett.Close())
			{
            	DestroyWindow(hwnd);
            	Sett.Settingwnd=NULL;
			}
            return 0;
            break;
        default:                      /* for messages that we don't deal with */
            //return DefWindowProc (hwnd, message, wParam, lParam);
            return false;
    }
    return 0;
}

LRESULT CALLBACK AboutDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    switch (message)/* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            //SetClassLong(hwnd,GCL_HBBACKGROUND,NULL);
			return true;
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            Aboutwnd=NULL;
            return 0;
            break;
        default:                      /* for messages that we don't deal with */
            //return DefWindowProc (hwnd, message, wParam, lParam);
            return false;
    }
    return 0;
}
