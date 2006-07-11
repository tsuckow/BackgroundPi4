#ifndef _BPC_MAIN_DLL_H_
#define _BPC_MAIN_DLL_H_
/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#define PI_COM_VERSION 2
 
#include <windows.h>
#include <fstream>
#include <math.h>
#include <gmp.h>
#include <winsock2.h>
#include "exresource.h"
#include "../dvar.hpp"

#define WM_RQUIT 		(WM_APP+0)
#define WM_NOTIFYICON 	(WM_APP+1)
#define WM_SETLOADTEXT 	(WM_APP+2)

#define ID_MENU_EXIT    (WM_USER+0)
#define ID_MENU_STATUS  (WM_USER+1)

#define SUM_PREC 50

extern bool doExit;
extern HWND Inviswnd;
extern HWND Splashwnd;
extern HANDLE thread;
extern HINSTANCE thisinstance;

extern LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog
extern LRESULT CALLBACK StatusDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Status Dialog
extern LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog

DWORD WINAPI MainThread(void*);

class StatusDlg
{
	#define StatusDlgMax 6
	public:
		StatusDlg() 
			{
				this->Statuswnd=NULL;
				this->Reset();
			};
		void Reset()
			{
				this->Stats[0]="...";
				this->Stats[1]="...";
				this->Stats[2]="...";
				this->Stats[3]="...";
				this->Stats[4]="...";
				this->Stats[5]="...";
				this->Stats[6]="Init...";
			};
		~StatusDlg() {DestroyWindow(this->Statuswnd);this->Statuswnd=NULL;};		
		void Update(unsigned short n,DString Text)
			{
				if(n<=StatusDlgMax)
				{
					this->Stats[n]=Text;
					if (IsWindow(this->Statuswnd))
					{
						SetDlgItemText(this->Statuswnd,IDC_CALC+n,Text);
					}
				}
			};
		void Create()
			{
				if (!IsWindow(this->Statuswnd)) 
                {
                    this->Statuswnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_STAT),NULL,(DLGPROC)StatusDialogProcedure);
                    ShowWindow(this->Statuswnd,SW_SHOW);
                    for(unsigned short i = 0;i<=StatusDlgMax;i++)
                    {
						this->Update(i,this->Stats[i]);
					}
                }    
                SetForegroundWindow(this->Statuswnd);
			};
		HWND Statuswnd;
	private:
		DString Stats[StatusDlgMax+1];
};

extern StatusDlg Stat;

class Config
{
	public:
		Config();
		bool Open();
		bool Save();
		
		//Vars
		short Port;
		int Resume;
		int Timeout;
		int Delay;
		DString Host;
		
};

extern Config Conf;

class TrayIcon
{
	public:
		TrayIcon(HINSTANCE);
		~TrayIcon();
		void Add();
		void Remove();
	private:
		NOTIFYICONDATA tnd;
		HINSTANCE iconinstance;
		DString tiptext;
};

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Main(int,HINSTANCE);

#endif /* _BPC_MAIN_DLL_H_ */
