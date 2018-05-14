# release notes

short summary of each release, with version and release date.

## current development
work in progress.
this work is available on "master" branch, but has not yet been released.

  * `Log::Backend::FieldInfo` now supports `vector<T>` as a collection type.
  * `Threading::LockProxy` is no longer implementation detail.
  * `Threading::LockProxy` provides `operator*()` and a member function `get()`, for convenience.

## v1.0.0 (2017-12-12)
new logger concept, based on a structure logging idea (https://www.baszerr.eu/lib/exe/fetch.php/docs/structured_logging.pdf).
build system updated and simplified.
countless small fixes and improvements - see the list below for the full overview.

  * `Log::Proxy` and `Log::ProxyThrowing` are now copy-and-move assignable.
  * `Threading::Fifo` now provides `withLock()` convenience helper, that returnes proxy, holding a lock.
  * conditional build with sanitizers with `BUT_SANITIZE` option.
  * conditional build of with link-time-optimization (LTO) with `BUT_ENABLE_LTO` option.
  * conditional build of place-independent-code with `BUT_ENABLE_PIC` option.
  * conditional build of tests with `BUT_ENABLE_TESTS` option.
  * arguments before `BUT_FORMAT` are now allowed as well (these do not participate in formatting).
  * added `Log::Destination::AutoFlush`, that forwards all logs to a `Sink` and automatically flushes, after each call.
  * added `Log::Destination::BackgroundThread`, that forwards all logs to a `Sink`, in a separate thread.
  * disabled tests for `ThreadPool`, using `boost` backend - GCC's thread sanitizer detects data races in boost...
  * all `BUT`'s includes are now in a form `<include>` instead of `"include"`, to better suite installation purposes.
  * `Threading::Fifo`, `Threading::Event` and `Threading::WaitWrapper` no longer throw exceptions on timeout - `true`/`false` is returned instead.
  * CI moved from codefresh to travis.
  * added `install` target, that installs lib and headers into `CMAKE_INSTALL_PREFIX` location.
  * `toFieldInfo()` is now the base interface for logging.
  * `Log::Proxy` and `Log::ProxyThrowing` that wrap all the logging with and without exceptions forwarding.
  * all `assert()` statements in the code are now `BUT_ASSERT()`.
  * `BUT_ASSERT` - assert that works with `constexpr` functions, too.
  * `Log::Destination::TextFile` - the same as `TextConsole`, but writing lines to a file.
  * `Log::Destination::TextConsole` - for simplest, possible logging on the screen.
  * `Log::Destination::TextStream` - generic `Sink`, to write data (make it structured or unstructured) to a given stream.
  * `Log::Destination::JsonTcp` - same as `JsonConsole`, but writing to a remote `host`:`port` endpoint (can be used to integration with LogStash!).
  * `Log::Destination::JsonFile` - same as `JsonConsole`, but writing to a file.
  * `Log::Destination::JsonConsole` - writes logs as Json entries - one entry per line.
  * `Log::Destination::Syslog` - logs to system `syslogd`, using linux API.
  * `Log::Destination::MultiSink` - convenient wrapper, allowing logging to multiple `Sink`s at the same time.
  * `Log::Destination::Null` - `Sink` that ignores all the arguments (mostly for testing).
  * `But::Format` module for doing basic, common string formatting, both at compile-time and at run-time.
  * `Optional<T>` now supports multi-argument c-tors.
  * `Log::Field::Pid` added.
  * added GCC-7.1 and Clang-4.0 to supported toolchains (and as default ones on CI).
  * `Optional`'s c-tors are no longer explicit.
  * automated docker SDK build on dockerhub: baszerr/but-sdk
  * `Mpl::parse{Signed,Unsigned}` - number parsing, that works at compile time and runtime.
  * fixed `Optional` construction from `const` object.
  * `NotNull::pointer` is deprecated in favor of more descriptive name `NotNull::underlyingPointer`.
  * `System::Descriptor` has not explicit `bool` conversion operator.
  * `BUT_MPL_VALUE_WRAP` macro for generating full-blown object, that's only purpose is to keep a given value inside.
  * `toValue(bool)` added for logger
  * `NotNull<>` has now a workaround for GCC compilation bug (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81182)
  * automated docker SDK build on dockerhub: `baszerr/but-sdk`
  * `BUT_THROW()` now throws `boost::current_exception()`-compatible types (boost does not work with C++11 correctly here).
  * `Container::Array` - a temporary std::array<> replacement, that works with constexpr functions.
  * `Mpl::NamedVariadicTemplate` now supports getting arguments count.
  * `gold` is now used as a default linkder.
  * added logger fields: `ThreadId` and `ThreadNo` (more human-friendly representation).
  * test headers now have `#pragma once`.
  * added generic thread pool implementation (policy based).
  * `ActiveObject` is now policy-driven.
  * exception now shows all the messages from all the base classes, in error message.
  * added `Log::Destination::Null`, that ignores all the output (useful for tests, that need to take logger argument).
  * all automated tests are now run in random order (with printing seed each time).
  * test applications generate XML reports in JUnit format.
  * color output from ctest.

## v0.2.0 (2017-03-27)
small interface updates along with minor fixes here and there.
the only purely-new elements is filtering backend for logger.

 * some `push_back()`s replaced with `emplace_back()`s
 * fixed typo in foreign destination base class name
 * now linking with `boost_filesystem`
 * `Log::Destination::Filter`
 * convenience using statements for `NotNull<>` pointer
 * unified pointer getting interface of `NotNull<>::pointer()` for lvalue and rvalue types

## v0.1.0 (2017-03-20)
after doing continuous development for quite a while a time has come for the first release.

 * basic utilities:
   * `Exception`
   * `NotNull`
   * `Optional`
   * `Guard`
 * containers:
   * `ArrayWithSize`
   * `BitPackedSequence`
   * `OffsetIterator`
   * `UnorderedArray`
 * type-safe logger
 * meta programming:
   * `FreeOperators`
   * `NamedVariadicTemplate`
   * `SizeTypeFor`
 * design patterns:
   * `AbstractFactory`
   * `Dispatcher`
 * system utilities:
   * `Descriptor`
   * `TempFile`
 * threading:
   * `ActiveObject`
   * `BasicLockable`
   * `CacheLine`
   * `Event`
   * `Fifo`
   * `JoiningThread`
   * `LockProxyProvider`
   * `WaitWrapper`
