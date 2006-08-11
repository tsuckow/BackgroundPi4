/* * * * * * * * * * * * * 
 * Background Pi Client  * 
 * v4                    * 
 *                       * 
 * Module: Main.DLL      * 
 *                       * 
 * * * * * * * * * * * * */
#include "BPC_MAIN_DLL.h"
#define STATS_VERSION

bool doExit = true;

HWND Inviswnd = NULL;
StatusDlg Stat;
HWND Splashwnd = NULL;
Config Conf;

HANDLE thread = NULL;
#ifdef STATS_VERSION
HANDLE statthread = NULL;
#endif
HINSTANCE thisinstance = NULL;

TrayIcon * hGlobalIcon = NULL;


//Tray Icon
TrayIcon::TrayIcon(HINSTANCE instance)
{
	iconinstance=instance;
	tiptext = "";
	Add();
}

TrayIcon::~TrayIcon()
{
	Remove();
}

void TrayIcon::Add()
{
	lstrcpyn(tnd.szTip, (LPCTSTR)tiptext, sizeof(tnd.szTip));
	tnd.uFlags = NIF_ICON | NIF_TIP|NIF_MESSAGE;
	tnd.uID =(UINT)IDI_WINLOGO;
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = Inviswnd;
    tnd.uCallbackMessage = WM_NOTIFYICON;
    tnd.hIcon = LoadIcon(iconinstance,"A");
    Shell_NotifyIcon(NIM_ADD, &tnd);
}
void TrayIcon::Remove()
{
    Shell_NotifyIcon(NIM_DELETE, &tnd);
}

void DIVN(mpz_t & t,const mpz_t & a,mpz_t & v,const int & vinc,mpz_t & kq,const unsigned int & kqinc)
{
	mpz_add_ui(kq,kq,kqinc);
	if (mpz_cmp(kq,a) >= 0)
	{
        mpz_mod(kq,kq,a);

        if (mpz_cmp_ui(kq,0) == 0)
		{
			mpz_t temp;
    		mpz_init(temp);
            
			mpz_set_si(temp,vinc);
            mpz_addmul_ui(v,temp,mpz_remove(t,t,a));
        	
        	mpz_clear(temp);
		}
    }
}

int is_prime(const mpz_t & n)
{
    if (mpz_divisible_ui_p(n,2) != 0) return false;

	mpz_t r,i;
    mpz_init (i);
    mpz_init (r);
    
	mpz_sqrt(r,n);
    for(mpz_set_ui(i,3);mpz_cmp(i,r)<=0;mpz_add_ui(i,i,2))
    {
        if (mpz_divisible_p(n,i) != 0)
        {
            mpz_clear(r);
            mpz_clear(i);
            return false;
        }
    }
    mpz_clear(r);
    mpz_clear(i);
    return true;
}

/* returns the prime number immediatly after n */

void next_prime(mpz_t & r,const mpz_t & n,const mpz_t & N)
{
   short probab = 0;
   mpz_set(r,n);
   do{
      mpz_nextprime (r,r);
      probab = mpz_probab_prime_p(r,7);//# Iterations, 7 is good
   } while( !( (probab==1)?is_prime(r):probab ) && mpz_cmp(r,N) <= 0 ); //WARNING: Terinary Operator Use! Read /w care.
   //added: detects that n is higher than N and returns
}

void PrimeCount(mpf_t & r,const mpz_t & bound)
{
    mpf_set_d(r, log( mpz_get_d(bound) ) );
    mpf_t sub;
    mpf_init_set_d(sub,1.083);
    mpf_sub(r,r,sub);
    mpf_set_z(sub,bound);
    mpf_div(r,sub,r);
    mpf_clear(sub);
}

void percentc (mpf_t & percent, const mpz_t & prime,const mpz_t & bound)
{
    mpf_t boundp;
    mpf_init(boundp);
    PrimeCount(percent,prime);
    PrimeCount(boundp,bound);
    mpf_div(percent,percent,boundp);
    mpf_mul_ui(percent,percent,100);
    mpf_clear(boundp);
}

