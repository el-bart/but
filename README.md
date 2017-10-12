# BUT

Basic Utilities and Tools is a set of useful helper functions, that make programming in C++14 easier.

following sections give a top-level overview on each part of the library.

## CI

[![travis CI build status](https://travis-ci.org/el-bart/but.svg?branch=master)](https://travis-ci.org/el-bart/but)

download automated build of [BUT's SDK Docker image](https://hub.docker.com/r/baszerr/but-sdk/), with a command: `docker pull baszerr/but-sdk`.


## basic content
it is very useful when type safety is a must, but underlying type is already existing and working.
this is improved version of `struct MyIndex { unsigned value_; };` type of approach, that provides all the methods and encapsulation.
with this class in place, code can be easily refactored later on, if needed (adding new methods, validating c-tor, etc...
 * `BUT_ASSERT` - assert that works equally well in normal and `constexpr` functions. removed in non-debug build.
 * `Exception` - allows to both define and throw exceptions in one line of code each. automatically
passed info like file, line and function to the message. allows to create whole tree of exceptions.
 * `NotNull` - wrapper for (smart)pointers, that ensures element held inside is not null. if user would
pass such a pointer, exception will be thrown during construction.
 * `Optional` - template fixing boost::optional's issues with move semantics, multi-arg c-tors and references.
 * `Guard` - helper object (and "make" function) to generate RAII-style cleanup, for local elements, that
are not worth a dedicated, reusable class, yet still must be exception-safe.


## container
 * `Array` - `std::array` replacement, that is constexpr-compatible (will be typedef after C++17).
 * `ArrayWithSize` - `std::array` equivalent, that additionally holds a number of elements stored inside.
this makes it a perfect replacement for `std::vector`, when maximum size is small, and known a priory.
 * `BitPackedSequence` - `std::vector<bool>`-like structure, that is capable of storing elements that
take more than 1 bit. serialization and deserialization mechanisms are provided by user, to compact
each element into a given bit size.
 * `OffsetIterator` - helper structure for creating random-access iterator out of a table-access operator,
of a given container.
 * `UnorderedArray` - cache-friendly data structure. keeps adding, browsing and removal fast in exchange
for lack of guarantees on objects order, after addition/removal of any element.


## logger
this module implements a type-safe, style-unified logger, with different output "destinations".
note that this logger is NOT line oriented!
instead it is entry-oriented, where entry is a sequence of (possibly nested) `<type,value>` pairs, that can be formatted / indexed later on.
this allows creating arbitrary filtering mechanisms and any output format, including structured ones (Json, XML, etc...).

for example logger can save timestamp, thread-id and message as 3 separate parameters.
this will render structures, that are named ("tagger") and have value.
it is easy to convert such a data entry to structured Json, XML or even log to data base or other key-value store.

all destinations implement `Destination::Sink`.
they support getting already-formatted sequence of type-value elements.

loggers are designed to be passed by values, though it is also possible to make it "enterprise-style", by
providing a global variable, used for logging from everywhere.
user can also provide macros to wrap that up with things like: file names, line numbers or function names.

all destinations support `flush()` method, that forces all logs to be sent to their destination (file, socket, etc...).
another option is `reload()`, that forces to re-establish destination (reconnect, reopen file, etc...),
so that log rotation can be implemented.

in order to add support for user's type/class `Abc`, it is enough to add `toFieldInfo(Abc const&) -> Backend::FieldInfo`
free function, to the same namespace `Abc` has been declared in (for ADL - Argument Dependent Lookup).
from this moment on, logger will be able to log your type just like any other.

 * `LoggerProxy` - proxy object making usage simpler. in order to use logger, one should provide convenience.
 * `LoggerProxyThrowing` - same as `LoggerProxy`, but forwarding exceptions from the implementation. useful in some unusual requirements scenarios.
 * `Destination` - namespace containing typical destinations, that are provided out of the box.
 * `Destination::Sink` - base class for dynamic destinations.


## meta programming
 * `BUT_MPL_VALUE_WRAP` - macro for generating full-blown object, that's only purpose is to keep a given value inside.
 * `FreeOperators` - set of macros for fast definitions of comparison operators for a simple structure types
and collections. both regular and template types are supported.
 * `NamedVariadicTemplate` - helper template that allows to store variadic template arguments in a
typedef, that can be expanded later on.
 * `SizeTypeFor` - type-deriving template, that provides the smallest, unsigned, integral type, that will
fit given value.
 * parseSigned/parseUnsigned - parses numbers at compile-time (and runtime, if needed).


## (design) patterns
 * `AbstractFactory` - compact implementation of abstract factory, based on functors. this makes C++14
code compact on user's side, as there is no need to create special derived classes for builders.
 * `Dispatcher` - C++14 implementation of dispatcher pattern, keeping user code as short as possible.
in general user should only derive from the class and provide names of message to be dispatched.


## system (utilities)
 * `Descriptor` - smart-pointer-like file/socket descriptor wrapper (RAII style).
 * `TempFile` - convenient wrapper for creating temporary files in one line.


## threading
 * `ActiveObject` - functor based active object implementation.
 * `BasicLockable` - base class for extending given derived, with a basic-lockable interface concept.
this means the class can be used as a mutex too (eg. a collection with external locking support).
 * `CacheLine` - helper data type wrapper, that helps preventing false-sharing issues in multithreaded
code.
 * `Event` - event flag, that can be set in one thread and block another, until it is not set.
 * `Fifo` - thread-safe queue to passing data between threads.
 * `JoiningThread` - wrapper for thread that joins thread in d-tor. any thread's implementation with
C++11-compatible API will work.
 * `LockProxyProvider` - base class for providing `withLock()` method, that returns temporary object, with
overloaded arrow-operator, to make one-shot, synchronized class, one-liners in implementation as well.
 * `ThreadPool` - policy-driven thread pool implementation.
 * `WaitWrapper` - helper structure that provide a `wait()` wrappers, that throw exceptions in case of
time-out. these are useful for testing, but keep in mind that having a blocking-wait in a high-throughput
system, effectively makes low-throughput system. ;)


## Format
 * `BUT_FORMAT` - helper for generated parsed format, that can check syntax at compile time and process arguments at runtime (arity is always checked at compile time, too). since output format for each parameter is predefined, effectively all checks are done `static_assert`s.
 * `BUT_FORMAT_RUNTIME` - variant of the `BUT_FORMAT` that performs all checks at run time.
