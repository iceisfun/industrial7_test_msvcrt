/* pfq_shim.h - portable wrappers (pq_*) for the positional-printf (_p) family.
 * ucrtbase: the _p functions are inline in the SDK headers over __stdio_common_v*_p (options 0 = default local options).
 * older CRTs: they are real exports (VS2005+), declared here because mingw headers lack them.
 * Only the wrappers a test uses end up in its import table (unused statics are dropped at -O).
 */
#ifndef PFQ_SHIM_H
#define PFQ_SHIM_H
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#ifdef _UCRT

static int pq_vsprintf_p(char *b, size_t n, const char *f, va_list ap)       { return __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); }
static int pq_vswprintf_p(wchar_t *b, size_t n, const wchar_t *f, va_list ap){ return __stdio_common_vswprintf_p(0, b, n, f, NULL, ap); }
static int pq_vfprintf_p(FILE *s, const char *f, va_list ap)                 { return __stdio_common_vfprintf_p(0, s, f, NULL, ap); }
static int pq_vfwprintf_p(FILE *s, const wchar_t *f, va_list ap)             { return __stdio_common_vfwprintf_p(0, s, f, NULL, ap); }
static int pq_vprintf_p(const char *f, va_list ap)                           { return __stdio_common_vfprintf_p(0, stdout, f, NULL, ap); }
static int pq_vscprintf_p(const char *f, va_list ap)                         { return __stdio_common_vsprintf_p(0, NULL, 0, f, NULL, ap); }
static int pq_vscwprintf_p(const wchar_t *f, va_list ap)                     { return __stdio_common_vswprintf_p(0, NULL, 0, f, NULL, ap); }
#else
/* real exports of msvcr90..120: call them directly so they appear in the import table */
int __cdecl _vsprintf_p(char *, size_t, const char *, va_list);
int __cdecl _vswprintf_p(wchar_t *, size_t, const wchar_t *, va_list);
int __cdecl _vfprintf_p(FILE *, const char *, va_list);
int __cdecl _vfwprintf_p(FILE *, const wchar_t *, va_list);
int __cdecl _vprintf_p(const char *, va_list);
int __cdecl _vscprintf_p(const char *, va_list);
int __cdecl _vscwprintf_p(const wchar_t *, va_list);
int __cdecl _sprintf_p(char *, size_t, const char *, ...);
int __cdecl _swprintf_p(wchar_t *, size_t, const wchar_t *, ...);
int __cdecl _fprintf_p(FILE *, const char *, ...);
int __cdecl _fwprintf_p(FILE *, const wchar_t *, ...);
int __cdecl _printf_p(const char *, ...);
int __cdecl _scprintf_p(const char *, ...);
int __cdecl _scwprintf_p(const wchar_t *, ...);
#define pq_vsprintf_p _vsprintf_p
#define pq_vswprintf_p _vswprintf_p
#define pq_vfprintf_p _vfprintf_p
#define pq_vfwprintf_p _vfwprintf_p
#define pq_vprintf_p _vprintf_p
#define pq_vscprintf_p _vscprintf_p
#define pq_vscwprintf_p _vscwprintf_p
#define pq_sprintf_p _sprintf_p
#define pq_swprintf_p _swprintf_p
#define pq_fprintf_p _fprintf_p
#define pq_fwprintf_p _fwprintf_p
#define pq_printf_p _printf_p
#define pq_scprintf_p _scprintf_p
#define pq_scwprintf_p _scwprintf_p
#endif

#ifdef _UCRT
/* variadic wrappers (go through the v* forms, which is what the UCRT inlines do too) */
#define PQ_VA(ret, name, vname, protoargs, callargs, last) \
    static ret name protoargs { va_list ap; ret r; va_start(ap, last); r = vname callargs; va_end(ap); return r; }
PQ_VA(int, pq_sprintf_p,   pq_vsprintf_p,   (char *b, size_t n, const char *f, ...),    (b, n, f, ap), f)
PQ_VA(int, pq_swprintf_p,  pq_vswprintf_p,  (wchar_t *b, size_t n, const wchar_t *f, ...), (b, n, f, ap), f)
PQ_VA(int, pq_fprintf_p,   pq_vfprintf_p,   (FILE *s, const char *f, ...),              (s, f, ap), f)
PQ_VA(int, pq_fwprintf_p,  pq_vfwprintf_p,  (FILE *s, const wchar_t *f, ...),           (s, f, ap), f)
PQ_VA(int, pq_printf_p,    pq_vprintf_p,    (const char *f, ...),                        (f, ap), f)
PQ_VA(int, pq_scprintf_p,  pq_vscprintf_p,  (const char *f, ...),                        (f, ap), f)
PQ_VA(int, pq_scwprintf_p, pq_vscwprintf_p, (const wchar_t *f, ...),                     (f, ap), f)
#endif
#endif