DString timeformat(const mpz_t & sa)//time (seconds)
{
    
    mpz_t m,h,s;
    mpz_init(m);
    mpz_init(h);
    mpz_init_set(s,sa);
    if(mpz_cmp_ui(s,0) < 0) mpz_set_ui(s,0);
    mpz_tdiv_qr_ui ( m, s, s, 60);
    mpz_tdiv_qr_ui ( h, m, m, 60);
    DString temp;
    if(mpz_get_ui(h) < 10)
        temp = (DString)"0\0" + (DString)mpz_get_ui(h);
    else
        temp = (DString)mpz_get_ui(h);
        
    if(mpz_get_ui(m) < 10)
        temp += (DString)":" + (DString)"0\0" + (DString)mpz_get_ui(m);
    else
        temp += (DString)":" + (DString)mpz_get_ui(m);
    
    if(mpz_get_ui(s) < 10)
        temp += (DString)":" + (DString)"0\0" + (DString)mpz_get_ui(s);
    else
        temp += (DString)":" + (DString)mpz_get_ui(s);
    mpz_clear(m);
    mpz_clear(h);
    mpz_clear(s);
    return temp;
}

DString timeleft(const time_t & t,const mpf_t & psa,const mpf_t & xa)//Start Time, Start %, Current %
{
    mpf_t ps,x;
    mpf_init_set(ps,psa);
    mpf_init_set(x,xa);
    if(mpf_cmp_ui(x,100) > 0) mpf_set_ui(x,100);
    mpz_t y;
    mpf_t tempf,tempf2;
    mpz_init(y);
    mpf_init(tempf);
    mpf_init(tempf2);

    mpz_set_ui( y, time(NULL)-t );
    mpf_ui_sub(tempf,100,x);
    mpf_sub(tempf2,x,ps);
    if(mpf_cmp(x,ps)==0)
    {
        mpf_set_ui(tempf,0);
    }else{
        mpf_div(tempf,tempf,tempf2);
    }
    mpf_set_z(tempf2,y);
    mpf_mul(tempf,tempf,tempf2);
    mpz_set_f(y,tempf);
    DString tempstr;
    tempstr = timeformat(y);
    mpz_clear(y);
    mpf_clear(tempf);
    mpf_clear(tempf2);
    mpf_clear(ps);
    mpf_clear(x);
    return tempstr;
}

DString totaltime(const time_t & t,const mpf_t & psa,const mpf_t & xa)//Start Time, Start %, Current %
{
    mpf_t ps,x;
    mpf_init_set(ps,psa);
    mpf_init_set(x,xa);
    if(mpf_cmp_ui(x,100) > 0) mpf_set_ui(x,100);
    mpz_t y;
    mpf_t tempf,tempf2;
    mpz_init(y);
    mpf_init(tempf);
    mpf_init(tempf2);

    mpz_set_ui( y, time(NULL)-t );
    mpf_ui_sub(tempf,100,ps);
    mpf_sub(tempf2,x,ps);
    if(mpf_cmp(x,ps)==0)
    {
        mpf_set_ui(tempf,0);
    }else{
        mpf_div(tempf,tempf,tempf2);
    }
    mpf_set_z(tempf2,y);
    mpf_mul(tempf,tempf,tempf2);
    mpz_set_f(y,tempf);
    DString tempstr;
    tempstr = timeformat(y);
    mpz_clear(y);
    mpf_clear(tempf);
    mpf_clear(tempf2);
    mpf_clear(ps);
    mpf_clear(x);
    return tempstr;
}

