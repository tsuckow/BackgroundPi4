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
	//      DCName		This is Dynamic
	#define DCHost		"defcon1.hopto.org"
	#define DCPort		3141
	#define DCTimeout	30
	#define DCResume	10
	#define DCDelay		0
	#define DCStats		false
	#define DCArch		"i386"
	
	//Defaults
	this->Name = (DString)"Guest" + (DString)time(NULL) + (DString)rand();
	this->Host = DCHost;
	this->Port = DCPort;
	this->Timeout = DCTimeout;
	this->Resume = DCResume;	
	this->Delay = DCDelay;
	this->Stats = DCStats;
	this->Arch = DCArch;
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
				if(Item=="CPUArch")
				{
					this->Arch=Value;
				}
				if(Item=="Host")
				{
					this->Host=Value;
				}
				else if(Item=="Port")
				{
					this->Port=atoi(Value);
					if(this->Port == 31415) this->Port = 3141;
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
				else if(Item=="Stats")
				{
					this->Stats=(bool)atoi(Value);
				}
				else if(Item=="Name" && Value!="")
				{
					this->Name=Value;
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
		hFile << "#This file is over-writen by BackPi.\n\n";
		hFile << "Name=" << this->Name << std::endl;
		if( this->Host != DCHost )			hFile << "Host=" << this->Host << std::endl;
		if( this->Port != DCPort )			hFile << "Port=" << this->Port << std::endl;
		if( this->Timeout != DCTimeout )	hFile << "Timeout=" << this->Timeout << std::endl;
		if( this->Resume != DCResume )		hFile << "Resume=" << this->Resume << std::endl;		
		if( this->Delay != DCDelay )		hFile << "Delay=" << this->Delay << std::endl;
		if( this->Arch != DCArch )			hFile << "CPUArch=" << this->Arch << std::endl;
		if( this->Stats != DCStats )		hFile << "Stats=" << this->Stats << std::endl;
		return true;
	}
	else
	{
		return false;
	}
	hFile.close();
	MessageBox(NULL,"Oh Shit.\nConfig::Save","Error: Main.DLL",MB_OK);
}
