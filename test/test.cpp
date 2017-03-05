#include <iostream>
#include <string>
#include <fstream>

#include "gtest/gtest.h"
#include "queue.hpp"
#include "list.hpp"
#include "message.hpp"

using namespace std;

namespace nsList {

TEST( ListTestCase, RemoveHead )
{
  cList<Message> loc_list;
  Message loc_msg[3U];

  for ( uint8_t i = 0U; i < 3U; i++ )
  {
    loc_list.insertNode( loc_msg[ i ] );
  }

  loc_list.removeNode( loc_msg[ 0U ] );

  EXPECT_EQ( loc_list.getSize(), 2U );
  EXPECT_FALSE( loc_list.getHead()->obj != loc_msg[ 1U ] );
}

TEST( ListTestCase, RemoveTail )
{
  cList<Message> loc_list;
  Message loc_msg[3U];

  for ( uint8_t i = 0U; i < 3U; i++ )
  {
    loc_list.insertNode( loc_msg[ i ] );
  }

  loc_list.removeNode( loc_msg[ 2U ] );

  EXPECT_EQ( loc_list.getSize(), 2U );
  EXPECT_FALSE( loc_list.getTail()->obj != loc_msg[ 1U ] );
}

TEST( ListTestCase, RemoveElement )
{
  cList<Message> loc_list;
  Message loc_msg[3U];

  for ( uint8_t i = 0U; i < 3U; i++ )
  {
    loc_list.insertNode( loc_msg[ i ] );
  }

  loc_list.removeNode( loc_msg[ 1U ] );

  EXPECT_EQ( loc_list.getSize(), 2U );
  EXPECT_FALSE( loc_list.getHead()->obj != loc_msg[ 0U ] );
  EXPECT_FALSE( loc_list.getTail()->obj != loc_msg[ 2U ] );
}

}

namespace nsQueue {

TEST( QueueTestCase, Enqueue )
{
  cQueue<Message> loc_messageQueue;
  Message loc_msg;
  loc_messageQueue.enqueue( loc_msg );

  EXPECT_EQ( loc_messageQueue.getNumElements(), 1U );
  EXPECT_FALSE( loc_msg != loc_messageQueue.getFront() );
}

TEST( QueueTestCase, Dequeue )
{
  cQueue<Message> loc_messageQueue;
  Message loc_msg;

  loc_messageQueue.enqueue( loc_msg );
  Message loc_msg2 = loc_messageQueue.dequeue();

  EXPECT_EQ( loc_messageQueue.getNumElements(), 0U );
  EXPECT_FALSE( loc_msg != loc_msg2 );
}

TEST( QueueTestCase, RemoveWithKey )
{
  cQueue<Message> loc_messageQueue;
  Message loc_msg[10U];

  for ( uint8_t i = 0U; i < 10U; i++ )
  {
    loc_msg[ i ].what = 1234;
    loc_messageQueue.enqueue( loc_msg[ i ] );
  }

  EXPECT_EQ( loc_messageQueue.getNumElements(), 10U );
  EXPECT_EQ( loc_messageQueue.buckets[ loc_messageQueue.hashKey( 1234 ) ].getSize(), 10U );
  loc_messageQueue.removeWithKey( 1234 );
  EXPECT_EQ( loc_messageQueue.getNumElements(), 0U );
}

}
