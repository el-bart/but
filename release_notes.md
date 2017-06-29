# release notes

short summary of each release, with version and release date.

## current development
work in progress.
this work is available on "master" branch, but has not yet been released.

  * `toString(bool)` added for logger
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
