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
  sThreadArg * loc_pArg = reinterpret_cast<sThreadArg *>( arg_queue );
  uint32_t loc_id = loc_pArg->id;
  nsQueue::cQueue<Message>& loc_queue = loc_pArg->queue;

  srand( time( NULL ) ^ int( loc_id ) );

  while ( true )
  {
    Message loc_message;
    loc_message.what = rand()%1000;
    loc_queue.enqueue( loc_message );
    uint32_t loc_size = loc_queue.getNumElements();

    pthread_mutex_lock( &mutexCout );
    cout << "Producer -- " << " id: " << loc_id << " size: " << loc_size << " what: " << loc_message.what << endl;
    pthread_mutex_unlock( &mutexCout );

    sleep( 1 );
  }

  pthread_exit( NULL );
  return NULL;
}

void * consumerThread( void * arg_queue )
{
  sThreadArg * loc_pArg = reinterpret_cast<sThreadArg *>( arg_queue );
  uint32_t loc_id = loc_pArg->id;
  nsQueue::cQueue<Message>& loc_queue = loc_pArg->queue;

  while ( true )
  {
    Message loc_message = loc_queue.dequeue();
    uint32_t loc_size = loc_queue.getNumElements();

    pthread_mutex_lock( &mutexCout );
    cout << "            " << " id: " << loc_id << " size: " << loc_size << " what: " << loc_message.what << " -- Consumer" << endl;
    pthread_mutex_unlock( &mutexCout );

    sleep( 2 );
  }
  pthread_exit( NULL );
  return NULL;
}

int main( int argc, char* argv[] )
{
	nsQueue::cQueue<Message> messageQueue;

	pthread_mutex_init( &mutexCout, NULL );

	uint8_t loc_size = atoi( argv[1] );
	pthread_t producer[ loc_size ];
	pthread_t consumer[ loc_size ];

	for ( uint8_t i = 0U; i < loc_size; ++i )
	{
	  sThreadArg loc_arg = { i, messageQueue };
	  pthread_create( &producer[ i ], NULL, producerThread, (void *)&loc_arg );
    pthread_create( &consumer[ i ], NULL, consumerThread, (void *)&loc_arg );
	}

  for ( uint8_t i = 0U; i < loc_size; ++i )
  {
    pthread_join( producer[ i ], NULL );
    pthread_join( consumer[ i ], NULL );
  }

	return 0;
}
