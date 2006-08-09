/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Update.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_UPDATE_DLL_H_
#define _BPC_UPDATE_DLL_H_
#include <windows.h>
#include <fstream>
#include <winsock2.h>
#include "../dvar.hpp"
#include "../md5.hpp"

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Update(int,HINSTANCE);

#endif /* _BPC_UPDATE_DLL_H_ */
