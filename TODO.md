# TODOs

## issues


## logger-related things

* `BUT_FORMAT` used from `But::Log::Logger::log()` for types with `tag()`ed `fieldValue` or direct value, should give just the value - not full declaration with type name.
* add Log/Fields for: `DateTime`, `ModuleName`, `FunctionName`, `PrettyFunctionName`
* add `Log::Field::ProcessName`
* logger feature for buffering debug logs for some time, so that they are logged only if warning/error occurs, but dropping if nothing 'special' happens for some time
* add support for correlation chains (UUIDs/GUIDs).
* retry-sink, that will try to send log, in case of errors, multiple times.
* `Log::Dst::BackgroundThread` - add overflow policy (drop oldest, drop new, block).
* add `destination` that would retry sending logs, according to a given setup (retry count, max wait time, etc...).
* macros for logging: `BUT_LOG_FILE`/`LINE`/`FUNCTION` + aggregated `BUT_LOG_SOURCE` that collects all 3.
* consider auto-trimming leading `../` sequences inside filename, when logging filename via macro (can be a dedicated type, if needed)
* add support for `std::optional<T>`, where name of `T` would be added as a field, but `null` would become its output value.


## improvements / new APIs
* `Optional::get_value_or()`
* split build into core and dev
* `but::throw` with comma-based syntax, instead of streams (compatible with logging + no need for a macro to use it)
* `but::guard<A>` -> `but::guard<B>` (lambda -> `std::function<void()>`)
* `backtrace` gathering helper
* add `BUT_THROW_BT()` that will throw exception and store `backtrace` inside
* add `System::TempDir` (alongside for temporary file creation)
* `Container::LRU`
* `Container::CircularBuffer`
* `Net` namespace, for synchronous, non-blocking I/O.
* replace `Log::Dst::Common::Tcp`'s implementation with `But::Net`, once available...
* remove exception-throwing wait wrapper...
* Mpl::HasFreeFunction<>
* But::System::Timer
* Container::BitPacked - change base element size.
* `BUT_ASSERT_{EQ,LT,etc...}` for more readable assert error messages.
* `BUT_THROW` and adding optional parameters, that can be later on extracted from the client. consider using FieldInfo, for logger support.
* maybe NullPtr for `unique_ptr` could offer `operator[]`, for convenience
* exception could hold tuple<args>, that would then be queryble (to keep exact values, that caused an error, etc...) and (optionally) loggable. `toString` vs `toFieldInfo` vs both? or storage by value and query by derived class only?
* `MovablePointer<T>` - class that will be a raw pointer, but will support moving (i.e. source is nulled at the end).
* `AbstractFactory` should return `NotNullUnique` pointer.
* change `BUT_THROW`'s `ss` internal variable to sth more unique, so that accidental name-slash is unlikely.
* consider auto-trimming leading `../` sequences inside filename, when throwing exception.
* `ThreadPool::queueSize()` for listing enqueued elements count.
* `But::Guard::trigger()` to trigger guard before d-tor is reached.
* `BUT_ASSERT_ALWAYS()` for asserts that shall remain in release builds as well.
* helper to check if UTF-8 sequence is valid ([utf-8 recovery overview](https://baszerr.eu/doku.php?id=blog:2020:04:24:2020-04-24_-_utf-8_recovery)).
* helper to replace invalid UTF-8 characters from the sequence (similar code to "validator" above).
* `System` should contain `Epoll` (interruptable)


## misc ideas
* iterator wrappers to make creation of more-constrained iterators easier (eg. forward-iterator out of random-access iterator)
* add value-based configuration I/O (i.e. the one that does not require default c-tors and syntax is compact, when it comes to r/w cfg from/to file)
* `sharedString<>` - read-only data structure, that shares underlying data pointer between instances. convertible to `string_view`.
* add convenience function: `apply(function, tuple)` (C++17 has that)
* add `RawPtr<T>`, to be used with `NotNull<P>`
* consider unifying `NotNullT` vs. `makeTNNN` naming unification
* PRNG-seeded hash function wrapper, to prevent O(N) hash tables attack?
* `But::Mpl::LastType<>` in O(1)
* consider adding `But::Net` with some basic, easy to use wrappers
* C++ streams formatting via overloads? sth. like out << hex(42) maybe? (sort of it done in `Log`, already)
* add signal handler helper, that will wrap lambda into a handler runner
* is it possible to evaluate `CONSTEXPR_ASSERT` in compile-time only, when doing non-debug build?
* `Struct::PackedPointerVector<T>`, where pointers to over-1B-aligned types are bit packed (based on `BitPackedSequence`)
* add test-coverage measuring build
* add x86 (i.e. 32-bit) architecture to the CI
