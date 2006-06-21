/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Executable    * 
 *                       * 
 * * * * * * * * * * * * */
#include <windows.h>

typedef bool (WINAPI * winUpdate)(int);
typedef bool (WINAPI * winLoader)(bool,int);
winUpdate Update;
winLoader Loader;

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,//Unused
                    LPSTR lpszArgument,
                    int nFunsterStil)//Minimized, Maximized, ...
{
	HINSTANCE hLib=NULL;
	while(true)
	{
		bool Temp=false;
		
		//Load Update.dll
		hLib=LoadLibrary("UPDATE.DLL");
		if(hLib==NULL)
		{
			MessageBox(NULL,"Failed To Load \"UPDATE.DLL\"","ERROR: BPC.EXE",MB_OK);
			return 0;
		}
		Update=(winUpdate)GetProcAddress((HMODULE)hLib,"Update");
		if(Update==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"UPDATE.DLL\"","ERROR: BPC.EXE",MB_OK);
			return 0;
		}
		Temp = Update(nFunsterStil);
		FreeLibrary(hLib);
		Update=NULL;
		
		//Load Loader.dll
		hLib=LoadLibrary("LOADER.DLL");
		if(hLib==NULL)
		{
			MessageBox(NULL,"Failed To Load \"LOADER.DLL\"","ERROR: BPC.EXE",MB_OK);
			return 0;
		}
		Loader=(winLoader)GetProcAddress((HMODULE)hLib,"Load");
		if(Loader==NULL)
		{
			FreeLibrary(hLib);
			MessageBox(NULL,"Failed To Bind \"LOADER.DLL\"","ERROR: BPC.EXE",MB_OK);
			return 0;
		}
		Temp=Loader(Temp,nFunsterStil);
		FreeLibrary(hLib);
		Loader=NULL;
		if(Temp)
		{
			//If Loader returns TRUE exit, otherwise update.
			break;
		}
	}
	return 0;
}
