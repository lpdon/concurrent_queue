# concurrent_queue

###About

Simple queue with thread-safe operations, implemented with POSIX threads.

Underneath the queue works as doubly linked-list. As the queue is FIFO, the list this way is able to insert elements at the end and remove from the beginning with constant time complexity ~ O(1). 

To ensure the integrity of the queue, every enqueue and dequeue operation locks a mutex before changing the queue and releases it afterwards. The dequeue operation returns only if is able to retrieve a valid object, which means that it blocks when the queue is empty and waits for a signal. The enqueue operation, after adding an object, sends the signal. 

The classic way to dequeue objects from a queue would be to read the front and execute a pop. This could run into some dangerous race conditions if more than one thread is trying to dequeue, so the decision was to use both in an atomic operation after locking the mutex to ensure that the read and popped objects are the same.

The queue also supports removing all the objects which match a given key. Using only a list the only possibility is to brute-force through all the elements. To avoid this, it uses a simple hash table. Every line of the table has all the address of a given key. As the table has limited size and collision could happen (more than one key in each line), every line is a list and handles it through chaining.

Developed and tested on Ubuntu 14.04. An .exe for Windows is provided, built using Eclipse internal builder. 

###Usage
```testqueue n```

where n = number of producers and consumers.

P.S: depending on the number of producers the log order can be confusing. 

Running this provides and "endless" producer-consumer scenario. 

###Install instructions

The program uses Google Test Framework for Unit Tests. Makefile generation is done with cmake.

To init the submodules after cloning the repo:

```git submodule init```

```git submodule update --recursive```

###Tests

In the test folder there are some tests for basic functionality. To run them after building enter:

```make test```

***