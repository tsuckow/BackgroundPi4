#ifndef _BPC_MAIN_DLL_H_
#define _BPC_MAIN_DLL_H_
/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#include <windows.h>
#include "exresource.h"
#include "../dvar.hpp"

#define WM_RQUIT 		(WM_APP+0)
#define WM_NOTIFYICON 	(WM_APP+1)
#define WM_SETLOADTEXT 	(WM_APP+2)

#define ID_MENU_EXIT    (WM_USER+0)

extern bool doExit;
extern HANDLE thread;
extern HINSTANCE thisinstance;

DWORD WINAPI MainThread(void*);

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
