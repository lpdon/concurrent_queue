#include <iostream>

#include "message.hpp"
#include "queue.hpp"

#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutexCout;

struct sThreadArg
{
  uint32_t id;
  nsQueue::cQueue<Message> &queue;
};

void producerThread( uint32_t arg_id, nsQueue::cQueue<Message>& arg_queue )
{
  auto loc_id = arg_id;
  auto& loc_queue = arg_queue;

  srand( time( nullptr ) ^ int( loc_id ) );

  while ( true )
  {
    Message loc_message;
    loc_message.what = rand()%1000;
    loc_queue.enqueue( loc_message );
    auto loc_size = loc_queue.getNumElements();

    {
      std::lock_guard<std::mutex> loc_lock( mutexCout );
      std::cout << "Producer -- " << " id: " << loc_id << " size: " << loc_size << " what: " << loc_message.what << std::endl;
    }

    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
  }
}

void consumerThread( uint32_t arg_id, nsQueue::cQueue<Message>& arg_queue )
{
  auto loc_id = arg_id;
  auto& loc_queue = arg_queue;

  while ( true )
  {
    auto loc_message = loc_queue.dequeue();
    auto loc_size = loc_queue.getNumElements();

    {
      std::lock_guard<std::mutex> loc_lock( mutexCout );
      std::cout << "            " << " id: " << loc_id << " size: " << loc_size << " what: " << loc_message.what << " -- Consumer" << std::endl;
    }

    std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
  }
}

int main( int argc, char* argv[] )
{
  nsQueue::cQueue<Message> messageQueue;

  //uint8_t loc_size = atoi( argv[1] );
  const uint8_t loc_size = 2U;
  std::thread producer[ loc_size ];
  std::thread consumer[ loc_size ];

  for ( uint8_t i = 0U; i < loc_size; ++i )
  {
    sThreadArg loc_arg = { i, messageQueue };

    producer[ i ] = std::thread( [ &i, &messageQueue ]{ return producerThread( i, messageQueue ); } );
    consumer[ i ] = std::thread( [ &i, &messageQueue ]{ return consumerThread( i, messageQueue ); } );
  }

  for ( uint8_t i = 0U; i < loc_size; ++i )
  {
    producer[i].join();
    consumer[i].join();
  }

  return 0;
}
