/* TC
 * id:    scn_swscanf_int_basic
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.2.2 (fwscanf); MS docs scanf type field characters / width specification
 * note:  %lld guarded to >=100 (ll support in older scanf not established; %I64d covers 64-bit there).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(dixuo) { int a = 0, b = 0; unsigned u = 0, x = 0, o = 0; int r;
    v = L"-12 0x1f 4000000000 ff 17"; r = swscanf((const wchar_t *)v, L"%d %i %u %x %o", &a, &b, &u, &x, &o);
    CHECK_EQ_I(r, 5); CHECK_EQ_I(a, -12); CHECK_EQ_I(b, 31); CHECK_EQ_U(u, 4000000000u); CHECK_EQ_U(x, 255); CHECK_EQ_U(o, 15); }
TC_CASE(i_bases) { int a = 0, b = 0, c = 0; int r; v = L"010 0x10 10"; r = swscanf((const wchar_t *)v, L"%i %i %i", &a, &b, &c);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(a, 8); CHECK_EQ_I(b, 16); CHECK_EQ_I(c, 10); }
TC_CASE(width) { int a = 0, b = 0; int r; v = L"123456"; r = swscanf((const wchar_t *)v, L"%2d%3d", &a, &b);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 345); }
TC_CASE(sizes) { short h = 0; long l = 0; __int64 q = 0; int r;
    v = L"-3 100000 -9000000000"; r = swscanf((const wchar_t *)v, L"%hd %ld %I64d", &h, &l, &q);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(h, -3); CHECK_EQ_I(l, 100000); CHECK(q == -9000000000LL); }
TC_CASE(ll) { long long q = 0; int r;
    if (!TC_AT_LEAST(100)) TC_SKIP("scanf %lld support before msvcr100 not established");
    v = L"9000000000"; r = swscanf((const wchar_t *)v, L"%lld", &q); CHECK_EQ_I(r, 1); CHECK(q == 9000000000LL); }
TC_CASE(suppress_n) { int a = 0, k = -1; int r; v = L"11 22 33"; r = swscanf((const wchar_t *)v, L"%*d %d%n", &a, &k);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 22); CHECK_EQ_I(k, 5); }
int main(void) { tc_begin(); TC_RUN(dixuo); TC_RUN(i_bases); TC_RUN(width); TC_RUN(sizes); TC_RUN(ll); TC_RUN(suppress_n); return tc_end(); }
