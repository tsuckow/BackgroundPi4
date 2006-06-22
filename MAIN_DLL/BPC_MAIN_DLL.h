/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_MAIN_DLL_H_
#define _BPC_MAIN_DLL_H_
#include <windows.h>

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Main(int,HINSTANCE);

#endif /* _BPC_MAIN_DLL_H_ */