void DoCalc(mpz_t const & counter,mpz_t & sum)
{
  mpz_t const & n = counter;
  
  mpz_t av,a,vmax,N,N3,num,den,k,kq1,kq2,kq3,kq4,t,v,s,i,t1,temp,temp2;
  mpf_t ps,tempf,tempf2;
  time_t time_start, time_resume;

  mpz_init(av);
  mpz_init(vmax);
  mpz_init(num);
  mpz_init(den);
  mpz_init(k);
  mpz_init(kq1);
  mpz_init(kq2);
  mpz_init(kq3);
  mpz_init(kq4);
  mpz_init(t);
  mpz_init(v);
  mpz_init(s);
  mpz_init(i);
  mpz_init(t1);
  mpz_init(temp);
  mpz_init(temp2);
  mpf_init(ps);
  mpf_init(tempf);
  mpf_init(tempf2);
  
  
  Stat.Update(6,"Loading Vars...");
    
  mpz_init(N);
  mpz_add_ui(N,n,30);//Increase this number should increase precision
  mpf_set_d(tempf,log(10)/log(13.5));
  mpf_set_z(tempf2,N);
  mpf_mul(tempf,tempf,tempf2);
  mpz_set_f(N,tempf);
  
  mpz_init(N3);
  mpz_mul_ui (N3, N, 3);
  mpz_set_ui(sum,0);
  mpz_init_set_ui(a,2);

  //LOAD RESUME DATA
	{
		std::fstream resume;
		resume.open ("Resume.cfg", std::fstream::in | std::ios::out);
		if(resume.is_open())
		{
			double tempd;
			resume >> tempd;
			mpz_set_d(temp,tempd);
  			if(mpz_cmp(temp,n) == 0 && !(resume.eof()))
  			{
      			resume >> tempd;
      			mpz_set_d(a,tempd);
      			char counterstr[255];
      			resume >> counterstr;
      			mpz_set_str(sum,counterstr,10);
				next_prime(a,a,N3);
  			}  
  			else
  			{
      			resume.clear();
      			resume << "0" << std::endl;
      			resume << "0" << std::endl;
      			resume << "0";
  			}
  			resume.close();
		}
		else
		{
			doExit = false;
			resume.open ("Resume.cfg",std::ios::out);
			resume << "";
			resume.close();
			MessageBox(NULL,"Background Pi CLIENT Was Unable To Open Resume.cfg.\n\nBackground Pi Will Try Again.","ERROR: Main.DLL",MB_OK);
            PostMessage(Inviswnd,WM_RQUIT,0,0);
            ExitThread(0);
		}       
	}
  percentc(ps,a,N3);
	time(&time_start);
	time(&time_resume);
	timeb Start;//TIME TAKEN
	timeb Now;//TIME TAKEN
	ftime(&Start);//TIME TAKEN
	Stat.Update(6,"Calculating...");
	
	{
      	char statstr[255];
    	PrimeCount(tempf,N3);
    	mpz_set_f(temp,tempf);
    	mpz_get_str(statstr,10,temp);
    	Stat.Update(1,statstr);
	}
	
  for(;mpz_cmp(a,N3) <= 0;next_prime(a,a,N3))
  {
    
    //Update Status
    
    percentc(tempf,a,N3);
    Stat.Update(5,ftods(mpf_get_d(tempf),3) + "%");
    
    mpz_set_ui(temp,time(NULL) - time_start);
    Stat.Update(4,timeformat(temp));
    
    Stat.Update(3,timeleft(time_start,ps,tempf));
    
    Stat.Update(7,totaltime(time_start,ps,tempf));
    
    ftime(&Now);//TIME TAKEN
    {
		long long time = (Now.time*1000+Now.millitm)-(Start.time*1000+Start.millitm);
    	Stat.Update(8,(DString)time + (DString)" ms.");
	}
    ftime(&Start);//TIME TAKEN
    
	{
		char statstr[255];
		PrimeCount(tempf,a);
    	mpz_set_f(temp,tempf);
    	mpz_get_str(statstr,10,temp);
    	Stat.Update(2,statstr);
	}
	//Done
    
    mpf_set_d(tempf, log( mpz_get_d(N3) ) );
    mpf_set_d(tempf2, log( mpz_get_d(a) ) );
    mpf_div(tempf,tempf,tempf2);
    mpz_set_f(vmax,tempf);
    if (mpz_cmp_ui(a,2) == 0 )
    {
        mpz_add(vmax,vmax,N);
        mpz_sub(vmax,vmax,n);
        if (mpz_cmp_ui(vmax,0) <= 0) continue;
    }
    mpz_set_ui(av,1);
    for( mpz_set_ui(i,0) ; mpz_cmp(i,vmax) < 0 ; mpz_add_ui(i,i,1) )
        mpz_mul(av,av,a);
    mpz_set_ui(s,0);
    mpz_set_ui(den,1);
    mpz_set_ui(kq1,0);
    mpz_set_si(kq2,-1);
    mpz_set_si(kq3,-3);
    mpz_set_si(kq4,-2);

    if (mpz_cmp_ui(a,2) == 0) {
        mpz_set_ui(num,1);
        mpz_neg(v,n);
    } else {
        mpz_set_ui(temp,2);
        mpz_powm(num,temp,n,av);
        mpz_set_ui(v,0);
    }
    
    /*Beginning of slow code...*/
    for(mpz_set_ui(k,1) ; mpz_cmp(k,N) <= 0 ; mpz_add_ui(k,k,1))
    {
		
        mpz_mul_ui(temp,k,2);
        mpz_set(t,temp);
        DIVN(t,a,v,-1,kq1,2);

        mpz_mul(num,num,t);
        mpz_mod(num,num,av);
      
        mpz_sub_ui(t,temp,1);
        DIVN(t,a,v,-1,kq2,2);
        mpz_mul(num,num,t);
        mpz_mod(num,num,av);

        mpz_mul_ui(t,k,9);
        mpz_sub_ui(t,t,3);
        
        DIVN(t,a,v,1,kq3,9);
        mpz_mul(den,den,t);
        mpz_mod(den,den,av);

        mpz_mul_ui(t,k,3);
        mpz_sub_ui(t,t,2);
        DIVN(t,a,v,1,kq4,3);

        if (mpz_cmp_ui(a,2) != 0)
            mpz_mul_ui(t,t,2);
        else
            mpz_add_ui(v,v,1);
        mpz_mul(den,den,t);
        mpz_mod(den,den,av);
        
        if (mpz_cmp_ui(v,0) > 0)
        {
            mpz_invert(t,den,av);
	        mpz_mul(t,t,num);
            mpz_mod(t,t,av);

	        for(mpz_set(i,v) ; mpz_cmp(i,vmax) < 0 ; mpz_add_ui(i,i,1) )
	        {
                mpz_mul(t,t,a);
                mpz_mod(t,t,av);
            }
            
	        mpz_mul_ui(t1,k,25);
	        mpz_sub_ui(t1,t1,3);
            mpz_mul(t,t,t1);
            mpz_mod(t,t,av);

            mpz_add(s,s,t);
	        if (mpz_cmp(s,av) >= 0) 
                mpz_sub(s,s,av);
        
		}
    
	}
	/*...End of slow code*/

    mpz_set_ui(temp,5);
    //mpz_set(temp2,n);
    mpz_sub_ui(temp2,n,1);
    mpz_powm(t,temp,temp2,av);

    mpz_mul(s,s,t);

    mpz_mod(s,s,av);
    
    mpz_ui_pow_ui(temp,10,SUM_PREC);
    mpz_mul(temp,temp,s);
    mpz_tdiv_q(temp,temp,av);
    mpz_add(sum,sum,temp);
    
    //RESUME SAVE
    if(time(NULL)-time_resume >= Conf.Resume && Conf.Resume != 0)
    {
      std::fstream resume; 
      resume.open ("Resume.cfg", std::fstream::out);
      resume << mpz_get_d(n) << std::endl;
      resume << mpz_get_d(a) << std::endl;
      resume.precision(64);
      char counterstr[255];
      mpz_get_str(counterstr,10,sum);
      resume << counterstr;
      //MessageBox(NULL,counterstr,"Sum",MB_OK);
      resume.close();
      
      time(&time_resume);
    }    
    //END RESUME SAVE
  }// End Master For Loop
	  
  //mpz_t av,a,vmax,N,N3,num,den,k,kq1,kq2,kq3,kq4,t,v,s,i,t1,temp,temp2;
  mpz_clear(av);
  mpz_clear(a);
  mpz_clear(vmax);
  mpz_clear(N);
  mpz_clear(N3);
  mpz_clear(num);
  mpz_clear(den);
  mpz_clear(k);
  mpz_clear(kq1);
  mpz_clear(kq2);
  mpz_clear(kq3);
  mpz_clear(kq4);
  mpz_clear(t);
  mpz_clear(v);
  mpz_clear(s);
  mpz_clear(i);
  mpz_clear(t1);
  mpz_clear(temp);
  mpz_clear(temp2);
  
  //mpf_t ps,tempf,tempf2;
  mpf_clear(ps);
  mpf_clear(tempf);
  mpf_clear(tempf2);
}

