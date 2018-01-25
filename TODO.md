# TODOs

## logger-related things

* consider adding bigger SSO to Backend::Value (compared to what std::string offers on GCC).
* add Log/Fields for: `DateTime`, `ModuleName`, `FunctionName`, `PrettyFunctionName`
* add `Log::Field::ProcessName`
* add fields: `Size`, `Index`, `Address`
* logger feature for buffering debug logs for some time, so that they are logged only if warning/error occurs, but dropping if nothing 'special' happens for some time
* add support for correlation chains (UUIDs/GUIDs).
* retry-sink, that will try to send log, in case of errors, multiple times.
* `Log::Dst::BackgroundThread` - add overflow policy (drop oldest, drop new, block).
* sub-second accuracy timestamp field.

## improvements / new APIs
* split build into core and dev
* `but::throw` with comma-based syntax, instead of streams (compatible with logging + no need for a macro to use it)
* `but::guard<A>` -> `but::guard<B>` (lambda -> `std::fuunction<void()>`)
* `backtrace` gathering helper
* add `BUT_THROW_BT()` that will throw exception and store `backtrace` inside
* add `System::TempDir` (alongside for temporary file creation)
* `Container::LRU`
* `Container::CircularBuffer`
* `Net` namespace, for synchronous, non-blocking I/O.
* remplate `Log::Dst::Common::Tcp`'s implementation with `But::Net`, once available...
* remove exception-throwing wait wrapper...
* Mpl::HasFreeFunction<>
* But::System::Timer
* Container::BitPacked - change base element size.

## misc ideas
* iterator wrappers to make creation of more-constrained iterators easier (eg. forward-iterator out of random-access iterator)
* add value-based configuration I/O (i.e. the one that does not require default c-tors and syntax is compact, when it comes to r/w cfg from/to file)
* `sharedString<>` - read-only data strucutre, that shares underlying data pointer between instances. convertible to `string_view`.
* add convenience function: `apply(function, tuple)` (C++17 has that)
* add `RawPtr<T>`, to be used with `NotNull<P>`
* consider unifying `NotNullT` vs. `makeTNNN` naming unification
* PRNG-seeded hash function wrapper, to prevent O(N) hash tables attack?
* `But::Mpl::LastType<>` in O(1)
* consider adding `But::Net` with some basic, easy to use wrappers
* C++ streams formating via overloads? sth. like out << hex(42) maybe? (sort of it done in `Log`, already)
* add signal handler helper, that will wrap lamba into a handler runner
* is it possible to evaluate `CONSTEXPR_ASSERT` in compile-time only, when doing non-debug build?
* `Struct::PackedPointerVector<T>`, where pointers to over-1B-aligned types are bit packed (based on `BitPackedSequence`)
* add test-covereage measuring build
* BUT_ASSERT_{EQ,LT,etc...} for more readable assert error messages.
