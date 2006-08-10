/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Updater.DLL   * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_UPDATER_DLL_H_
#define _BPC_UPDATER_DLL_H_
#include <windows.h>
#include <fstream>
#include <winsock2.h>
#include "../dvar.hpp"
#include "../md5.hpp"

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT void Update(int, HINSTANCE);

#endif /* _BPC_UPDATER_DLL_H_ */
