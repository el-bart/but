# BUT

Basic Utilities and Tools is a set of useful helper functions, that make programming in C++14 easier.

following sections give a top-level overview on each part of the library.

## CI

download automated build of [BUT's SDK Docker image](https://hub.docker.com/r/baszerr/but-sdk/), with a command: `docker pull baszerr/but-sdk`.

### GCC

#### Debug

* regular [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Debug%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:592dc9b6b0b4db000166f035~GCC%20/%20Debug%20/%20none)
* address sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Debug%20%2F%20address&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593311a1cb02b300014679fd~GCC%20/%20Debug%20/%20address) (CI - docker w/o PTRACE capability)
* thread sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Debug%20%2F%20thread&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331317cb02b300014685ce~GCC%20/%20Debug%20/%20thread) ([problem on GCC < 7.1](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67308))
* undefined behavior sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Debug%20%2F%20undefined&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:5933132f711e490001678bb3~GCC%20/%20Debug%20/%20undefined)

#### Release

* regular: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Release%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:592f243f4c8edc00018b6366~GCC%20/%20Release%20/%20none)
* address sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Release%20%2F%20address&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:5933143fcb02b30001468f54~GCC%20/%20Release%20/%20address) (CI - docker w/o PTRACE capability)
* thread sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Release%20%2F%20thread&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593314d2c757810001165ee3~GCC%20/%20Release%20/%20thread) ([problem on GCC < 7.1](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67308))
* undefined behavior sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Release%20%2F%20undefined&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331507a11fd200010fdcbf~GCC%20/%20Release%20/%20undefined)

#### Misc

* Profile: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20Profile%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:592f2a444c8edc00018b9509~GCC%20/%20Profile%20/%20none)
* Release with debug info: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GCC%20%2F%20RelWithDebInfo%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331565c757810001166397~GCC%20/%20RelWithDebInfo%20/%20none)


### Clang

#### Debug

* regular: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Debug%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:5933158fcb02b300014699b3~Clang%20/%20Debug%20/%20none)
* address sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Debug%20%2F%20address&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593315bbf0fe8000018b2ed2~Clang%20/%20Debug%20/%20address) (CI - docker w/o PTRACE capability)
* thread sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Debug%20%2F%20thread&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593315ef2f40e50001f20f6e~Clang%20/%20Debug%20/%20thread)
* undefined behavior sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Debug%20%2F%20undefined&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331616711e49000167a381~Clang%20/%20Debug%20/%20undefined)

#### Release

* regular: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Release%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:5933163ce2f8a70001cea2db~Clang%20/%20Release%20/%20none)
* address sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Release%20%2F%20address&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593316b8a11fd200010fea75~Clang%20/%20Release%20/%20address) ([Clang problem](https://github.com/google/sanitizers/issues/647)) (CI - docker w/o PTRACE capability)
* thread sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Release%20%2F%20thread&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593316e7cb02b3000146a4cb~Clang%20/%20Release%20/%20thread)
* undefined behavior sanitizer: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Release%20%2F%20undefined&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331702a11fd200010fecc6~Clang%20/%20Release%20/%20undefined)

#### Misc

* Profile: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Profile%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:59331727a11fd200010fedf8~Clang%20/%20Profile%20/%20none) ([problem on Clang < 4.0](https://bugs.llvm.org/show_bug.cgi?id=31923))
* Release with debug info: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Clang%20%2F%20Relwithdebuginfo%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:5933174da11fd200010fef66~Clang%20/%20Relwithdebuginfo%20/%20none)


### Default

* Debug: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Default%20%2F%20Debug%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593317762f40e50001f21b94~Default%20/%20Debug%20/%20none)
* Release: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Default%20%2F%20Release%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593317b4e2f8a70001ceaecc~Default%20/%20Release%20/%20none)
* Profile: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Default%20%2F%20Profile%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593317d9cb02b3000146ac49~Default%20/%20Profile%20/%20none)
* Release with debug info: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=Default%20%2F%20Relwithdebinfo%20%2F%20none&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593317ff2f40e50001f22018~Default%20/%20Relwithdebinfo%20/%20none)


### Misc

* GNU/Make: [![Codefresh build status]( https://g.codefresh.io/api/badges/build?repoOwner=el-bart&repoName=but&branch=master&pipelineName=GNU%20%2F%20Make&accountName=el-bart&type=cf-1)]( https://g.codefresh.io/repositories/el-bart/but/builds?filter=trigger:build;branch:master;service:593319bf5845b900019d5906~GNU%20/%20Make)



## basic content
it is very useful when type safety is a must, but underlying type is already existing and working.
this is improved version of `struct MyIndex { unsigned value_; };` type of approach, that provides all the methods and encapsulation.
with this class in place, code can be easily refactored later on, if needed (adding new methods, validating c-tor, etc...
 * `Exception` - allows to both define and throw exceptions in one line of code each. automatically
passed info like file, line and function to the message. allows to create whole tree of exceptions.
 * `NotNull` - wrapper for (smart)pointers, that ensures element held inside is not null. if user would
pass such a pointer, exception will be thrown during construction.
 * `Optional` - template fixing boost::optional's issues with move semantics.
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
instead it is entry-oriented, where entry is a sequence of `<type,value>` pairs, that can be formatted later on.
this allows creating arbitrary filtering mechanisms and any output format, including structured ones.

for example logger can save timestamp, thread-id and message with 2 parameters.
this will render 5 (or more) parameters, that are named (by its type) and have value.
it is easy to convert such a data entry to structured json, xml or even log to data base or other key-value store.

all destinations implement dual interface.
on one hand they can support getting already-formatted sequence of type-value elements.
on the other there is a "raw" mode, where they get all the parameters "as is", from a caller.
this means that user can choose more abstract format, that can be changed at runtime, or static one,
with no extra overhead, if it is known that only a certain output type is required and all the abstractions
can be dropped.

loggers are designed to be passed by values, though it is also possible to make it "enterprise-style", by
providing a global variable, used for logging from everywhere.
user can also provide macros to wrap that up with things like: file names, line numbers or function names.

all destinations support `flush()` method, that forces all logs to be sent to their destination (file, socket, etc...).
another option is `reload()`, that forces to re-establish destination (reconnect, reopen file, etc...),
so that log rotation can be implemented.

 * `LoggerProxy` - proxy object making usage simpler. in order to use logger, one should provide convenience
wrapper for this object, adding fields to achieve required log message content.
 * `Destination` - namespace containing typical destinations, that are provided out of the box.
 * `Destination::Foregin` - base class for dynamic destinations.


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