bool DoComm(mpz_t & counter,mpz_t const & sum)
{
	// SET VAR
    int test = 1;
    time_t timeout_start;
    mpf_t tempf;
    mpz_t temp;
    mpz_init(temp);
    mpf_init(tempf);
    
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
    {
        //printf("Error at WSAStartup()\n");
        MessageBox(NULL,"Background Pi CLIENT Encountered An Error Loading Windows Sockets.\nTry Restarting Your Computer.\nBackground Pi Will Now Terminate.","Winsock",MB_OK);
        PostMessage(Inviswnd,WM_RQUIT,0,0);
        ExitThread(0);
        return true;
    }    

    // Create a socket.
    SOCKET m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( m_socket == INVALID_SOCKET )
    {
        MessageBox(NULL,"Background Pi CLIENT Encountered An Error Loading The Socket.\nTry Restarting Your Computer.\nBackground Pi Will Now Terminate.\n\nCODE: " + (DString)WSAGetLastError(),"Winsock",MB_OK);
        PostMessage(Inviswnd,WM_RQUIT,0,0);
        WSACleanup();
        ExitThread(0);
        return true;
    }

    // Connect to a server.
    sockaddr_in clientService;
    memset(&clientService,'\0',sizeof(clientService)); 
    clientService.sin_family = AF_INET;
    
    struct hostent *phostent;
    if ((phostent = gethostbyname(Conf.Host)) != NULL)
    {
        clientService.sin_addr.s_addr = *(unsigned long*)phostent->h_addr_list[0];
    }    
    else
    {
        clientService.sin_addr.s_addr = inet_addr(Conf.Host);// 32-bit network address
    }
       
    //clientService.sin_addr.s_addr = inet_addr(urladdr);
    clientService.sin_port = htons( 31415 );

    if ( connect( m_socket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR)
    {
         WSACleanup();
        //UpdateMode(MD_ERROR, true);
        //PRGMODE *= MD_ERROR;
        
        //Experamental Custom Sleep Logic
        time_t sleep_start;
        time(&sleep_start);
        while(((int)time(NULL)-sleep_start) < (Conf.Timeout))
        {
            //Sit for a while
            
            Stat.Update(6,"Will Retry Soon...");
            Stat.Update(5,ftods((((float)time(NULL)-(float)sleep_start)/(float)Conf.Timeout*100),2) + "%");
            //char counterstr[255];
            //mpz_get_str(counterstr,10,counter);
            //status[8] = counterstr;
            Sleep(500);
        }    
        
        //UpdateMode(MD_ERROR, false);
        //PRGMODE /= MD_ERROR;    
    }
    else
    {

        //bpcstatus.SetVal("Communicating...",0);
        Stat.Update(6,"Communicating...");
        //char counterstr[255];
        //mpz_get_str(counterstr,10,counter);
        //status[8] = counterstr;
        
        // Send and receive data.
        int bytesSent;
        int bytesRecv = SOCKET_ERROR;
        char recvbuf[32] = "";

		DString buffer;
        buffer = PI_COM_VERSION;
        bytesSent = send( m_socket, buffer, strlen(buffer), 0 );//IGNORED
        strcpy(recvbuf,"");
        bytesRecv = recv( m_socket, recvbuf, 32, 0 );
        if(bytesRecv != SOCKET_ERROR)
        {
            if(strcmp(recvbuf,"003") == 0)
            {
                memset(recvbuf,'\0',strlen(recvbuf));
                bytesRecv = recv( m_socket, recvbuf, 32, 0 );
                if(bytesRecv != SOCKET_ERROR)
                {
                     bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                     closesocket(m_socket);
                     buffer = (DString)"Your Background Pi Communications Version Differs From Servers\nNote: This is not the Background Pi Version\n\nYour Version: " + (DString)PI_COM_VERSION + (DString)"\nServer Version: " + recvbuf;
                     MessageBox(NULL,buffer,"Version Conflict",MB_OK); 
                     PostMessage(Inviswnd,WM_RQUIT,0,0);
                     ExitThread(0);
                }
            }
            else if (strcmp(recvbuf,"001") == 0)
            {
                //TrayMessage(hGlobalwnd, NIM_MODIFY, hGlobalInstance, "Sending 9 Digits...");
                //Extract the 9 digits from a really long int!
                char counterstr[255];
                mpz_get_str(counterstr,10,sum);
                DString DTemp;
                DTemp = counterstr;
                while(!DTemp.Isend())
                {
                    DTemp.Nextchar();
                }
                for(unsigned int i = 0; i < (SUM_PREC - 10);i++)
                {
                    DTemp.Prevchar();
                }
                DTemp = DTemp.Getsbc();
                while(!DTemp.Isend())
                {
                    DTemp.Nextchar();
                }
                for(unsigned int i = 0; i < 9;i++)
                {
                    DTemp.Prevchar();
                }
                DTemp = DTemp.Getspc();
                //Got the Digits
                bytesSent = send( m_socket, DTemp, strlen(DTemp), 0 );//9 #
                memset(recvbuf,'\0',strlen(recvbuf));
                bytesRecv = recv( m_socket, recvbuf, 32, 0 );
                if(bytesRecv != SOCKET_ERROR)
                {
                    if(strcmp(recvbuf,"003") == 0)
                    {
                        //Exit Abnormal
                        bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                        closesocket(m_socket);
                        //End Abnormal Exit Sequence
                    } 
                    else if (strcmp(recvbuf,"001") == 0)
                    {
                        //TrayMessage(hGlobalwnd, NIM_MODIFY, hGlobalInstance, "Sending Pi Range...");
                        //sprintf(buffer,"%li",counter);
                        char counterstr[255];
                        mpz_get_str(counterstr,10,counter);
                        buffer = counterstr;
                        bytesSent = send( m_socket, buffer, strlen(buffer), 0 );//I
                        memset(recvbuf,'\0',strlen(recvbuf));
                        bytesRecv = recv( m_socket, recvbuf, 32, 0 );
                        if(bytesRecv != SOCKET_ERROR)
                        {
                            if(strcmp(recvbuf,"003") == 0)
                            {
                                //Exit Abnormal
                                bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                                closesocket(m_socket);
                                //End Abnormal Exit Sequence
                            }
                            else if (strcmp(recvbuf,"001") == 0)
                            {
                                bytesSent = send( m_socket, "001", strlen("001"), 0 );//Abnormal
                                memset(recvbuf,'\0',strlen(recvbuf));
                                bytesRecv = recv( m_socket, recvbuf, 32, 0 );
                                if(bytesRecv != SOCKET_ERROR)
                                {
                                    if(strcmp(recvbuf,"003") == 0)
                                    {
                                        //Exit Abnormal
                                        bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                                        closesocket(m_socket);
                                        //End Abnormal Exit Sequence
                                    }
                                    //TrayMessage(hGlobalwnd, NIM_MODIFY, hGlobalInstance, "Getting Next Pi Range...");
                                    buffer = recvbuf;
                                    //MessageBox(hhwnd,buffer,"Comm",MB_OK);
                                    bytesSent = send( m_socket, "001", strlen("001"),0 );//System Ready
                                    memset(recvbuf,'\0',strlen(recvbuf));
                                    bytesRecv = recv( m_socket, recvbuf, 32, 0 );
                                    if(bytesRecv != SOCKET_ERROR)
                                    {
                                        //MessageBox(hhwnd,recvbuf,"Comm",MB_OK);
                                        if(strcmp(recvbuf,"003") == 0)
                                        {
                                            //Exit Abnormal
                                            bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                                            closesocket(m_socket);
                                            //End Abnormal Exit Sequence
                                        }
                                        else if(strcmp(recvbuf,"002") == 0)
                                        {
                                            //TrayMessage(hGlobalwnd, NIM_MODIFY, hGlobalInstance, "Setting Pi Range & Disconnecting...");
                                            mpz_set_str(counter,(const char *)buffer,10);
                                            closesocket(m_socket);
                                            mpf_clear(tempf);
    										mpz_clear(temp);
    										return false;
                                            
                                        }
                                        else
                                        {
                                            bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                                            closesocket(m_socket);  
                                        }
                                    }
                                }
                                else
                                {
                                    closesocket(m_socket);
                                    mpf_clear(tempf);
    								mpz_clear(temp);
    								//return false;
                                }
                            }
                            else
                            {
                                bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                                closesocket(m_socket);  
                            }
                        }
                        else
                        {
                            closesocket(m_socket);
                            mpf_clear(tempf);
    						mpz_clear(temp);
    						//return false;
                        }           
                    }
                    else
                    {
                        bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                        closesocket(m_socket);
                    }
                }
                else
                {
                    closesocket(m_socket);
                    mpf_clear(tempf);
    				mpz_clear(temp);
    				//return false;
                }
            }
            else
            {
                bytesSent = send( m_socket, "003", strlen("003"), 0 );//Abnormal
                closesocket(m_socket);
            }
        }
        else
        {
            closesocket(m_socket);
            mpf_clear(tempf);
    		mpz_clear(temp);
    		//return false;
        }
    }  
    closesocket(m_socket);    
    mpf_clear(tempf);
    mpz_clear(temp);
    
    //Status Update
	return true;
}

#ifdef STATS_VERSION
DWORD WINAPI StatsListen(void*)
{
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if( iResult != NO_ERROR )
    {
        MessageBox(NULL,"Failed to initiate \"Windows Sockets 2\".","Winsock Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
        return 0;
    }
     
    // Create a socket.
    SOCKET m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( m_socket == INVALID_SOCKET )
    {
        WSACleanup();
        MessageBox(NULL,"Failed to initiate socket.","Winsock Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
        return 0;
    }
    
    // Bind the socket.
    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( "0.0.0.0" );
    service.sin_port = htons( 31416 );
    
     if ( bind( m_socket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
     {
          WSACleanup();
         //printf( "bind() failed.\n" );
         closesocket(m_socket);
         MessageBox(NULL,"Failed to bind socket.\nPort may already be in use.","Winsock Error",MB_OK | MB_ICONERROR);
         
         
         ///
         //SHUTDOWN PROGRAM!
         ///
         return 0;
    }
    
   
    
    // Listen on the socket.
    if ( listen( m_socket, 10 ) == SOCKET_ERROR )
    {
         WSACleanup();
         //printf( "Error listening on socket.\n");
         MessageBox(NULL,"Failed to listen on socket.","Winsock Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
              
         ///
         //SHUTDOWN PROGRAM!
         ///
         return 0;
    }
    
    SOCKET AcceptSocket;
    sockaddr_in clientaddr;
    
    while (1)
    {          
        AcceptSocket = (unsigned int)SOCKET_ERROR;
        while ( AcceptSocket == SOCKET_ERROR )
        {
            int size = sizeof(clientaddr);
            AcceptSocket = accept( m_socket,(struct sockaddr*)&clientaddr,&size);            
        }
          
        // Send and receive data
        int bytesSent;
        DString buffer;

        for(int i = 0;i<=8;i++)
        {
            buffer += Stat.Stats[i] + "\r\n";
        }
        
        bytesSent = send( AcceptSocket, buffer, strlen(buffer), 0 );//System Ready
        closesocket(AcceptSocket);

    }
}
#endif

DWORD WINAPI MainThread(void*)
{
	#ifdef STATS_VERSION
		statthread = CreateThread(NULL,0,StatsListen,NULL,0,NULL);
    #endif
	Sleep(50);
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Loading Settings...");
	Stat.Update(6,"Loading Settings...");
	if(!Conf.Open())
	{
		Stat.Update(6,"Saving Settings...");
		if(!Conf.Save())
		{
			MessageBox(NULL,"Failed to load Config.cfg","Error: Main.DLL",MB_OK);
			PostMessage(Inviswnd,WM_RQUIT,0,0);
			ExitThread(0);
		}
	}
	
	Stat.Update(6,"Preparing Data...");
	PostMessage(Splashwnd,WM_SETLOADTEXT,0,(LPARAM)"Preparing Data...");
	mpz_t counter;
	mpz_t sum;
	mpz_init(counter);
	mpz_init(sum);
	{
		std::fstream hFile;
		hFile.open("Current.cfg", std::fstream::in);
    	if( !( hFile.is_open() ) )
    	{
        	mpz_set_ui(counter,10);
    	}
    	else
    	{
        	double tempd;
        	hFile >> tempd;
        	mpz_set_d(counter,tempd);
        	hFile.close();
    	}
	}
	
	ShowWindow (Splashwnd, SW_HIDE);
    
    while(true)
    {
		PostMessage(Splashwnd,WM_CLOSE,0,0);
    	
		//Make it divis by 9
		mpz_tdiv_q_ui(counter,counter,9);
        mpz_mul_ui(counter,counter,9);
        
        //Slight Increase in number
        if (counter <= 0)
        {
            mpz_set_ui(counter,1);
        }
        if (mpz_cmp_ui(counter,1) > 0)
        {
            mpz_add_ui(counter,counter,1);
        }
        
        //Set Thread Priority For Calc
        SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_IDLE);
        
        Stat.Reset();
        Stat.Update(6,"Preparing Calc...");
        
		{
			char counterstr[255];
        	mpz_get_str(counterstr,10,counter);
        	Stat.Update(0,counterstr);
		}
		
        DoCalc(counter,sum);
        
        Stat.Reset();
        Stat.Update(6,"Preparing Comm...");
        
        {
			char counterstr[255];
        	mpz_get_str(counterstr,10,counter);
        	Stat.Update(0,counterstr);
		}
        
        SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL);
        
        while(DoComm(counter,sum))
        {
			Sleep(1);
		}
        
        Stat.Reset();
        Stat.Update(6,"Saving Range...");
        
        {
			char counterstr[255];
        	mpz_get_str(counterstr,10,counter);
        	Stat.Update(0,counterstr);
		}
        
        std::fstream hFile;
		hFile.open("Current.cfg", std::fstream::out);
    	if( !( hFile.is_open() ) )
    	{
        	MessageBox(NULL,"Background Pi CLIENT Was Unable To Open The Current.cfg File After Connecting To The Server.\n\nBackground Pi Will Now Terminate.","ERROR: Main.DLL",MB_OK);
            mpz_clear(counter);
            mpz_clear(sum);
			PostMessage(Inviswnd,WM_RQUIT,0,0);
            ExitThread(0);
    	}
    	else
    	{
            hFile << mpz_get_d(counter) << std::endl;
            hFile.close();
    	}
    	
    	Stat.Update(6,"End Loop...");
        
		//break;
	}
	mpz_clear(counter);
	mpz_clear(sum);
	//PostMessage(Inviswnd,WM_RQUIT,0,0);
}

bool APIENTRY Main(int nFunsterStil,HINSTANCE instance)
{
	thisinstance = instance;
	
	Inviswnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_INVIS),NULL,(DLGPROC)InvisDialogProcedure);
	//ShowWindow (Inviswnd, SW_SHOW);
	
	TrayIcon hIcon(thisinstance);
	hGlobalIcon = &hIcon;
	
	if(nFunsterStil != 7)
    {
		Splashwnd = CreateDialog(thisinstance,MAKEINTRESOURCE(IDD_SPLASH),Inviswnd,(DLGPROC)SplashDialogProcedure);
		ShowWindow (Splashwnd, SW_SHOW);
    }
	
	thread = CreateThread(NULL,0,MainThread,NULL,0,NULL); //Returns handle to thread, may be useful...
	
	MSG messages;
	// Run the message loop. It will run until GetMessage() returns 0 
    while (GetMessage (&messages, NULL, 0, 0) > 0)
    {
        if(!IsDialogMessage(Inviswnd, &messages) && !IsDialogMessage(Splashwnd, &messages) && !IsDialogMessage(Stat.Statuswnd, &messages))
        {
            // Translate virtual-key messages into character messages 
            TranslateMessage(&messages);
            // Send message to WindowProcedure 
            DispatchMessage(&messages);
        }
    }
    DestroyWindow(Inviswnd);
    Inviswnd=NULL;
    DestroyWindow(Splashwnd);
    Splashwnd=NULL;
    Sleep(50);
    CloseHandle(thread);
    #ifdef STATS_VERSION
    	CloseHandle(statthread);
    #endif
	return doExit;
}
