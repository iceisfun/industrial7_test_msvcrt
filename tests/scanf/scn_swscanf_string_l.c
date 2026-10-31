/* TC
 * id:    scn_swscanf_string_l
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.2.2; MS docs scanf width/type: %ls %lc %l[
 * note:  Avoids bare %s/%c (MS legacy vs ISO ambiguity covered by scn_common_* tests).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;
static void fillw(wchar_t *p, int n) { int i; for (i = 0; i < n; i++) p[i] = L'Z'; }

TC_CASE(ls) { wchar_t b[16]; int r; fillw(b, 16); v = L"  hello world"; r = swscanf((const wchar_t *)v, L"%ls", b);
    CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"hello"); CHECK_EQ_I(b[6], L'Z'); }
TC_CASE(lc) { wchar_t b[4]; int r; fillw(b, 4); v = L"xyz"; r = swscanf((const wchar_t *)v, L"%lc", b);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(b[0], L'x'); CHECK_EQ_I(b[1], L'Z'); }
TC_CASE(lc_count) { wchar_t b[6]; int r; fillw(b, 6); v = L"abcdef"; r = swscanf((const wchar_t *)v, L"%3lc", b);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[1], L'b'); CHECK_EQ_I(b[2], L'c'); CHECK_EQ_I(b[3], L'Z'); }
TC_CASE(ls_width) { wchar_t b[16]; int r; fillw(b, 16); v = L"abcdef"; r = swscanf((const wchar_t *)v, L"%3ls", b);
    CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(b[4], L'Z'); }
TC_CASE(scanset) { wchar_t b[16]; int r, k = -1; fillw(b, 16); v = L"abcXYZ"; r = swscanf((const wchar_t *)v, L"%l[a-z]%n", b, &k);
    CHECK_EQ_I(r, 1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(k, 3); CHECK_EQ_I(b[4], L'Z'); }
TC_CASE(two_words) { wchar_t a[8], b[8]; int r; v = L"one\ttwo"; r = swscanf((const wchar_t *)v, L"%ls %ls", a, b);
    CHECK_EQ_I(r, 2); CHECK_WSTR(a, L"one"); CHECK_WSTR(b, L"two"); }
int main(void) { tc_begin(); TC_RUN(ls); TC_RUN(lc); TC_RUN(lc_count); TC_RUN(ls_width); TC_RUN(scanset); TC_RUN(two_words); return tc_end(); }
