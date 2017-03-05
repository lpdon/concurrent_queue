#ifndef QUEUE_HPP
#define QUEUE_HPP

#ifndef _INTTYPES_H_
  #include <inttypes.h>
#endif

#ifndef LIST_HPP
  #include "list.hpp"
#endif

#include <pthread.h>

#ifdef __linux__
  #include <gtest/gtest_prod.h>
#endif

using namespace nsList;

namespace nsQueue
{

template<class T, uint32_t TSIZE=64U>
class cQueue
{
public:
  cQueue( void )
  : list()
  {
    pthread_mutex_init( &mutex, NULL );
    pthread_cond_init( &cond, NULL );
  }

  inline T getFront( void )
  {
    pthread_mutex_lock( &mutex );
    while ( list.getSize() == 0U )
    {
      pthread_cond_wait( &cond, &mutex );
    }
    const T loc_front = list.getHead()->obj;
    pthread_mutex_unlock( &mutex );
    return loc_front;
  }

  inline uint32_t getNumElements( void )
  {
    const uint32_t loc_size = list.getSize();
    return loc_size;
  }

  void enqueue( const T &arg_element );
  T dequeue( void );
  void removeWithKey( const typename T::keyType arg_key );

private:
  cList<T>   list;
  cList<T *> buckets[ TSIZE ];

  pthread_mutex_t mutex;
  pthread_cond_t cond;

  void _enqueue( const T &arg_element );
  void _dequeue( void );
  void _removeWithKey( const typename T::keyType arg_key );

  inline uint32_t hashKey( const typename T::keyType arg_key ) const
  {
    return ( static_cast<uint32_t>( arg_key ) % TSIZE );
  }

#ifdef __linux__
  FRIEND_TEST( QueueTestCase, RemoveWithKey );
#endif
};

template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::enqueue( const T &arg_element )
{
  pthread_mutex_lock( &mutex );
  _enqueue( arg_element );
  pthread_cond_signal( &cond );
  pthread_mutex_unlock( &mutex );
}

template<class T, uint32_t TSIZE>
T cQueue<T, TSIZE>::dequeue( void )
{
  pthread_mutex_lock( &mutex );
  while ( list.getSize() == 0U )
  {
    pthread_cond_wait( &cond, &mutex );
  }
  const T loc_front = list.getHead()->obj;
  _dequeue();
  pthread_mutex_unlock( &mutex );
  return loc_front;
}

template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::removeWithKey( const typename T::keyType arg_key )
{
  pthread_mutex_lock( &mutex );
  _removeWithKey( arg_key );
  pthread_mutex_unlock( &mutex );
}

template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::_enqueue( const T &arg_element )
{
  const uint32_t loc_idx = hashKey( arg_element.key() );
  list.insertNode( arg_element );
  buckets[ loc_idx ].insertNode( const_cast<T *>( &arg_element ) );
}

template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::_dequeue( void )
{
  if ( list.getHead() != NULL )
  {
    const T loc_obj = list.getHead()->obj;
    const uint32_t loc_idx = hashKey( loc_obj.key() );
    buckets[ loc_idx ].removeNode( const_cast<T *>( &loc_obj ) );
    list.removeNode( loc_obj );
  }
}

template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::_removeWithKey( const typename T::keyType arg_key )
{
  const uint32_t loc_idx = hashKey( arg_key );
  Node<T*> * loc_pBucketNode = const_cast< Node<T*> * >( buckets[ loc_idx ].getHead() );

  while ( loc_pBucketNode != NULL )
  {
    T * loc_pObj = loc_pBucketNode->obj;
    loc_pBucketNode = loc_pBucketNode->next;
    if ( arg_key == loc_pObj->key() )
    {
      T loc_obj = *loc_pObj;
      buckets[ loc_idx ].removeNode( loc_pObj );
      list.removeNode( loc_obj );
    }
  }
}

}

#endif
