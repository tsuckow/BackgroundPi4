#include "BPC_LOADER_DLL.h"
#include <windows.h>

typedef void (WINAPI * winUpdater)(int);
winUpdater Updater;

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
        //TODO
		MessageBox(NULL,"TODO: Load Main","Loader.DLL",MB_OK);
		return true;
    }
    MessageBox(NULL,"Oh Shit.","Loader.DLL",MB_OK);
    return true;
}
