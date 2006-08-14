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
#include <sys/timeb.h>
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
#define ID_MENU_SETT  	(WM_USER+2)
#define ID_MENU_HELP	(WM_USER+3)

#define SUM_PREC 50

extern bool doExit;
extern HWND Inviswnd;
extern HWND Splashwnd;
extern HANDLE thread;
extern HINSTANCE thisinstance;

extern LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog
extern LRESULT CALLBACK StatusDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Status Dialog
extern LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog
extern LRESULT CALLBACK SettingDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog

DWORD WINAPI MainThread(void*);

class Config
{
	public:
		Config();
		bool Open();
		bool Save();
		
		//Vars
		short Port;
		int Delay;
		int Resume;
		int Timeout;
		DString Host;
		DString Name;
		bool Stats;
		
};

extern Config Conf;

class StatusDlg
{
	#define StatusDlgMax 8
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
				this->Stats[7]="...";
				this->Stats[8]="...";
				for(unsigned short i = 0;i<=StatusDlgMax;i++)
                {
					this->Update(i,this->Stats[i]);
				}
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
		DString Stats[StatusDlgMax+1];
};

extern StatusDlg Stat;


class SettingDlg
{
	#define SettingDlgMax 5
	public:
		SettingDlg() 
			{
				this->Settingwnd=NULL;
			};
		void Reset()
			{
				this->Setts[0]=Conf.Host;
				this->Setts[1]=Conf.Port;
				this->Setts[2]=Conf.Timeout;
				this->Setts[3]=Conf.Resume;
				this->Setts[4]=Conf.Delay;
				this->Setts[5]=Conf.Name;
			};
		~SettingDlg() {DestroyWindow(this->Settingwnd);this->Settingwnd=NULL;};
		bool Close()
			{
				bool changed = false;
				for(unsigned short i = 0;i<=SettingDlgMax;i++)
                {
					char buffer[998];
					GetDlgItemText(this->Settingwnd,IDC_HOST+i,buffer,998);
					this->Setts[i] = buffer;
				}
				
				if(this->Setts[0]!=Conf.Host) changed = true;
				if(this->Setts[1]!=(DString)Conf.Port) changed = true;
				if(this->Setts[2]!=(DString)Conf.Timeout) changed = true;
				if(this->Setts[3]!=(DString)Conf.Resume) changed = true;
				if(this->Setts[4]!=(DString)Conf.Delay) changed = true;
				if(this->Setts[5]!=(DString)Conf.Name) changed = true;
				
				if(this->Setts[5]=="")
				{
					MessageBox(this->Settingwnd,"Username CANNOT be blank.","Settings",MB_YESNOCANCEL | MB_ICONHAND | MB_TASKMODAL);
					return false;
				}
				
				if(changed)
				{
					switch(MessageBox(this->Settingwnd,"Save changes?","Settings",MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL))
					{
						case IDYES:
							Conf.Host=this->Setts[0];
							Conf.Port=atoi(this->Setts[1]);
							Conf.Timeout=atoi(this->Setts[2]);
							Conf.Resume=atoi(this->Setts[3]);
							Conf.Delay=atoi(this->Setts[4]);
							Conf.Name=this->Setts[5];
							if(!Conf.Save())
							{
								MessageBox(NULL,"Failed to save to Config.cfg\nYou may not have permission to write to that directory.","Error: Main.DLL",MB_OK);
							}
						case IDNO:
							return true;
							break;
						case IDCANCEL:
							return false;
							break;
					}
				}
				else
				{
					return true;
				}
			}
			
		void Update(unsigned short n,DString Text)
			{
				if(n<=SettingDlgMax)
				{
					this->Setts[n]=Text;
					if (IsWindow(this->Settingwnd))
					{
						SetDlgItemText(this->Settingwnd,IDC_HOST+n,Text);
					}
				}
			};
		void Create()
			{			
				if (!IsWindow(this->Settingwnd)) 
                {
					this->Reset();
                    this->Settingwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SETT),NULL,(DLGPROC)SettingDialogProcedure);
                    ShowWindow(this->Settingwnd,SW_SHOW);
                    for(unsigned short i = 0;i<=SettingDlgMax;i++)
                    {
						this->Update(i,this->Setts[i]);
					}
                }    
                SetForegroundWindow(this->Settingwnd);
			};
		HWND Settingwnd;
		DString Setts[SettingDlgMax+1];
};

extern SettingDlg Sett;


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

extern TrayIcon * hGlobalIcon;

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Main(int,HINSTANCE);

#endif /* _BPC_MAIN_DLL_H_ */
