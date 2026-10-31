/* TC
 * id:    pfs_wstr_s_null
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf type field characters (wprintf: s = wide)
 * note:  NULL string args print (null), as wide literal
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile wnull = NULL;
static const char *volatile nnull = NULL;

TC_CASE(s)  { wchar_t w[32]; int n = _snwprintf(w, 32, L"%s", wnull); CHECK_WSTR(w, L"(null)"); CHECK_EQ_I(n, 6); }
TC_CASE(hs) { wchar_t w[32]; _snwprintf(w, 32, L"%hs", nnull); CHECK_WSTR(w, L"(null)"); }
TC_CASE(S)  { wchar_t w[32]; _snwprintf(w, 32, L"%S", nnull); CHECK_WSTR(w, L"(null)"); }
TC_CASE(width) { wchar_t w[32]; int n = _snwprintf(w, 32, L"[%10s]", wnull); CHECK_WSTR(w, L"[    (null)]"); CHECK_EQ_I(n, 12); }

int main(void) { tc_begin(); TC_RUN(s); TC_RUN(hs); TC_RUN(S); TC_RUN(width); return tc_end(); }
