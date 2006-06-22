/////////////////////////////////////////
//                                     //
//  DString.h -- Defcon String Header  //
//                                     //
//  A Cheesey Attempt To Make C++ Easy //
//                                     //
//  MMMMM... Cheese...                 //
//                                     //
// Version 0.9                         //
// DString Version: 1.9 Beta           //
// DMath Version: 0.2 Work In Progress //
/////////////////////////////////////////

#ifndef _dvar_h_
#define _dvar_h_

#include <string>

//
// DSTRING
//
class DString
{
    public:
        //Constructors
        DString() { Init(); Setval(""); }
        
        //Conversions <-
        DString(const short num) { Init(); sintconstruct(num); }
        DString(const unsigned short num) { uintconstruct(num); }
        DString(const int num) { Init(); sintconstruct(num); }
        DString(const unsigned int num) { Init(); uintconstruct(num); }
        DString(const long num) { Init(); sintconstruct(num); }
        DString(const unsigned long num) { Init(); uintconstruct(num); }
        DString(const long long num) { Init(); sintconstruct(num); }
        
        DString(const double);
        
        DString(const DString& temp) { Init(); Setval( temp.Getval() ); }
        DString(const char* temp) { Init(); Setval( temp ); }
        DString(const char temp)
        {
            Init();
            delete [] mystring;
            mystring = NULL;
            mystring = new char[2];
            mystring[1] = 0;
            mystring[0] = temp;
        }
                
        //Conversions ->
        operator const char* () { return mystring; }
        
        //Operators
        DString operator=(const DString);
        
        //IO
        const char* Getval() const { return mystring; }
        void Setval(const char* temp)
        {
            delete [] mystring;
            mystring = NULL;
            mystring = new char[ strlen(temp)+1 ];
            strcpy (mystring,temp); 
            charpos = mystring;
            //std::cout << "<New String>: " << mystring << std::endl;
        }
        void Nextchar();
        void Prevchar() { if(charpos!=mystring){ charpos--; } }
    	char Getchar() { return *charpos; }
    	void Setchar(const char temp) { if(*charpos!=0 && temp != 0){ *charpos = temp; } }
    	bool Isend();
    	bool Isbeg() { return (charpos == mystring); }
    	void Beg() { charpos = mystring; }
    	DString Getspc();
    	DString Getsbc();
    	
        //Unset
        void Destroy()
        { 
             delete [] mystring;
             mystring= NULL;
             charpos = NULL;
        }
        
        //Deconstructer
        ~DString();
    private:
        
        void Init() {mystring=NULL; charpos=NULL;}
        
        /* Integer Convertion Templates */
        template<class T>
        void sintconstruct(T);
        
        template<class T>
        void uintconstruct(T);
        
        char * mystring;
        char * charpos;
};

//
/* EXTERNAL DSTRING FUNCTIONS */
//
DString operator+ (const DString,const DString);
DString operator+= (DString&,const DString);
DString ftods (const double, const int);
//
//
//

//End Dvar
#endif
//
