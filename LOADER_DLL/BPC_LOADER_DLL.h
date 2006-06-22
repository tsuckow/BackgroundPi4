/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Loader.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_LOADER_DLL_H_
#define _BPC_LOADER_DLL_H_
#include <windows.h>

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Load(bool, int, HINSTANCE);

#endif /* _BPC_LOADER_DLL_H_ */
