#include "dvar.hpp"

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
      
    while(num2 > 10)
    {
        num2 /= 10;
        pos--;
    }
    
    
    char c [2];
    c[0] = 0;
    c[1] = 0;
    
    while(num2 != 0)
    {
        c[0] = short(num2);
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

DString operator+ (const DString StringA,const DString StringB)
{
    char tempchar[ (strlen(StringA.Getval()) + strlen(StringB.Getval()) + 1) ];
    strcpy( tempchar,StringA.Getval() );
    strcat( tempchar,StringB.Getval() );
    return DString(tempchar);
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
