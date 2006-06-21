/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Update.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#ifndef _BPC_UPDATE_DLL_H_
#define _BPC_UPDATE_DLL_H_

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Update(int nFunsterStil);

#endif /* _BPC_UPDATE_DLL_H_ */
