#ifndef QUEUE_HPP
#define QUEUE_HPP

#ifndef _INTTYPES_H_
  #include <inttypes.h>
#endif

#ifndef MESSAGE_HPP
  #include "message.hpp"
#endif

#ifndef LIST_HPP
  #include "list.hpp"
#endif

#include <pthread.h>

using namespace nsList;

namespace nsQueue
{

typedef enum
{
    E_QUEUE_STATUS_OK,
    E_QUEUE_STATUS_FAILED
} eQueueStatus;

template<class T, typename TSIZETYPE=uint8_t, TSIZETYPE TSIZE=32U>
class cQueue
{
public:
  cQueue(void)
  : readIndex(0U)
  , writeIndex(0U)
  , numElements(0U)
  {
//    pthread_mutex_init( &mutex, NULL );
  }

  void enqueue( const T& arg_element );
  void dequeue( void );

  inline TSIZETYPE getNumElements( void ) const
  {
    return numElements;
  }

private:
  T buffer[ TSIZE ];
  TSIZETYPE readIndex;
  TSIZETYPE writeIndex;
  TSIZETYPE numElements;

  cList<T> list;

//  pthread_mutex_t mutex;

  eQueueStatus _enqueue( T arg_element );
  eQueueStatus _dequeue( void );
};

template<class T, typename TSIZETYPE, TSIZETYPE TSIZE>
eQueueStatus cQueue<T, TSIZETYPE, TSIZE>::enqueue(const T& arg_element)
{
//  pthread_mutex_lock( &mutex );
  const eQueueStatus loc_sts = _enqueue( arg_element );
//  pthread_mutex_unlock( &mutex );
  return loc_sts;
}

template<class T, typename TSIZETYPE, TSIZETYPE TSIZE>
eQueueStatus cQueue<T, TSIZETYPE, TSIZE>::dequeue(void)
{
//  pthread_mutex_lock( &mutex );
  const eQueueStatus loc_sts = _dequeue();
//  pthread_mutex_lock( &mutex );
  return loc_sts;
}

template<class T, typename TSIZETYPE, TSIZETYPE TSIZE>
eQueueStatus cQueue<T, TSIZETYPE, TSIZE>::_enqueue(T arg_element)
{

}

template<class T, typename TSIZETYPE, TSIZETYPE TSIZE>
eQueueStatus cQueue<T, TSIZETYPE, TSIZE>::_dequeue(void)
{
  const TSIZETYPE loc_readIndex = readIndex;

  if( readIndex == writeIndex )
  {
    // nop
    return E_QUEUE_STATUS_FAILED;
  }

  readIndex = ( readIndex >= TSIZE ) ? 0U : readIndex;
  numElements = ( numElements > 0U ) ? ( numElements - 1U ) : numElements;

  return E_QUEUE_STATUS_OK;
}

}

#endif
