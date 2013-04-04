
#ifndef CONS_LIST_H
#define CONS_LIST_H

#include <cassert>

namespace ImmutableCPP
{


/*
	TODO: Thread safety (just need a thread safe integer)
*/
template<typename T>
class ConsList
{public:

	ConsList()
		: head(0)
	{
	}

	ConsList(ConsList const&o)
		: head(o.head ? o.head->reference() : 0)
	{
	}

	ConsList &operator=(ConsList const&o)
	{
		this->~ConsList();

		this->head = o.head;

		if(this->head)
			this->head->reference();

		return *this;
	}

	~ConsList()
	{
		// Non-recursive GC: we walk the list iteratively, dereferencing each node we arrive to. If the refcount goes to 0, then we continue. If not, we stop
		for(ConsListNode *node = head;
			node;)
		{
			ConsListNode *next = node->next;

			if(!node->dereference())
				break;

			node = next;
		}

		head = 0;
	}

	ConsList cons(T const&value)const
	{
		return ConsList(new ConsListNode(value, head));
	}

	ConsList rest()const
	{
		assert(head);
		return ConsList(head->next);
	}

	T const&first()const
	{
		assert(head);
		return head->value;
	}

	bool empty()const
	{
		return head == 0;
	}

	// TODO: Length

private:

	class ConsListNode
	{public:

		ConsListNode(T const&value, ConsListNode *next)
			: refs(0), value(value), next(next?next->reference():0)
		{
		}

		ConsListNode *reference()
		{
			++refs;
			return this;
		}

		/**
		  * @return Whether or not the node was destroyed
		  */
		bool dereference()
		{
			if((--refs) == 0)
			{
				delete this;
				return true;
			}

			return false;
		}

		const	T			 value;
		ConsListNode  *const next;

	private:

		size_t			   refs;
	};

	ConsList(ConsListNode *head)
		: head(head?head->reference():0)
	{
	}

	ConsListNode *head;
};

};

#endif//CONS_LIST_H

