/* TC
 * id:    pfi_int_len_ll
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p7; MS docs printf size prefixes (ll)
 * note:  msvcrt.dll (Win7) ll behaviour is undocumented and excluded; see separate probe
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>

TC_CASE(lld) { char b[64]; volatile long long mn = LLONG_MIN, mx = LLONG_MAX; sprintf(b, "%lld|%lld", mn, mx); CHECK_STR(b, "-9223372036854775808|9223372036854775807"); }
TC_CASE(llu) { char b[64]; volatile unsigned long long u = ULLONG_MAX; sprintf(b, "%llu", u); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(llx) { char b[64]; volatile long long m = -1; sprintf(b, "%llx|%llX", m, m); CHECK_STR(b, "ffffffffffffffff|FFFFFFFFFFFFFFFF"); }
TC_CASE(llo) { char b[64]; volatile unsigned long long u = ULLONG_MAX; sprintf(b, "%llo", u); CHECK_STR(b, "1777777777777777777777"); }
TC_CASE(lli) { char b[64]; volatile long long m = -5000000000LL; sprintf(b, "%lli", m); CHECK_STR(b, "-5000000000"); }
TC_CASE(plus_width) { char b[64]; volatile long long m = 1LL << 40; sprintf(b, "[%+20lld]", m); CHECK_STR(b, "[      +1099511627776]"); }

int main(void) { tc_begin(); TC_RUN(lld); TC_RUN(llu); TC_RUN(llx); TC_RUN(llo); TC_RUN(lli); TC_RUN(plus_width); return tc_end(); }
