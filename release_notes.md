# release notes

short summary of each release, with version and release date.


## current development
work in progress.
this work is available on `master` branch, but has not yet been released.

  * support for tests in `catch2` inside the library code
  * `But::System::SocketPair` added (RAII-wrapper for `socketpair()` syscall)


## v2.0.0 (2023-08-13)
major redesign of `But::Log`, including user-facing interface.
shifting away from `boost` in favor of corresponding `std` components.
renewed SDKs.
many minor tweaks and improvements.

  * tooling:
    * C++17 is now new minimal version required.
    * dropped support for gcc: 7, 8.
    * dropped support for clang: 6, 7.
    * added support for gcc: 11, 12.
    * added support for clang: 13, 14, 15.
    * all SDKs now contain also basic tools like `vim` and `gdb`.
  * `But::Log`:
    * performance is nearly double, with still a big space for improvement.
    * `Logger*` now check for uniqueness of arguments at compile time.
    * `Proxy*` classes have been renamed to (more adequate) `Logger*`.
    * `Field::Priority` has been renamed to (more adequate) `Field::LogLevel`.
  * removed:
    * `boost::thread` support in But::Threading is now dropped (bugs, compatibility issues, not that relevant nowadays).
    * removed example integration with ELK stack - it was PoC only, not a production one.
    * removed integration with `boost::enable_current_exception`, as C++ standard has now proper tools for a long time.
  * internal changes:
    * moved from `boost::filesystem` to `std::filesystem`.
    * `But::Optional` is now based on `std::optional`.
    * certain tests are now disabled for TSAN builds, due to bugs in TSAN itself (`BUT_THREAD_SANITIZER_ENABLED` test macro).
  * `System::CpuTimer` added - wrapper around intrinsic for counting CPU cycles, for very precise time measurements.
  * `BUT_MPL_VALUE_WRAP` is now split into: `BUT_MPL_VALUE_WRAP_CP`, `BUT_MPL_VALUE_WRAP_MV` and `BUT_MPL_VALUE_WRAP_CP_MV`, depending if type is copyable, movable or both.


## v1.2.1 (2022-11-02)
minor bugfix

  * fixed missing `#pragma once` in `date2str.hpp`.


## v1.2.0 (2022-07-10)
set of minor features.
final release of `1.x` series, before bigger changes planned for `2.0`.

  * note that on some versions on gcc and clang, `thread` sanitizer may report errors when using `But::Thread::Event::wait()` with timeout.
    `Event`'s implementation is fine. this is TSAN's bug:
    - [gcc bug report](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=101978)
    - [clang bug report](https://github.com/google/sanitizers/issues/1259)
  * `System::Descriptor` no longer calls `close(-1)` (while not required by POSIX, it can avoid a syscall and also silences Valgrind warning).
  * added per-toolchain flags support
  * added support for Clang versions: 9, 11
  * added support for GCC versions: 9, 10
  * changed SDK concept, to have 1 SDK per toolchain (it's far more flexible).
  * added `BUT_TYPED_TEST_SUITE_P` (and others) to cover gtest naming changes from `TYPED_TEST_CASE_P` to `TYPED_TEST_SUITE_P` in recent gtest releases.
  * fixes for Clang-7 and  GCC-8 compilation.
  * dropped support `toFieldInfo()` for `volatile char` combinations.
  * `Guard` is now explicit about holding functors by value only.
  * added `run_in_sdk` helper script to do docker-based build (using provided SDK).
  * fixed compilation problem with `gtest` and variadic macro expansion for type parametrized tests.
  * `nlohmann::json` v3 is now used instead of v2.
  * added support for Clang 7.
  * `Container::ArrayWithSize::back()` added, for easier access.
  * `Container::ArrayWithSize::front()` added, for easier access.
  * `Container::BitPackedSequence::resize()` added (2 overloads, as in `std::vector`).


## v1.1.2 (2021-02-10)
minor release fixing couple of issues on different compiler versions.

  * fixed warnings on clang-7.
  * removed old toolchains from build.
  * GCC-6 support is dropped (it's no longer in Debian:10 repos).
  * fixed clang compilation (empty `...` in macro).


## v1.1.1 (2020-01-24)
minor release to fix 32-bit compilation bug.

  * fixed type size mismatches on 32-bit architectures in `Log::Value` and `Log::toFieldInfo` (used to be a compilation error).


## v1.1.0 (2018-10-10)
another round date. ;)
this release contains usability improvements and bugfixes.
mostly related to logging.

  * fixed logging of empty collections - now tag remains, but the content is empty.
  * dropped support for clang 4.0 (packages also removed from latest Debian).
  * `Optional` now compiles correctly with copy-initialization from a non-const `Optional`.
  * `System::TempFile` now do not leave `XXX` artifact in file name.
  * `toFieldInfo` is now able to pretty-convert `std::exception`-derived classes, `std::pair`, `std::vector`, `std::tuple` and `std::chrono` time units.
  * `But::Optional` now has relational operators defined.
  * `Log::Field::PreciseDT` for nanosecond-precision timestamps in logs.
  * `Optional::emplace(...)` can now be used to conveniently create objects in place, including those that are non-copyable and non-movable.
  * added support for GCC-8 and Clang-6.
  * `Log::Destination::Verbose*` family of text-type outputs, that keep tag info in a human-readable format and with depth-limiting factor.
  * `Log::Proxy*::withFields()` API for easy creating correlation chains.
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
