# release notes

short summary of each release, with version and release date.

## current development
work in progress.
this work is available on "master" branch, but has not yet been released.

## v0.2.0 (2017-03-27)
small interface updates along with minor fixes here and there.
the only purely-new elements is filtering backend for logger.

 * some push\_back()s replaced with emplace\_back()s
 * fixed typo in foreign destination base class name
 * now linking with boost\_filesystem
 * Log::Destination::Filter
 * convenience using statements for NotNull<> pointer
 * unified pointer getting interface of NotNull<>::pointer() for lvalue and rvalue types

## v0.1.0 (2017-03-20)
after doing continuous development for quite a while a time has come for the first release.

 * basic utilities:
   * Exception
   * NotNull
   * Optional
   * Guard
 * containers:
   * ArrayWithSize
   * BitPackedSequence
   * OffsetIterator
   * UnorderedArray
 * type-safe logger
 * meta programming:
   * FreeOperators
   * NamedVariadicTemplate
   * SizeTypeFor
 * design patterns:
   * AbstractFactory
   * Dispatcher
 * system utilities:
   * Descriptor
   * TempFile
 * threading:
   * ActiveObject
   * BasicLockable
   * CacheLine
   * Event
   * Fifo
   * JoiningThread
   * LockProxyProvider
   * WaitWrapper
