#ifndef QUEUE_HPP
#define QUEUE_HPP

#ifndef _INTTYPES_H_
  #include <inttypes.h>
#endif

#ifndef LIST_HPP
  #include "list.hpp"
#endif

#ifndef _MUTEX_
  #include <mutex>
#endif

#ifndef _CONDITION_VARIABLE_
  #include <condition_variable>
#endif

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
  , mutex()
  {

  }

  inline uint32_t getNumElements( void )
  {
    return list.getSize();
  }

  T getFront( void );
  void enqueue( const T &arg_element );
  T dequeue( void );
  void removeWithKey( const typename T::keyType arg_key );

private:
  cList<T>   list;
  cList<T *> buckets[ TSIZE ];

  std::mutex mutex;
  std::condition_variable cond;

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

/*
 * Returns a copy of the first element,
 * locks if the queue is empty. In a non
 * concurrent scenario, one should check
 * the size before using the method.
 *
 * Runtime: O(1)
 */
template<class T, uint32_t TSIZE>
T cQueue<T, TSIZE>::getFront( void )
{
  std::unique_lock<std::mutex> loc_lock( mutex );
  cond.wait( loc_lock, [this]{ return ( list.getSize() != 0U ); } );
  const T loc_front = list.getHead()->obj;
  return loc_front;
}

/*
 * Adds an object to the end of the
 * queue.
 *
 * Runtime: O(1)
 */
template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::enqueue( const T &arg_element )
{
  {
    std::lock_guard<std::mutex> loc_lock( mutex );
    _enqueue( arg_element );
  }
  cond.notify_all();
}

/*
 * Removes and returns the beginning
 * of the queue, locks if the queue
 * is empty. In a non concurrent scenario,
 * one should check the size before
 * using the method.
 *
 * Runtime: O(1)
 */
template<class T, uint32_t TSIZE>
T cQueue<T, TSIZE>::dequeue( void )
{
  std::unique_lock<std::mutex> loc_lock( mutex );
  cond.wait( loc_lock, [this]{ return ( list.getSize() != 0U ); } );
  const T loc_front = list.getHead()->obj;
  _dequeue();
  return loc_front;
}

/*
 * Removes all objects of the queue
 * with a given key. The object is
 * expected to specify a keyType and
 * have a key() method.
 *
 * Runtime:
 * Best case: O(1), only one object with key
 * Average case: O(n), where n = number of
 * objects with the key
 * Worst case: O(n), where n = number of
 * objects in the queue, if the number of
 * collisions is too high
 */
template<class T, uint32_t TSIZE>
void cQueue<T, TSIZE>::removeWithKey( const typename T::keyType arg_key )
{
  std::lock_guard<std::mutex> lock( mutex );
  _removeWithKey( arg_key );
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
  if ( list.getHead() != nullptr )
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

  while ( loc_pBucketNode != nullptr )
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
