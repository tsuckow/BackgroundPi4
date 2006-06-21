#ifndef _BPC_LOADER_DLL_H_
#define _BPC_LOADER_DLL_H_

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Load(bool Update_Updater, int nFunsterStil);

#endif /* _BPC_LOADER_DLL_H_ */
