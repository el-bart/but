# BUT

Basic Utilities and Tools is a set of useful helper functions, that make programming in C++14 easier.

following sections give a top-level overview on each part of the library.


## basic content
it is very useful when type safety is a must, but underlying type is already existing and working.
this is improved version of `struct MyIndex { unsigned value_; };` type of approach, that provides all the methods and encapsulation.
with this class in place, code can be easily refactored later on, if needed (adding new methods, validating c-tor, etc...
 * `BUT_ASSERT` - assert that works equally well in normal and `constexpr` functions. removed in non-debug build.
 * `Exception` - allows to both define and throw exceptions in one line of code each. automatically
passed info like file, line and function to the message. allows to create whole tree of exceptions.
 * `NotNull` - wrapper for (smart)pointers, that ensures element held inside is not null. if user would
pass such a pointer, exception will be thrown during construction.
 * `Optional` - a version of `std::optional`, where moved-object is unset, instead of being set, but moved-from.
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
instead it is oriented at structured logging, where message is fixed (compile-time constant) and all parameters are named.
this allows creating arbitrary complex and nested output JSON format.
having machine processable logs allows for querying and filtering, in a non-ambiguous way.

note that whole implementation is more of a logger-framework.
user shall create own wrapper, that adds common fields required (eg. timestamp, log level, thread ID, etc...).
see `But/Log/ExampleLogger.manual.cpp` for an example, how such a custom wrapper can be crated.

loggers are designed to be passed around by values.
while it is also possible to make it "enterprise-style", by providing a global variable, used for logging from everywhere,
it's not recommended as it makes testing harder and does not guard against thread safety.

all destinations implement `Destination::Sink` interface.
they support getting already-formatted sequence of type-value elements.

all destinations support `flush()` method, that forces all logs to be sent to their destination (file, socket, etc...).
another option is `reload()`, that forces to re-establish destination (reconnect, reopen file, etc...),
so that log rotation can be implemented.

in order to add support for user's type/class `Abc`, it is enough to add two, free functions,
in structure's namespace (for ADL - Argument Dependent Lookup):
- `constexpr inline std::string_view fieldName(Abc const*) { return "My_type_name_is_Abc"; }`
- depending on `Abc`'s semantics, exactly one of 3 functions below:
  - `X fieldValue(Abc const& e)` - if `Abc` can be represented as a simple type `X` (eg. `int`, `dobule`, `string`, etc...)
  - `void objectValue(Backend::EntryProxy& p, Abc const& e)` - if `Abc` is an object type (contains multiple fields)
  - `void arrayValue(Backend::EntryArray& p, Abc const& e)` - if `Abc` is a collection

from this moment on, logger will be able to log your type just like any other.
this includes all the fields, eg. custom priorities, or domain-specific types.
framework will automatically pick correct "value function", depending on what's available.

note that `fieldName()` must be `constexpr`.
this is used internally, to provide additional checks at compile time (eg. if given field type is not used more than once).

logger object also supports `withFields(...)` member function, that creates a new logger, that will always append given values to each log it logs.
this way it is extremely easy to create correlation chains - just pass on "extended" loggers down the call chain, as the data gets processed.
added fields will always be added, to each log, w/o a need for internal code to know about it.

 * `Logger` - basic logger, object allowing to log in a structured fashion. user is expected to create a domain-specific wrapper to use in own projects.
 * `LoggerThrowing` - same as `Logger`, but forwarding exceptions from the implementation. useful only some unusual cases.
 * `Destination` - namespace containing typical destinations, that are provided out of the box.
 * `Destination::Sink` - base class for dynamic destinations.

 note that it is trivially possible to integrate with LogStash, via `Destination::Tcp` sink.


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


## library organization
source code is organized into files, name to signify its purpose.
following sections summarize the conventions.

### general rules
CamelCase is used throughout the code base.
directories map 1:1 to namespaces and start with upper-case.
classes are named upper-case.

build is organized into docker-based SDKs, that can be ran independently for:
* each compiler
* each mode (debug, release, etc...)
* each sanitizer (thread, address, etc...)
see `ci/run_all` for examples how to utilize these capabilities to have a good testing across many different configurations.

for rationale of these rules see [taming of the software](https://www.youtube.com/watch?v=NGLid96ceEo) video.

### files naming
aside from functions and classes, file extensions have extra meaning, too.
regular sources are named `*.cpp` and `*.hpp`, as usual.
test are named after test type, i.e.:
* `ut` is Unit Tests (1 class)
* `mt` is Module Tests (integration between different elements)
* `it` is Integration Tests (integration of bigger chunks and / or testing against actual OS dependencies)
* `mtest` is Manual TEST (application that does some action, that can be then verified manually for correctness)

### testing frameworks
BUT development started around 2014, but it's roots are dating back to around 2006, with (now abandoned) `System` library.
when it all started `gtest` and `gmock` were the state of the art.
nowadays `catch2` is much better choice.
however since BUT (as of 20025) is ~20KLOC, porting won't happen over night.

therefor old, `gtest`-based tests are named `*.<test_type>g.?pp` and new `catch2`-based are `*.<test_type>.?pp`.
e.g.:
* `Foo.utg.cpp` -> `gtest`-based UT for Foo class.
* `Bar.ut.cpp` -> `catch2`-based UT for Bar class.
* `xyz.ut.hpp` -> `catch2`-based helper code for UTs.
* `abc.utg.hpp` -> `gtest`-based helper code for UTs.
