/* TC
 * id:    pfw_wide_upper_s_hs_narrowstr
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters: in wide functions S and hs = narrow (char) string
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 64; i++) b[i] = L'Z'; }
static const char * volatile ns = "xyz";
static const char * volatile es = "";
TC_CASE(S_ascii)  { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%S]", ns); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[xyz]"); CHECK_EQ_I(n, 5); }
TC_CASE(hs_ascii) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%hs]", ns); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[xyz]"); CHECK_EQ_I(n, 5); }
TC_CASE(S_with_width_precision) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%5S][%-5hs][%.2S]", ns, ns, ns); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[  xyz][xyz  ][xy]"); CHECK_EQ_I(n, 18); }
TC_CASE(empty_narrow) { wchar_t b[64]; int n; fill(b); n = _snwprintf(b, 63, L"[%S][%hs][%3S]", es, es, es); b[n < 0 ? 0 : n] = 0; CHECK_WSTR(b, L"[][][   ]"); CHECK_EQ_I(n, 9); }
int main(void) { tc_begin(); TC_RUN(S_ascii); TC_RUN(hs_ascii); TC_RUN(S_with_width_precision); TC_RUN(empty_narrow); return tc_end(); }
