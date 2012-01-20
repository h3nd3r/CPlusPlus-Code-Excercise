// String.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>

using namespace std;
	
int isnewline(char c)
{
	if('\n' == c)
		return 1;

	return 0;
}

unsigned int strlength(const char *pstr)
{
	unsigned int length = 0;

	while(0 != pstr[length])
		length++;

	return length;
}

char * strcopy(char *dest, const char *src)
{
	char *save = dest;
	while(*dest++ = *src++);
	return save;
}

class String
{
	public:
		String();
		String(char ch);
		String(const char *pstr);

		const String & operator+=(const String &rhs);
		
		int Compare(String myString, String &myNewString);
		void InvertConcat(String myString, String &myNewString);
		void Merge(String myString, String &myNewString);
		void InvertMerge(String myString, String &myNewString);
		int  Length();
		const char * C_str() const;
		
	private:
		char     *cstring;
		unsigned  length;
		unsigned  bufferLength;
};

String::String()
{
	cstring = NULL;
	length = 0;
	bufferLength = 0;
}

String::String(char ch)
{
	length = 1;
	bufferLength = length + 1;
	cstring = new char[bufferLength];
	cstring[0] = ch;
	cstring[1] = 0;
}

const char * String::C_str() const
{
	return cstring;
}

int String::Length()
{
	return length;
}

const String & String::operator+=(const String &rhs)
{
	if(this == &rhs)
	{
		String copy(rhs);
		return *this += copy;
	}

	int newLength = Length() + rhs.length;
	if(newLength >= (int)bufferLength)
	{
		bufferLength = 2 * (newLength + 1);

		char *oldBuffer = cstring;
		cstring = new char[bufferLength];
		strcopy(cstring, oldBuffer);
		delete [ ] oldBuffer;
	}

	strcopy(cstring + Length(), rhs.cstring);
	length = newLength;

	return *this;
}

istream & operator>>(istream &in, String &str)
{
	char ch;
	str = "";
	in >> ch;

	if(!in.fail())
	{
		do
		{
			str += ch;
			in.get(ch);
		} while(!in.fail() && !isnewline(ch));
	}

	return in;
}

ostream & operator<<(ostream & out, const String & str)
{
	return out << str.C_str();
}

String::String(const char *pstr)
{
	length = strlength(pstr);
	bufferLength = length + 1;
	
	cstring	= new char[bufferLength];
	
	strcopy(cstring, pstr);
}

int String::Compare(String myString, String &myNewString)
{
	char *s1 = this->cstring;
	char *s2 = myString.cstring;
	
	// while the each character of the string is the same
	for(; *s1 == *s2; ++s1, ++s2)
	{
		// the strings are equal
		if(*s1 == 0)
		{
			// display and set the return string
			this->Merge(myString, myNewString);
			return 0;
		}
	}

	int	compare = *(unsigned char *)s1 - *(unsigned char *)s2;

	if(compare < 0)
		this->InvertConcat(myString, myNewString);
	else
		this->InvertMerge(myString, myNewString);

	return compare;	
}

/*
** return a string made up of the two strings inverted and then concatenated,
** e.g. abcdef and uvwxyz would be fedcbazyxwvu
*/
void String::InvertConcat(String myString, String &myNewString)
{
	int length = this->length + myString.length;
	char *pstrNew = (char *)calloc(length + 1, sizeof(char));
	myNewString.cstring = pstrNew;

	for(int i=this->length-1; i>=0; i--)
	{
		*pstrNew =  *(this->cstring + i);
		pstrNew++;
	}

	for(int i=myString.length-1; i>=0; i--)
	{
		*pstrNew =  *(myString.cstring + i);
		pstrNew++;
	}
	
	cout << "New inverted concatenated string: " << myNewString.cstring << endl;
}

/*
** return a string made up of the two strings merged 
** e.g. abcdef and abcdef would be aabbccddeeff
*/
void String::Merge(String myString, String &myNewString)
{
	int length = this->length + myString.length;
	char *pstrNew = (char *)calloc(length + 1, sizeof(char));
	myNewString.cstring = pstrNew;

	for(unsigned int i=0; i<this->length; i++)
	{
		*pstrNew =  *(this->cstring + i);
		pstrNew+=2;
	}
	pstrNew = myNewString.cstring + 1;
	for(unsigned int i=0; i<myString.length; i++)
	{
		*pstrNew =  *(myString.cstring + i);
		pstrNew+=2;
	}

	cout << "New merged string: " << myNewString.cstring << endl;
}

/*
** return a string made up of the two strings inverted and merged 
** e.g. uvwxyz and abcdef would be zfyexdwcvbua
*/
void String::InvertMerge(String myString, String &myNewString)
{
	int i = this->length;
	int j = myString.length;
	int length = i + j;
	char *pstrNew = (char *)calloc(length + 1, sizeof(char));

	myNewString.cstring = pstrNew;
	i-=1;
	j-=1;

	for(int k=0; k<length; ++pstrNew, ++k)
	{
		if( ((k%2) && (i>=0)) || (j<0))
		{
			*pstrNew = *(this->cstring + i); 
			i--;
		}
		else if(j>=0)
		{
			*pstrNew = *(myString.cstring + j);
			j--;
		}
	}
	
	cout << "New inverted merged string: " << myNewString.cstring << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	String myFirstString, mySecondString, myNewString;

	cout << "Please enter your first string" << endl;
	cin >> myFirstString;

	cout << "Please enter your second string" << endl;
	cin >> mySecondString;

	int compare = myFirstString.Compare(mySecondString, myNewString);
	cout << "Result of comparison: " << compare << endl;
	return 0;
}

