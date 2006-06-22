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
#include "../dvar.hpp"

class TrayIcon
{
	public:
		TrayIcon(HINSTANCE);
		~TrayIcon();
		void Add();
		void Remove();
	private:
		NOTIFYICONDATA tnd;
		HINSTANCE iconinstance;
		DString tiptext;
};

# define DLLIMPORT extern "C" __declspec(dllexport)

DLLIMPORT bool Main(int,HINSTANCE);

#endif /* _BPC_MAIN_DLL_H_ */
