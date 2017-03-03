//============================================================================
// Name        : concurrent_queue.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "../include/queue.hpp"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	nsQueue::cQueue<int> queueA;
	nsQueue::cQueue<Message> messageQueue;

	Message kapps, kapps2, kapps3;
	messageQueue.enqueue( kapps );

	cout << static_cast<uint32_t>( messageQueue.getNumElements() ) << endl;

	nsList::cList<Message> testList;
	testList.insertNode( kapps );
	testList.insertNode( kapps2 );
	testList.insertNode( kapps3 );
	testList.removeNode( kapps3 );

	return 0;
}
