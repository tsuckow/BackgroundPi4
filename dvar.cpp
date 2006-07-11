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

bool DString::Isend() const
{
	if(*charpos == 0)
	return true;
	//charpos++;
	if(*(charpos+1) == 0)
	{
		//charpos--;
		return true;
	}
	//charpos--;
	return false;
}

DString DString::Getspc() const
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

DString DString::Getsbc() const
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

DString operator+= (DString& StringA,DString const & StringB)
{
    char tempchar[ (strlen(StringA.Getval()) + strlen(StringB.Getval()) + 1) ];
    strcpy( tempchar,StringA.Getval() );
    strcat( tempchar,StringB.Getval() );    
    StringA.Setval(tempchar);
    return StringA;
}

bool operator== (DString const & StringA,const DString & StringB)
{
    return strcmp (StringA,StringB) == 0;
}

bool operator== (DString const & StringA,const char * StringB)
{
    return StringA == (DString)StringB;
}

bool operator== (const char * StringA,DString const & StringB)
{
    return (DString)StringA == StringB;
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
