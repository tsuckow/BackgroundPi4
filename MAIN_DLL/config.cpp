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
	Resume = 10;
}

bool Config::Open() //return false on fail
{
	return true;
}
