BUT
===

Basic Utilities and Tools is a set of useful helper functions, that make programming in C++14 easier.


==== basic content
 * Exception - allows to both define and throw exceptions in one line of code each. automatically
passed info like file, line and function to the message. allows to create whole tree of exceptions.
 * NotNull - wrapper for (smart)pointers, that ensures element held inside is not null. if user would
pass such a pointer, exception will be thrown during construction.
 * UnorderedArray - cache-friendly data structure. keeps adding, browsing and removal fast in exchange
for lack of guarantees on objects order, after addition/removal of any element.


==== meta programming
 * NamedVariadicTemplate - helper template that allows to store variadic template arguments in a
typedef, that can be expanded later on.


==== design patterns
 * AbstractFactory - compact implementation of abstract factory, based on functors. this makes C++14
code compact on user's side, as there is no need to create special derived classes for builders.
 * Dispatcher - C++14 implementation of dispatcher pattern, keeping user code as short as possible.
in general user should only derive from the class and provide names of message to be dispatched.


==== system utilities
 * Descriptor - smart-pointer-like file/socket descriptor wrapper (RAII style).


==== threading
 * ActiveObject - functor based active object implementation.
 * CacheLine - helper data type wrapper, that helps preventing false-sharing issues in multithreaded
code.
 * Event - event flag, that can be set in one thread and block another, until it is not set.
 * Fifo - thread-safe queue to passing data between threads.
 * JoiningThread - wrapper for thread that joins thread in d-tor. any thread's implementation with
C++11-compatible API will work.
