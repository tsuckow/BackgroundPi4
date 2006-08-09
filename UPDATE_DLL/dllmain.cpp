/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Update.DLL    * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_UPDATE_DLL.h"
#include <windows.h>
#include "exresource.h"

#define WM_RQUIT (WM_APP+0)
#define WM_SETLOADTEXT (WM_APP+1)

bool Update_Updater = false;

bool Windowlessquit = false;

HWND Inviswnd = NULL;
HWND Splashwnd = NULL;

HANDLE thread = NULL;

LRESULT CALLBACK InvisDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Hidden Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND, UINT, WPARAM, LPARAM); //Spalsh Dialog

HINSTANCE thisinstance = NULL;

bool DoRecv(SOCKET & handle, DString & Buffer)
{
	Buffer="";
	char buf[2];
	buf[1]=0;
	ssize_t length;
	bool gotRet = false;
	
	length = recv(handle,buf,1,0);
	while( length > 0 )
	{
		//std::cout << buf[0] << ":" << (int)buf[0] << " ";
		if(gotRet && buf[0]=='\n')
		{
			return true;
		}
		else
		{
			if(gotRet)
			{
				Buffer+=(char)'\r';
			}
			if(buf[0]=='\r')
			{
				gotRet=true;
			}
			else
			{
				gotRet=false;
				if(buf[0]==8)
				{
					while(!Buffer.Isend())
					{
						Buffer.Nextchar();
					}
					Buffer = Buffer.Getsbc();
				}
				else
				{
					Buffer+=buf[0];
				}
			}
		}
		length = recv(handle,buf,1,0);
	}
	return false;
}

bool DoSend(SOCKET & Handle,DString const & Msg)
{
	if(send(Handle, Msg, strlen(Msg), 0) == -1)
	{
		return false;
	}
	return true;
}

bool ParseCommLine(DString Line, DString & Item, DString & Value)
{
     while(Line.Getchar() != ':' && !Line.Isend())
     {
                Line.Nextchar();   
     }
     if(Line.Getchar() != ':')
     {
            return false;
     }
     Item=Line.Getsbc();
     Value=Line.Getspc();
     
     return true;
}

DWORD WINAPI UpdateThread(void*)
{
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Connecting To Server...");
	
	WSADATA wsaData;
    if ( WSAStartup( MAKEWORD(2,2), &wsaData ) != NO_ERROR )
    {
        MessageBox(NULL,"Background Pi CLIENT Encountered An Error Loading Windows Sockets.\nTry Restarting Your Computer.","Winsock",MB_OK);
        Update_Updater = true;
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
        ExitThread(0);
    }
    
    SOCKET m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( m_socket == INVALID_SOCKET )
    {
		WSACleanup();
        MessageBox(NULL,"Background Pi CLIENT Encountered An Error Loading The Socket.\nTry Restarting Your Computer.\n\nCODE: " + (DString)WSAGetLastError(),"Winsock",MB_OK);
        Update_Updater = true;
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
        ExitThread(0);
    }
    
    // Connect to a server.
    sockaddr_in clientService;
    memset(&clientService,'\0',sizeof(clientService)); 
    clientService.sin_family = AF_INET;
    
    struct hostent *phostent;
    if ((phostent = gethostbyname("defcon1.hopto.org")) != NULL)
    {
        clientService.sin_addr.s_addr = *(unsigned long*)phostent->h_addr_list[0];
    }    
    else
    {
        clientService.sin_addr.s_addr = inet_addr("defcon1.hopto.org <- ip");// 32-bit network address
    }
       
    clientService.sin_port = htons( 3141 ); //PORT

    if ( connect( m_socket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR)
    {
        WSACleanup();
        MessageBox(NULL,"Background Pi Client FAILED to connect to the server.\nTry verifing settings.","Winsock",MB_OK);
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
        ExitThread(0);
    }
    
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Checking For Updates...");
	
	DString Buffer;
	DString Item;
	DString Value;
	
	if(!DoSend(m_socket,"101:Hello\r\n"))
	{
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
		ExitThread(0);
	}
	if(!DoRecv(m_socket,Buffer))
	{
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
		ExitThread(0);
	}
	
	if(ParseCommLine(Buffer,Item,Value) && Item=="201")
	{
		if(!DoSend(m_socket,"150:Update\r\n"))
		{
			Windowlessquit = true;
			PostMessage(Inviswnd,WM_RQUIT,0,0);
			ExitThread(0);
		}
		
		while(true)//Update all files
		{
			DString File;
			if(!DoRecv(m_socket,Buffer))
			{
				Windowlessquit = true;
				PostMessage(Inviswnd,WM_RQUIT,0,0);
				ExitThread(0);
			}
			if(ParseCommLine(Buffer,Item,Value) && Item=="250")
			{
				if(!DoRecv(m_socket,Buffer))
				{
					Windowlessquit = true;
					PostMessage(Inviswnd,WM_RQUIT,0,0);
					ExitThread(0);
				}
				if(ParseCommLine(Buffer,Item,Value) && Item=="205")
				{
					File = Value;
					{
						DString temp = "Verifying: " + File;
						PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)(const char*)temp);
					}
					MD5 md5class;
					std::ifstream tmp;
					tmp.open(File,std::ios::in);
					md5class.update(tmp);
					md5class.finalize();
					tmp.close();
					if(!DoSend(m_socket,"105:"))
					{
						Windowlessquit = true;
						PostMessage(Inviswnd,WM_RQUIT,0,0);
						ExitThread(0);
					}
					if(!DoSend(m_socket,md5class.hex_digest()))
					{
						Windowlessquit = true;
						PostMessage(Inviswnd,WM_RQUIT,0,0);
						ExitThread(0);
					}
					if(!DoSend(m_socket,"\r\n"))
					{
						Windowlessquit = true;
						PostMessage(Inviswnd,WM_RQUIT,0,0);
						ExitThread(0);
					}
					
					if(!DoRecv(m_socket,Buffer))
					{
						Windowlessquit = true;
						PostMessage(Inviswnd,WM_RQUIT,0,0);
						ExitThread(0);
					}
					if(ParseCommLine(Buffer,Item,Value) && (Item=="202" || Item=="204"))
					{
						if(Item == "202")//If Up2Date
						{
							continue;
						}
						else if(Item = "204")
						{
							{
								DString temp = "Updating: " + File;
								PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)(const char*)temp);
							}
							//Update File
						}
						else
						{
						 	//Shit
						 	MessageBox(NULL,"Impossible,Code can ONLY be 202 or 204.\nWas: " + Item,"Impossible",MB_OK);
						 	ExitThread(0);
						}
					}
					else
					{
						//Unexpected command
						DoSend(m_socket,"106:Goodbye, Unexpected Response\r\n");
						Windowlessquit = true;
						PostMessage(Inviswnd,WM_RQUIT,0,0);
						ExitThread(0);
					}
				}
				else
				{
					//Unexpected command
					DoSend(m_socket,"106:Goodbye, Unexpected Response\r\n");
					Windowlessquit = true;
					PostMessage(Inviswnd,WM_RQUIT,0,0);
					ExitThread(0);
				}			
			}
			else
			{
				//Unexpected command
				DoSend(m_socket,"106:Goodbye, Unexpected Response\r\n");
				Windowlessquit = true;
				PostMessage(Inviswnd,WM_RQUIT,0,0);
				ExitThread(0);
			}
			
		}//Update All	
			
	}
	else
	{
		//Didn't Say Hello
		DoSend(m_socket,"106:Goodbye, You didn't say Hello\r\n");
		Windowlessquit = true;
		PostMessage(Inviswnd,WM_RQUIT,0,0);
		ExitThread(0);
	}
	
	
	
	Sleep(1000);
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Done...");
	closesocket(m_socket);
	WSACleanup();
	
	//Update_Updater = true;
	Windowlessquit = true;
	PostMessage(Inviswnd,WM_RQUIT,0,0);
}

