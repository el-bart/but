# BUT

Basic Utilities and Tools is a set of useful helper functions, that make programming in C++14 easier.

following sections give a top-level overview on each part of the library.


## basic content
 * Exception - allows to both define and throw exceptions in one line of code each. automatically
passed info like file, line and function to the message. allows to create whole tree of exceptions.
 * NotNull - wrapper for (smart)pointers, that ensures element held inside is not null. if user would
pass such a pointer, exception will be thrown during construction.


## container
 * ArrayWithSize - std::array equivalent, that additionally holds a number of elements stored inside.
this makes it a perfect replacement for std::vector, when maximum size is small, and known a priory.
 * BitPackedSequence - std::vector<bool>-like structure, that is capable of storing elements that
take more than 1 bit. serialization and deserialization mechanisms are provided by user, to compact
each element into a given bit size.
 * OffsetIterator - helper structure for creating random-access iterator out of a table-access operator,
of a given container.
 * UnorderedArray - cache-friendly data structure. keeps adding, browsing and removal fast in exchange
for lack of guarantees on objects order, after addition/removal of any element.


## meta programming
 * FreeOperators - set of macros for fast definitions of comparison operators for a simple structure types
and collections. both regular and template types are supported.
 * NamedVariadicTemplate - helper template that allows to store variadic template arguments in a
typedef, that can be expanded later on.
 * SizeTypeFor - type-deriving template, that provides the smallest, unsigned, integral type, that will
fit given value.


## (design) pattern
 * AbstractFactory - compact implementation of abstract factory, based on functors. this makes C++14
code compact on user's side, as there is no need to create special derived classes for builders.
 * Dispatcher - C++14 implementation of dispatcher pattern, keeping user code as short as possible.
in general user should only derive from the class and provide names of message to be dispatched.


## system (utilities)
 * Descriptor - smart-pointer-like file/socket descriptor wrapper (RAII style).


## threading
 * ActiveObject - functor based active object implementation.
 * CacheLine - helper data type wrapper, that helps preventing false-sharing issues in multithreaded
code.
 * BasicLockable - base class for extending given derived, with a basic-lockable interface concept.
this means the class can be used as a mutex too (eg. a collection with external locking support).
 * Event - event flag, that can be set in one thread and block another, until it is not set.
 * Fifo - thread-safe queue to passing data between threads.
 * JoiningThread - wrapper for thread that joins thread in d-tor. any thread's implementation with
C++11-compatible API will work.
 * LockProxyProvider - base class for providing withLock() method, that returns temporary object, with
overloaded arrow-operator, to make one-shot, synchronized class, one-liners in implementation as well.
 * WaitWrapper - helper structure that provide a wait() wrappers, that throw exceptions in case of
time-out. these are useful for testing, but keep in mind that having a blocking-wait in a high-throughput
system, effectively makes low-throughput system. ;)
