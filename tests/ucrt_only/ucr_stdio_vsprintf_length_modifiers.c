/* TC
 * id:    ucr_stdio_vsprintf_length_modifiers
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1; MS docs printf Size Prefixes (hh, h, l, ll, I, I32, I64, z, j, t)
 * note:  long is 32 bits on Windows; %Iu is size_t sized.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static int vs(unsigned long long opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static double mk(unsigned long long bits) { union { double d; unsigned long long u; } x; x.u = bits; return x.d; }
static volatile unsigned long long v_bits;
static double dv(unsigned long long bits) { v_bits = bits; return mk(v_bits); }

static volatile int v300 = 300, v70000 = 70000, v65537 = 65537;
TC_CASE(hh) { char b[64]; vs(0, b, sizeof b, "%hhd|%hhu", v300, v300); CHECK_STR(b, "44|44"); }
TC_CASE(h) { char b[64]; vs(0, b, sizeof b, "%hd|%hu", v70000, v65537); CHECK_STR(b, "4464|1"); }
TC_CASE(l_ll) { char b[96]; vs(0, b, sizeof b, "%ld|%lu|%lld|%llu", LONG_MIN, ULONG_MAX, LLONG_MIN, ULLONG_MAX);
    CHECK_STR(b, "-2147483648|4294967295|-9223372036854775808|18446744073709551615"); }
TC_CASE(I64_I32) { char b[96]; vs(0, b, sizeof b, "%I64d|%I64u|%I32d|%I32u", (__int64)-9000000000LL, (unsigned __int64)9000000000ULL, (__int32)-5, (unsigned __int32)4000000000u);
    CHECK_STR(b, "-9000000000|9000000000|-5|4000000000"); }
TC_CASE(z_j_t) { char b[96]; vs(0, b, sizeof b, "%zu|%jd|%td|%zx", (size_t)123456, (intmax_t)-7, (ptrdiff_t)-9, (size_t)255); CHECK_STR(b, "123456|-7|-9|ff"); }
TC_CASE(ptr_size) { char b[96]; size_t s = SIZE_MAX; vs(0, b, sizeof b, "%Iu", s);
    CHECK_STR(b, sizeof(void *) == 8 ? "18446744073709551615" : "4294967295"); }
TC_CASE(hex_upper) { char b[96]; vs(0, b, sizeof b, "%llx|%llX|%lX|%#llx", 0xdeadbeefcafeULL, 0xdeadbeefcafeULL, 0xabcdef01UL, 0x1fULL); CHECK_STR(b, "deadbeefcafe|DEADBEEFCAFE|ABCDEF01|0x1f"); }

int main(void)
{
    tc_begin();
    TC_RUN(hh);
    TC_RUN(h);
    TC_RUN(l_ll);
    TC_RUN(I64_I32);
    TC_RUN(z_j_t);
    TC_RUN(ptr_size);
    TC_RUN(hex_upper);
    return tc_end();
}
