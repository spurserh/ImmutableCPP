
#include <iostream>
#include "ConsList.h"
#include <list>

using namespace std;
using namespace ImmutableCPP;

template<typename T>
void Print(ConsList<T> l)
{
	for(; !l.empty() ; l = l.rest())
	{
		cout<<l.first().c_str()<<' ';
	}
}

static size_t mynodes_constructed=0;
static size_t mynodes_destroyed=0;

class mynode
{public:

	mynode():val(0){++mynodes_constructed;}

	mynode(mynode const&o)
		: val(o.val)
	{
		++mynodes_constructed;
	}

	mynode(size_t val)
		: val(val)
	{
		++mynodes_constructed;
	}

	~mynode()
	{
		++mynodes_destroyed;
	}

	size_t val;
};

int main()
{
	ConsList<string> justWorld;

	{
		ConsList<string> foo;

		justWorld = foo = foo.cons("World");
		foo = foo.cons("Hello");

		cout<<"List: ";
		Print(foo);
		cout<<endl;
	}

	// Big list test
	cout<<"Starting big cons"<<endl;

	{
		{
			ConsList<mynode> big;

			for(size_t i=0;i<(1000*1000*2);++i)
				big = big.cons(i);

			cout<<"Big cons done"<<endl;
		}

		cout<<"Big cons destroyed"<<endl;

	}

	{
		{
			std::list<mynode> big_stl;

			for(size_t i=0;i<(1000*1000*2);++i)
				big_stl.push_front(i);

			cout<<"Big STL done"<<endl;
		}

		cout<<"Big STL destroyed"<<endl;
	}
	return 0;
}

