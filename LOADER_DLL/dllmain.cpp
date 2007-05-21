/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Loader.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_LOADER_DLL.h"

typedef void (WINAPI * winUpdater)(int,HINSTANCE);
typedef bool (WINAPI * winMainf)(int,HINSTANCE);
winUpdater Updater;
winMainf Main;

DString Arch = "i386";//Default CPU Arch for GMP
bool doMutex = true; //Default Single Instance of program

bool ParseLine(DString Line, DString & Item, DString & Value)
{
     while(Line.Getchar() != '=' && !Line.Isend())
     {
                Line.Nextchar();   
     }
     if(Line.Getchar() != '=')
     {
            return false;
     }
     Item=Line.Getsbc();
     Value=Line.Getspc();
     
     return true;
}

bool ConfigOpen() //return false on fail
{
	std::fstream hFile;
	hFile.open("Config.cfg",std::ios::in);
	if(hFile.is_open())
	{
		char Buffer[1001];
		while(!hFile.eof())
		{
			DString Item;
			DString Value;
			hFile.getline(Buffer,1000);
			if(ParseLine(Buffer,Item,Value))
			{
				//Config If's
				if(Item=="CPUArch")
				{
					Arch=Value;
				}
				else if(Item=="OneInstance")
				{
					doMutex=(bool)atoi(Value);
				}
				//End Config If's
			}
		}
		hFile.close();
		return true;
	}
	else
	{
		return false;
	}
	hFile.close();
	MessageBox(NULL,"Oh Shit.\nConfigOpen","Error: Loader.DLL",MB_OK);
	return false;
}

bool FileMove(DString Src, DString Dst)
{
	if(Src==Dst) return false;
	
	std::ifstream in;
	std::ofstream out;
	in.open(Src,std::ios::in | std::ios::binary);
	out.open(Dst,std::ios::out | std::ios::binary);
	
	if( !(in.is_open() && out.is_open()) )
	{
		in.close();
		out.close();
		return false;
	}
	
	char buffer[1024];
	while(!in.eof())
	{
		in.read(buffer,1024);
		int length = in.gcount();
		out.write(buffer,length);
	}
	
	in.close();
	out.close();
	return true;
}

bool APIENTRY Load(bool Update_Updater, int nFunsterStil,HINSTANCE thisinstance)
{
	ConfigOpen();
	
	HANDLE PMutex = NULL;
	
	if(OpenMutex(MUTEX_ALL_ACCESS, false, "BackPi4")!=NULL)
	{
		if(doMutex)
		{
			MessageBox(NULL,"Background Pi 4 is already running.","ERROR: LOADER.DLL",MB_OK);
			return true;
		}
	}
	else
	{
		PMutex = CreateMutex(NULL, FALSE, "BackPi4");
	}
	
	HINSTANCE hLib=NULL;
	if(Update_Updater==true)
	{
        hLib=LoadLibrary("UPDATER.dll");
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
		ReleaseMutex(PMutex);
    	CloseHandle(PMutex);
		return false;
    }
    else
    {
		
		//What CPU we running?
		
		//ConfigOpen();
		
		//Put the GMP DLL into place
		
		if(!FileMove("libgmp_" + Arch + ".dll","libgmp-3.dll"))
		{
			MessageBox(NULL,"Failed To Prepare GMP Library\nFailed to use CPU Type: " + Arch,"ERROR: LOADER.DLL",MB_OK);
			return true;
		}
		
		//Done.		
		
        hLib=LoadLibrary("MAIN.dll");

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
		if(!Temp) 
		{
			ReleaseMutex(PMutex);
    		CloseHandle(PMutex);
		}
		return Temp;
    }
    MessageBox(NULL,"Oh Shit.","Loader.DLL",MB_OK);
    return true;
}
