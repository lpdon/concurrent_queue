#ifndef LIST_HPP
#define LIST_HPP

#ifndef _STDLIB_H
  #include <stdlib.h>
#endif

#ifndef _INTTYPES_H_
  #include <inttypes.h>
#endif

namespace nsList
{

template<class T>
struct Node
{
  T obj;
  Node * previous;
  Node * next;
};

template<class T>
class cList
{
public:
  cList( void )
  : head( NULL )
  , tail( NULL )
  , size( 0U )
  {

  }

  inline uint32_t getSize( void ) const
  {
    return size;
  }

  inline const Node<T> * const getHead( void ) const
  {
    return head;
  }

  inline const Node<T> * const getTail( void ) const
  {
    return tail;
  }

//  void insertNode( const Node<T> * const arg_node );
  void insertNode( const T &arg_node );
//  void removeNode( const Node<T> * const arg_node );
  void removeNode( const T &arg_node );

private:
  Node<T> * head;
  Node<T> * tail;
  uint32_t size;
};

template<class T>
//void cList<T>::insertNode( const Node<T> * const arg_node )
void cList<T>::insertNode( const T &arg_node )
{
//  if ( arg_node != NULL )
  {
    Node<T> * const loc_node = new Node<T>();
    loc_node->obj = arg_node;
    loc_node->previous = NULL;
    loc_node->next = NULL;

    if ( ( head == NULL ) && ( tail == NULL ) )
    {
      head = loc_node;
      tail = loc_node;
//      head->previous = NULL;
//      head->next = tail;
//      tail->previous = head;
//      tail->next = NULL;
    }
    else
    {
      Node<T> * const loc_currentTail = tail;
      tail = loc_node;
      tail->previous = loc_currentTail;
      tail->next = NULL;
      loc_currentTail->next = tail;
    }

    ++size;
  }
}

template<class T>
//void cList<T>::removeNode( const Node<T> * const arg_node )
void cList<T>::removeNode( const T &arg_node )
{
  Node<T> * loc_node = head;

  while ( ( loc_node != NULL ) && ( loc_node->obj != arg_node ) )
  {
    loc_node = loc_node->next;
  }

  if ( loc_node != NULL )
  {
    Node<T> * loc_previous = loc_node->previous;
    Node<T> * loc_next = loc_node->next;

    if ( loc_previous != NULL )
    {
      loc_previous->next = loc_next;
    }
    else //node to be removed is head
    {
      head = loc_next;
    }

    if ( loc_next != NULL )
    {
      loc_next->previous = loc_previous;
    }
    else //node to be removed is tail
    {
      tail = loc_previous;
    }

    --size;
    delete loc_node;
  }
}

}


#endif
