/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: NO Update.DLL * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_UPDATE_DLL_H_
#define _BPC_UPDATE_DLL_H_
#include <windows.h>

# define DLLIMPORT extern "C" __declspec(dllexport)
DLLIMPORT bool Verify(HINSTANCE);

DLLIMPORT bool Update(int,HINSTANCE);

#endif /* _BPC_UPDATE_DLL_H_ */
