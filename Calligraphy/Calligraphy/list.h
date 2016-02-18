#ifndef LIST_H
#define LIST_H

template<class T, class Alloc = alloc>
class list{
protected:
	typedef __list_node<T> list_node;
public:
	typedef list_node* link_type;
protected:
	link_type node;
	
}

#endif