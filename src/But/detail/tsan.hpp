#pragma once
// helper header for providing wrappers for thread sanitizer on different compilers.

// due to a bug in TSAN, using Event::wait() version with timeout or deadline results in false-positives.
//   * gcc:   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=101978
//   * clang: https://github.com/google/sanitizers/issues/1259
// to avoid it these some tests need to be disabled when running under TSAN, for affected compilers


#if __clang__major__ <= 11
// clang detection:
// - https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual
// - https://clang.llvm.org/docs/ThreadSanitizer.html
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define BUT_THREAD_SANITIZER_ENABLED
#endif
#endif
#endif // clang


#if __GNUC__ <= 10
// gcc detection:
// - https://stackoverflow.com/questions/57499943/how-to-detect-thread-sanitizer-for-gcc-5
#ifdef __SANITIZE_THREAD__
#define BUT_THREAD_SANITIZER_ENABLED
#endif
#endif // gcc
