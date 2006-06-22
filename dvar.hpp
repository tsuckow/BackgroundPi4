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

#define D1_long_long
//#define D1_debug

#ifndef dvar_h
#define dvar_h

#include <string>

#ifdef D1_debug
#include <iostream>
#include <iomanip>
#endif

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
        #ifdef D1_long_long
        DString(const long long num) { Init(); sintconstruct(num); }
        #endif
        
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



//
// DMATH
//
class DMath
{
    public:
		//Constructors
        DMath() { Setval(""); }
		
        //Convertions
        DMath (const DString);
        DMath (const DMath& temp) { Setval( temp.mystring ); }
        
        //IO
        const DString Getval() const { return mystring; }
                
        //Operators
        DMath operator=(const DMath);
        friend DMath operator+ (const DMath,const DMath);
        friend DMath operator- (const DMath,const DMath);
        friend bool operator> (const DMath,const DMath);
        friend bool operator< (const DMath,const DMath);
        friend bool operator>= (const DMath,const DMath);
        friend bool operator<= (const DMath,const DMath);
        friend bool operator== (const DMath,const DMath);
        friend bool operator!= (const DMath,const DMath);
        
        operator DString () { return mystring; }
        
        ~DMath();
    private:
        //IO
        friend void Doadd(DMath & StringA,DMath & StringB, unsigned char & carry, DString & C);
        void Setval( const DString temp ) { mystring = temp; }
        void Nextchar() { mystring.Nextchar(); }
        void Prevchar() { mystring.Prevchar(); }
    	char Getchar() { return mystring.Getchar(); }
    	void Setchar(const char temp) { mystring.Setchar(temp); }
    	bool Isend() { return mystring.Isend(); }
        bool Isbeg() { return mystring.Isbeg(); }
        void Beg() { mystring.Beg(); }
    	DString Getspc() { return mystring.Getspc(); }
    	DString Getsbc() { return mystring.Getsbc(); }
        
        //The var
        DString mystring;
};

///////////////////////////////////
//                               //
//                               //
//  BEGIN FUNCTION DEFINITIONS   //
//                               //
//                               //
///////////////////////////////////





//
//
/* BEGIN INTEGER CONVERTION TEMPLATES */
//
//

template<class T>
void DString::sintconstruct(T num)
{
    bool negative = false;
    T a;
    if(num == 0)
    {
        Setval("0");
        return;
    }
    else
    {
        Setval("");
    }
         
    if(num < 0)
    {
        negative = true;
        a = num * -1;
    }
    else
    {
        a = num;
    }
    
    char c [2];
    c[0] = 0;
    c[1] = 0;
    
    while(a > 0)
    {
        c[0] = a - ( (T(a/10))*10 ) + 48;
        Setval( DString(c) + Getval() );
        a /= 10;
    }
    if(negative)
    {
        Setval( DString("-") + Getval() );
    }
}

template<class T>
void DString::uintconstruct(T num)
{
    T a;
    
    if(num == 0)
    {
        Setval("0");
        return;
    }
    else
    {
        Setval("");
    }
    
    a = num;
    
    char c [2];
    c[0] = 0;
    c[1] = 0;
    
    while(a > 0)
    {
        c[0] = a - ( (T(a/10))*10 ) + 48;
        Setval( DString(c) + Getval() );
        a /= 10;
    }
}

//
//
/* END INTEGER CONVERTION TEMPLATES */
//
//





//
//
/* BEGIN DSTRING CONVERTIONS */
//
//
DString::DString(const double num)
{
    /*
    char t[18];
    sprintf( t, "%1.15lf" , num);
    Setval(t);
    //*/
    /*
    delete [] mystring;
    mystring = 0;
    mystring = new char[ strlen(t)+1 ];
    strcpy( mystring,t );*/
    
    ///*
    double num2 = 0;
    
    if(num < 0)
    {
        Setval("-");
        num2 = num * -1;
    }
    else
    {
        Setval("");
        num2 = num;
    }
    
    short pos = 0;
    
    #ifdef D1_debug
    std::cout << std::setprecision ( 308 );
    std::cout << num << std::endl;
    #endif
    
    #ifdef D1_debug
    std::cout << num2 << std::endl;
    #endif
    
    while(num2 > 10)
    {
        num2 /= 10;
        pos--;
        #ifdef D1_debug
        std::cout << num2 << std::endl;
        #endif
    }
    
    #ifdef D1_debug
    std::cout << num2 << std::endl;
    #endif
    
    char c [2];
    c[0] = 0;
    c[1] = 0;
    
    while(num2 != 0)
    {
        c[0] = short(num2);
        #ifdef D1_debug
        std::cout << char(c[0]+48) << " " << num2 << std::endl;
        #endif
        num2 -= c[0];
        c[0] += 48;
        Setval( Getval() + DString(c) );
        if(pos == 0)
        {
            Setval( Getval() + DString(".") );
        }
        num2 *= 10;
        pos++;
    }
    
    //*/
}

DMath::DMath(const DString temp)
{
    //
    //
    //
    //Add Verify
    //
    //
    //
    mystring.Setval( temp.Getval() );
}

//
//
/* END CONVERTIONS */
//
//





//
/* DECONSTRUCTER */
//
DString::~DString()
{
    delete [] mystring;
    mystring = NULL;
    charpos = NULL; //Dont delete it because it points inside mystring!
}



DMath::~DMath()
{
    mystring.Destroy();
}
//
//
//





//
//
/* BEGIN IO FUNCTIONS */
//
//
void DString::Nextchar()
{
	if(*charpos!=0)
	{
		charpos++;
		if(*charpos==0)
		{
			charpos--;
		}
	}
}

