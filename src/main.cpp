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

void * producerThread( void * arg_queue )
{
  nsQueue::cQueue<Message> * loc_pQueue = reinterpret_cast<nsQueue::cQueue<Message> *>( arg_queue );
  nsQueue::cQueue<Message>& loc_queue = *loc_pQueue;

  while ( true )
  {
    Message loc_message;
    loc_message.what = rand()%1000;
    loc_queue.enqueue( loc_message );

    pthread_mutex_lock( &mutexCout );
    cout << "Producer -- " << " size: " << loc_queue.getNumElements() << " what: " << loc_message.what << endl;
    pthread_mutex_unlock( &mutexCout );

//    usleep( 100000/2 );
//    sleep( 1 );
    nanosleep((const struct timespec[]){{0, 250000000L}}, NULL);
  }

  pthread_exit( NULL );
  return NULL;
}

void * consumerThread( void * arg_queue )
{
  nsQueue::cQueue<Message> * loc_pQueue = reinterpret_cast<nsQueue::cQueue<Message> *>( arg_queue );
  nsQueue::cQueue<Message>& loc_queue = *loc_pQueue;

  while ( true )
  {
//    Message loc_message;
//    loc_queue.enqueue( loc_message );
    loc_queue.getFront();
    Message loc_message = loc_queue.dequeue();

    pthread_mutex_lock( &mutexCout );
    cout << "Consumer1 -- " << " size: " << loc_queue.getNumElements() << " what: " << loc_message.what << endl;
//    cout << "Consumer -- " << " size: " << loc_queue.getNumElements() << endl;
    pthread_mutex_unlock( &mutexCout );

//    usleep( 100000 );
//    sleep( 2 );
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
  }
  pthread_exit( NULL );
  return NULL;
}

void * consumerThread2( void * arg_queue )
{
  nsQueue::cQueue<Message> * loc_pQueue = reinterpret_cast<nsQueue::cQueue<Message> *>( arg_queue );
  nsQueue::cQueue<Message>& loc_queue = *loc_pQueue;

  while ( true )
  {
//    Message loc_message;
//    loc_queue.enqueue( loc_message );
    loc_queue.getFront();
    Message loc_message = loc_queue.dequeue();

    pthread_mutex_lock( &mutexCout );
    cout << "Consumer2 -- " << " size: " << loc_queue.getNumElements() << " what: " << loc_message.what << endl;
//    cout << "Consumer -- " << " size: " << loc_queue.getNumElements() << endl;
    pthread_mutex_unlock( &mutexCout );

//    usleep( 100000 );
//    sleep( 2 );
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
  }
  pthread_exit( NULL );
  return NULL;
}


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	nsQueue::cQueue<Message> messageQueue;

	Message kapps, kapps2, kapps3;
//	messageQueue.enqueue( kapps );
//	messageQueue.enqueue( kapps2 );
//	messageQueue.enqueue( kapps3 );

	messageQueue.removeWithKey( kapps.key() );

	srand ( time( NULL ) );

  cout << static_cast<uint32_t>( messageQueue.getNumElements() ) << endl;

	pthread_mutex_init( &mutexCout, NULL );

	pthread_t producer;
	pthread_t producer2;
	pthread_t consumer;
	pthread_t consumer2;

	pthread_create( &producer, NULL, producerThread, (void *)&messageQueue );
	pthread_create( &producer2, NULL, producerThread, (void *)&messageQueue );
	pthread_create( &consumer, NULL, consumerThread, (void *)&messageQueue );
	pthread_create( &consumer2, NULL, consumerThread2, (void *)&messageQueue );

//	messageQueue.dequeue();
//	messageQueue.dequeue();
//	messageQueue.dequeue();
//
//	messageQueue.dequeue();

	pthread_join( producer, NULL );

	return 0;
}
