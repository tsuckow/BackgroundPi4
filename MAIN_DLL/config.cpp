/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
 #include "BPC_MAIN_DLL.h"
 
Config::Config()
{
	//Defaults
	this->Port = 31415;
	this->Resume = 10;
	this->Timeout = 30;
	this->Delay = 0;
	this->Host = "defcon1.hopto.org";
}

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

bool Config::Open() //return false on fail
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
				if(Item=="Host")
				{
					this->Host=Value;
				}
				else if(Item=="Port")
				{
					this->Port=atoi(Value);
				}
				else if(Item=="Resume")
				{
					this->Resume=atoi(Value);
				}
				else if(Item=="Timeout")
				{
					this->Timeout=atoi(Value);
				}
				else if(Item=="Delay")
				{
					this->Delay=atoi(Value);
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
	MessageBox(NULL,"Oh Shit.\nConfig::Open","Error: Main.DLL",MB_OK);
}

bool Config::Save()
{
	std::fstream hFile;
	hFile.open("Config.cfg",std::ios::out);
	if(hFile.is_open())
	{
		hFile << "Host=" << this->Host << std::endl;
		hFile << "Port=" << this->Port << std::endl;
		hFile << "Resume=" << this->Resume << std::endl;
		hFile << "Timeout=" << this->Timeout << std::endl;
		hFile << "Delay=" << this->Delay << std::endl;
		return true;
	}
	else
	{
		return false;
	}
	hFile.close();
	MessageBox(NULL,"Oh Shit.\nConfig::Save","Error: Main.DLL",MB_OK);
}
