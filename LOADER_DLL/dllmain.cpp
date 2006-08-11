/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Loader.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_LOADER_DLL.h"
#include <windows.h>

typedef void (WINAPI * winUpdater)(int,HINSTANCE);
typedef bool (WINAPI * winMainf)(int,HINSTANCE);
winUpdater Updater;
winMainf Main;

bool APIENTRY Load(bool Update_Updater, int nFunsterStil,HINSTANCE thisinstance)
{
	if(Update_Updater==true)
	{
        HINSTANCE hLib=NULL;
        hLib=LoadLibrary("UPDATER.DLL");
		if(hLib==NULL)
		{
			MessageBox(NULL,"Failed To Load \"UPDATER.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return true;
		}
		Updater=(winUpdater)GetProcAddress((HMODULE)hLib,"Update");
		if(Updater==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"UPDATER.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return true;
		}
		Updater(nFunsterStil,hLib);
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
			return true;
		}
		Main=(winMainf)GetProcAddress((HMODULE)hLib,"Main");
		if(Main==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"MAIN.DLL\"","ERROR: LOADER.DLL",MB_OK);
			return true;
		}
		bool Temp = true;
		Temp=Main(nFunsterStil,hLib);
		FreeLibrary(hLib);
		Main=NULL;
		return Temp;
    }
    MessageBox(NULL,"Oh Shit.","Loader.DLL",MB_OK);
    return true;
}