bool DString::Isend()
{
	if(*charpos == 0)
	return true;
	charpos++;
	if(*charpos == 0)
	{
		charpos--;
		return true;
	}
	charpos--;
	return false;
}

DString DString::Getspc()
{
	DString temp;
	if( !Isend() )
	{
		char * tc = charpos;
		char t[2];
		t[1] = 0;
		while( *tc != 0 )
		{
			tc++;
			t[0] = *tc;
			temp += t;
		}
		
	}
	return temp;
}

DString DString::Getsbc()
{
	DString temp;
	if( !Isbeg() )
	{
		char * tc = charpos;
		char t[2];
		t[1] = 0;
		while( tc != mystring )
		{
			tc--;
			t[0] = *tc;
			temp = (DString)t + temp;
		}
		
	}
	return temp;
}

//
//
/* END IO FUNCTIONS */
//
//

void Doadd(DMath & StringA,DMath & StringB, unsigned char & carry, DString & C)
{
		if(StringA.Getchar() != '.')
        {
            carry = StringA.Getchar() + StringB.Getchar() + carry - 48*2;
            if(carry >= 10)
            {
                C = (DString)(char)(carry - 10*(int(carry/10)) + 48) + C;
                carry = carry/10;
            }
            else
            {
                C = (DString)(char)(carry+48) + C;
                carry = 0;
            }
        }
        else
        {
            C = (DString)'.' + C;
        }
}


//
//
/* BEGIN OPERATOR OVERLOADING */
//
//
DString DString::operator= (const DString temp)
{
    if(this == &temp)
    return *this;
    Setval( temp.Getval() );
    return *this;
}

DMath DMath::operator= (const DMath temp)
{
    if(this == &temp)
    return *this;

    Setval( temp.Getval() );
    return *this;
}

DString operator+ (const DString StringA,const DString StringB)
{
    char tempchar[ (strlen(StringA.Getval()) + strlen(StringB.Getval()) + 1) ];
    strcpy( tempchar,StringA.Getval() );
    strcat( tempchar,StringB.Getval() );
    return DString(tempchar);
}

DMath operator+ (const DMath A,const DMath B)
{
	DString C;
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	unsigned char carry = 0;
	
	//Find Decimal then move together to mismatch
	while(StringA.Getchar() != '.' && !StringA.Isend())
	{
        StringA.Nextchar();
    }
    while(StringB.Getchar() != '.' && !StringB.Isend())
	{
        StringB.Nextchar();
    }
    
    //Handle Decimles
    if(StringA.Getchar() == '.')
    StringA.Prevchar();
    if(StringB.Getchar() == '.')
    StringB.Prevchar();
    
    while( !StringA.Isend() && !StringB.Isend() )
    {
		StringA.Nextchar();
		StringB.Nextchar();
	}
	
	//Get End Data For Add - (A+0)=A
	if( !StringA.Isend() )
	{
		C = StringA.Getspc();
	}
	else if( !StringB.Isend() )
	{
		C = StringB.Getspc();
	}
	
    //Until you reach front of number for either, add them
	while( !StringA.Isbeg() && !StringB.Isbeg() )
	{
        Doadd(StringA,StringB,carry,C);
        StringA.Prevchar();
        StringB.Prevchar();      
    }
    
    //Add the 2 begining #
    Doadd(StringA,StringB,carry,C);
    
    //Find which is not at begining and add to 0
    if(!StringA.Isbeg())
    {
		DMath DMzero = (DString)"0";
		while(!StringA.Isbeg())
		{
			StringA.Prevchar();
			Doadd(DMzero,StringA,carry,C);
		}
	}
    else if(!StringB.Isbeg())
    {
		DMath DMzero = (DString)"0";
		while(!StringB.Isbeg())
		{
			StringB.Prevchar();
			Doadd(DMzero,StringB,carry,C);
		}
	}
    
    return C;
}

DMath operator- (const DMath A,const DMath B)
{
	DString C;
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	unsigned char carry = 0;
	bool neg = false;
	
	
	
	return C;
}

bool operator> (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	return true;
}

bool operator< (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	return true;
}

bool operator>= (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	return true;
}

bool operator<= (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	return true;
}

bool operator== (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	
	while(!StringA.Isend() && !StringB.Isend())
	{
        if(StringA.Getchar() != StringB.Getchar())
        return false;
        StringA.Nextchar();
        StringB.Nextchar();
    }
    
    if(StringA.Getchar() != StringB.Getchar())
    return false;
	
	return true;
}

bool operator!= (const DMath A,const DMath B)
{
	DMath StringA = A, StringB = B;
	StringA.Beg();
	StringB.Beg();
	
	while(!StringA.Isend() && !StringB.Isend())
	{
        if(StringA.Getchar() != StringB.Getchar())
        return true;
        StringA.Nextchar();
        StringB.Nextchar();
    }
    
    if(StringA.Getchar() != StringB.Getchar())
    return true;
	
	return false;
}

DString operator+= (DString& StringA,const DString StringB)
{
    char tempchar[ (strlen(StringA.Getval()) + strlen(StringB.Getval()) + 1) ];
    strcpy( tempchar,StringA.Getval() );
    strcat( tempchar,StringB.Getval() );    
    StringA.Setval(tempchar);
    return StringA;
}
//
//
/* END OPERATOR OVERLOADING */
//
//





//
//Additional Functions to help
//
DString ftods (const double num,const int dec)
{
    char t[18];
    DString temp;
    temp = "%1." + (DString)dec + "lf";
    sprintf( t, temp, num);
    
    temp = t;
    return temp;
}
//
//
//

//End Dvar
#endif
//
