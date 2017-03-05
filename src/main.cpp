//============================================================================
// Name        : concurrent_queue.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "message.hpp"
#include "queue.hpp"

#include <unistd.h>
#include <time.h>

using namespace std;

pthread_mutex_t mutexCout;

struct sThreadArg
{
  uint32_t id;
  nsQueue::cQueue<Message> &queue;
};

void * producerThread( void * arg_queue )
{
//  uint32_t loc_id = 0U;//pthread_self();
//  nsQueue::cQueue<Message> * loc_pQueue = reinterpret_cast<nsQueue::cQueue<Message> *>( arg_queue );
//  nsQueue::cQueue<Message>& loc_queue = *loc_pQueue;

  sThreadArg * loc_pArg = reinterpret_cast<sThreadArg *>( arg_queue );
  uint32_t loc_id = loc_pArg->id;
  nsQueue::cQueue<Message>& loc_queue = loc_pArg->queue;

  while ( true )
  {
    Message loc_message;
    loc_message.what = rand()%1000;
    loc_queue.enqueue( loc_message );

    pthread_mutex_lock( &mutexCout );
    cout << "Producer -- " << " id: " << loc_id << " size: " << loc_queue.getNumElements() << " what: " << loc_message.what << endl;
    pthread_mutex_unlock( &mutexCout );

//    usleep( 100000/2 );
    sleep( 1 );
//    nanosleep((const struct timespec[]){{0, 250000000L}}, NULL);
  }

  pthread_exit( NULL );
  return NULL;
}

void * consumerThread( void * arg_queue )
{
//  uint32_t loc_id = 0U;//pthread_self();
//  nsQueue::cQueue<Message> * loc_pQueue = reinterpret_cast<nsQueue::cQueue<Message> *>( arg_queue );
//  nsQueue::cQueue<Message>& loc_queue = *loc_pQueue;

  sThreadArg * loc_pArg = reinterpret_cast<sThreadArg *>( arg_queue );
  uint32_t loc_id = loc_pArg->id;
  nsQueue::cQueue<Message>& loc_queue = loc_pArg->queue;

  while ( true )
  {
//    Message loc_message;
//    loc_queue.enqueue( loc_message );
    loc_queue.getFront();
    Message loc_message = loc_queue.dequeue();

    pthread_mutex_lock( &mutexCout );
    cout << "            " << " id: " << loc_id << " size: " << loc_queue.getNumElements() << " what: " << loc_message.what << " -- Consumer" << endl;
//    cout << "Consumer -- " << " size: " << loc_queue.getNumElements() << endl;
    pthread_mutex_unlock( &mutexCout );

//    usleep( 100000 );
    sleep( 2 );
//    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
  }
  pthread_exit( NULL );
  return NULL;
}

int main() {
	nsQueue::cQueue<Message> messageQueue;

//	Message kapps, kapps2, kapps3;
//	messageQueue.enqueue( kapps );
//	messageQueue.enqueue( kapps2 );
//	messageQueue.enqueue( kapps3 );

//	messageQueue.removeWithKey( kapps.key() );

	srand ( time( NULL ) );

  cout << static_cast<uint32_t>( messageQueue.getNumElements() ) << endl;

	pthread_mutex_init( &mutexCout, NULL );

	uint8_t loc_size = 10U;
	pthread_t producer[ loc_size ];
	pthread_t consumer[ loc_size ];

	for ( uint8_t i = 0U; i < loc_size; ++i )
	{
	  sThreadArg loc_arg = { i, messageQueue };
	  pthread_create( &producer[ i ], NULL, producerThread, (void *)&loc_arg );
    pthread_create( &consumer[ i ], NULL, consumerThread, (void *)&loc_arg );
//    pthread_create( &producer[ i ], NULL, producerThread, (void *)&messageQueue );
//    pthread_create( &consumer[ i ], NULL, consumerThread, (void *)&messageQueue );
	}

  for ( uint8_t i = 0U; i < loc_size; ++i )
  {
    pthread_join( producer[ i ], NULL );
    pthread_join( consumer[ i ], NULL );
  }

	return 0;
}
