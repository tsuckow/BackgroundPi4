/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Loader.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_LOADER_DLL.h"
#include <windows.h>

typedef void (WINAPI * winUpdater)(int);
typedef bool (WINAPI * winMainf)(int);
winUpdater Updater;
winMainf Main;

bool APIENTRY Load(bool Update_Updater, int nFunsterStil)
{
	if(Update_Updater==true)
	{
        HINSTANCE hLib=NULL;
        hLib=LoadLibrary("UPDATER.DLL");
		if(hLib==NULL)
		{
			MessageBox(NULL,"Failed To Load \"UPDATER.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return 0;
		}
		Updater=(winUpdater)GetProcAddress((HMODULE)hLib,"Update");
		if(Updater==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"UPDATER.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return 0;
		}
		Updater(nFunsterStil);
		FreeLibrary(hLib);
		Updater=NULL;
		return false;
    }
    else
    {
        HINSTANCE hLib=NULL;
        hLib=LoadLibrary("MAIN.DLL");
		if(hLib==NULL)
		{
			MessageBox(NULL,"Failed To Load \"MAIN.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return 0;
		}
		Main=(winMainf)GetProcAddress((HMODULE)hLib,"Main");
		if(Main==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"MAIN.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return 0;
		}
		bool Temp = true;
		Temp=Main(nFunsterStil);
		FreeLibrary(hLib);
		Main=NULL;
		return Temp;
    }
    MessageBox(NULL,"Oh Shit.","Loader.DLL",MB_OK);
    return true;
}
