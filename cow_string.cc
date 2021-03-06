 ///
 /// @file    cow_string.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-03-02 20:29:24
 ///

#include <stdio.h>
#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

class String
{
public:
	String();

	String(const char * pstr);

	String(const String & rhs);

	String & operator=(const String & rhs);

	~String();


	char & operator[](size_t idx);

	int refcount()
	{
		return *(int *)(_pstr - 4);

	}

	const char * c_str() const
	{
		return _pstr;
	}

	size_t size() const
	{
		return strlen(_pstr);
	}

private:
	void initRefcount()
	{
		*(int *)(_pstr - 4)=1;
	}


	void increaseRefcount()
	{
		++(*(int *)(_pstr - 4));
	}

	
	void decreaseRefcount()
	{
		--(*(int *)(_pstr - 4));
	}

	void release()
	{
		decreaseRefcount();
		if(refcount()==0)
		{
			delete [] (_pstr - 4);
			cout << "~String()" << endl;
		}

	}
	friend std::ostream & operator<<(std::ostream & os, const String & rhs);
private:
	char * _pstr;

};

String::String()
: _pstr(new char[5]()+4)
{
	initRefcount();
}

String::String(const char * pstr)
: _pstr(new char[strlen(pstr)+5]() + 4)
{
	strcpy(_pstr,pstr);
	initRefcount();
}

String::String(const String & rhs)
: _pstr(rhs._pstr)
{
	increaseRefcount();
}

String::~String()
{
	release();
}

String & String::operator=(const String & rhs)
{
	if(this != & rhs)
	{
		release();
		_pstr = rhs._pstr;
		increaseRefcount();
	}
	return *this;
}

char & String::operator[](size_t idx)
{
	static char nullchar = '\0';
	if(idx >= size())
	{
		return nullchar;
	}
	else
	{
		if(refcount() > 1)
		{
			decreaseRefcount();
			char * ptmp = new char[size() + 5]() + 4;
			strcpy(ptmp,_pstr);
			_pstr = ptmp;
			initRefcount();
		}
		return _pstr[idx];
	}
}

std::ostream & operator<<(std::ostream & os,const String & rhs)
{
	os << rhs._pstr;
	return os;
}

int test(void)
{
	String s1;
	cout << "s1,1 refcount = " << s1.refcount() << endl;

	String s2(s1);

	cout << "s1,1 refcount = " << s1.refcount() << endl;
	cout << "s2,1 refcount = " << s1.refcount() << endl;
	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;

}
int test2(void)
{
	String s1("hello,word");
	cout << "s1,1 refcount = " << s1.refcount() << endl;
	String s2(s1);

	cout << "s1,1 refcount = " << s1.refcount() << endl;
	cout << "s2,1 refcount = " << s2.refcount() << endl;
	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;
	printf("&s1 = %p\n",s1.c_str());
	printf("&s2 = %p\n",s2.c_str());
	cout << endl;
	
	String s3 = "shenzhen,wagdao";
	s2 = s3 ;

	cout << "s1,1 refcount = " << s1.refcount() << endl;
	cout << "s2,1 refcount = " << s2.refcount() << endl;
	cout << "s3,1 refcount = " << s3.refcount() << endl;
	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;
	cout << "s3 = " << s3 << endl;
	printf("&s1 = %p\n",s1.c_str());
	printf("&s2 = %p\n",s2.c_str());
	printf("&s3 = %p\n",s3.c_str());

	cout << endl << "xie" << endl;
	s3[0]='S';

	cout << "s2,1 refcount = " << s2.refcount() << endl;
	cout << "s3,1 refcount = " << s3.refcount() << endl;
	cout << "s2 = " << s2 << endl;
	cout << "s3 = " << s3 << endl;
	printf("&s2 = %p\n",s2.c_str());
	printf("&s3 = %p\n",s3.c_str());
}
int main(void)
{
	test2();
	return 0;
}