bool APIENTRY Update(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	
	//Display Splash Screen If NOT started in minimized
	Inviswnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_INVIS),NULL,(DLGPROC)InvisDialogProcedure);
    
    if(nFunsterStil != 7)
    {
		Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),Inviswnd,(DLGPROC)SplashDialogProcedure);
		ShowWindow (Splashwnd, SW_SHOW);
    }
	
	thread = CreateThread(NULL,0,UpdateThread,NULL,0,NULL); //Returns handle to thread, may be useful...
	
	if(IsWindow(Inviswnd))
	{
		MSG messages;
		// Run the message loop. It will run until GetMessage() returns 0 
    	while (GetMessage (&messages, NULL, 0, 0) > 0)
    	{
        	if(!IsDialogMessage(Inviswnd, &messages) && !IsDialogMessage(Splashwnd, &messages))
        	{
            	// Translate virtual-key messages into character messages 
            	TranslateMessage(&messages);
            	// Send message to WindowProcedure 
            	DispatchMessage(&messages);
        	}
    	}
	}
	else
	{
		while(!Windowlessquit)
		{
			Sleep(100);
		}
	}
    DestroyWindow(Splashwnd);
    Splashwnd=NULL;
    DestroyWindow(Inviswnd);
    Inviswnd=NULL;
    Sleep(50);
    CloseHandle(thread);
	return Update_Updater;
}

//Invis Dialog
LRESULT CALLBACK InvisDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            return false;
            break;
        case WM_RQUIT:
			PostQuitMessage(0);
			break;
        default:
            return false;
    }
    return 0;
}

//Splash Dialog
LRESULT CALLBACK SplashDialogProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_INITDIALOG:
            SetClassLong(hwnd,GCL_HICON,(long) LoadIcon(thisinstance,"A"));
            SetClassLong(hwnd,GCL_HICONSM,(long) LoadIcon(thisinstance,"A"));
            SetDlgItemText(hwnd,IDC_LOAD,(char *)"Init...");
            return false;
            break;
		case WM_SETLOADTEXT:
            return SetDlgItemText(hwnd,IDC_LOAD,(char *)lParam);
        default:
            return false;
    }
    return 0;
}
