/* pfl_shim.h - portable wrappers (pl_*) for the locale-taking printf family (_l variants).
 * ucrtbase: these are inline in the SDK headers over __stdio_common_v* (what is replicated here; options as the SDK default:
 *           narrow 0, wide 4 = legacy wide specifiers, vsnprintf-style 1 = legacy null termination).
 * older CRTs (msvcr90..120): real exports, declared here with dllimport (mingw headers are incomplete/inconsistent).
 */
#ifndef PFL_SHIM_H
#define PFL_SHIM_H
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

#ifdef _UCRT
static int pl_vsprintf_l(char *b, const char *f, _locale_t l, va_list ap)              { return __stdio_common_vsprintf(0ULL, b, (size_t)-1, f, l, ap); }
static int pl_vsnprintf_l(char *b, size_t n, const char *f, _locale_t l, va_list ap)   { return __stdio_common_vsprintf(1ULL, b, n, f, l, ap); }
static int pl_vswprintf_l(wchar_t *b, const wchar_t *f, _locale_t l, va_list ap)       { return __stdio_common_vswprintf(4ULL, b, (size_t)-1, f, l, ap); }
static int pl_vsnwprintf_l(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, va_list ap) { return __stdio_common_vswprintf(5ULL, b, n, f, l, ap); }
static int pl_vfprintf_l(FILE *s, const char *f, _locale_t l, va_list ap)              { return __stdio_common_vfprintf(0ULL, s, f, l, ap); }
static int pl_vfwprintf_l(FILE *s, const wchar_t *f, _locale_t l, va_list ap)          { return __stdio_common_vfwprintf(4ULL, s, f, l, ap); }
static int pl_vprintf_l(const char *f, _locale_t l, va_list ap)                        { return __stdio_common_vfprintf(0ULL, stdout, f, l, ap); }
static int pl_vwprintf_l(const wchar_t *f, _locale_t l, va_list ap)                    { return __stdio_common_vfwprintf(4ULL, stdout, f, l, ap); }
static int pl_vscprintf_l(const char *f, _locale_t l, va_list ap)                      { return __stdio_common_vsprintf(2ULL, NULL, 0, f, l, ap); }
static int pl_vscwprintf_l(const wchar_t *f, _locale_t l, va_list ap)                  { return __stdio_common_vswprintf(6ULL, NULL, 0, f, l, ap); }
#else
__declspec(dllimport) int __cdecl _vsprintf_l(char *, const char *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vsnprintf_l(char *, size_t, const char *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vsnwprintf_l(wchar_t *, size_t, const wchar_t *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vfprintf_l(FILE *, const char *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vfwprintf_l(FILE *, const wchar_t *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vprintf_l(const char *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vwprintf_l(const wchar_t *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vscprintf_l(const char *, _locale_t, va_list);
__declspec(dllimport) int __cdecl _vscwprintf_l(const wchar_t *, _locale_t, va_list);
#define pl_vsprintf_l _vsprintf_l
#define pl_vsnprintf_l _vsnprintf_l
#define pl_vsnwprintf_l _vsnwprintf_l
#define pl_vfprintf_l _vfprintf_l
#define pl_vfwprintf_l _vfwprintf_l
#define pl_vprintf_l _vprintf_l
#define pl_vwprintf_l _vwprintf_l
#define pl_vscprintf_l _vscprintf_l
#define pl_vscwprintf_l _vscwprintf_l
#endif

#define PL_VA(name, vname, protoargs, callargs, last) \
    static int name protoargs { va_list ap; int r; va_start(ap, last); r = vname callargs; va_end(ap); return r; }
PL_VA(pl_sprintf_l,   pl_vsprintf_l,   (char *b, const char *f, _locale_t l, ...),              (b, f, l, ap), l)
PL_VA(pl_snprintf_l,  pl_vsnprintf_l,  (char *b, size_t n, const char *f, _locale_t l, ...),    (b, n, f, l, ap), l)
PL_VA(pl_snwprintf_l, pl_vsnwprintf_l, (wchar_t *b, size_t n, const wchar_t *f, _locale_t l, ...), (b, n, f, l, ap), l)
PL_VA(pl_fprintf_l,   pl_vfprintf_l,   (FILE *s, const char *f, _locale_t l, ...),             (s, f, l, ap), l)
PL_VA(pl_fwprintf_l,  pl_vfwprintf_l,  (FILE *s, const wchar_t *f, _locale_t l, ...),          (s, f, l, ap), l)
PL_VA(pl_wprintf_l,   pl_vwprintf_l,   (const wchar_t *f, _locale_t l, ...),                    (f, l, ap), l)
PL_VA(pl_printf_l,    pl_vprintf_l,    (const char *f, _locale_t l, ...),                       (f, l, ap), l)
PL_VA(pl_scprintf_l,  pl_vscprintf_l,  (const char *f, _locale_t l, ...),                       (f, l, ap), l)
PL_VA(pl_scwprintf_l, pl_vscwprintf_l, (const wchar_t *f, _locale_t l, ...),                    (f, l, ap), l)
#ifdef _UCRT
PL_VA(pl_swprintf_l,  pl_vswprintf_l,  (wchar_t *b, const wchar_t *f, _locale_t l, ...),       (b, f, l, ap), l)
#endif
#endif
