/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_MAIN_DLL_H_
#define _BPC_MAIN_DLL_H_

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Main(int nFunsterStil);

#endif /* _BPC_MAIN_DLL_H_ */
