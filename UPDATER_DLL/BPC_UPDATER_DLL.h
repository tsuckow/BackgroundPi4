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

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT void Update(int, HINSTANCE);

#endif /* _BPC_UPDATER_DLL_H_ */
