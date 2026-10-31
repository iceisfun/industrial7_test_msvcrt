/* TC
 * id:    pfm_2_swprintf_l_signature_probe
 * area:  stdio_printf
 * funcs:
 * procs: _swprintf_l __swprintf_l _swprintf_c_l
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs sprintf, _sprintf_l, swprintf, _swprintf_l (docs show both a no-count legacy and a count-taking form)
 * note:  PROBES which export carries which signature. Guess: these DLLs export the legacy form (buf, fmt, locale, ...) as
 *        _swprintf_l or __swprintf_l. (ucrtbase has _swprintf_l only as a header inline and is excluded.) _swprintf_c_l (count form, documented) is a separate case.
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *legacy_t)(wchar_t *, const wchar_t *, void *, ...);
typedef int (__cdecl *count_t)(wchar_t *, size_t, const wchar_t *, void *, ...);

TC_CASE(count_c_l)
{
    wchar_t b[16]; FARPROC p = tc_crt_proc("_swprintf_c_l"); int n;
    if (!p) TC_SKIP("_swprintf_c_l absent");
    n = ((count_t)p)(b, 16, L"%d", NULL, 5);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], L'5'); CHECK_EQ_I(b[1], 0);
}
TC_CASE(plain_l)
{
    wchar_t b[16]; FARPROC p = tc_crt_proc("_swprintf_l"); int n;
    if (!p) p = tc_crt_proc("__swprintf_l");
    CHECK(p != NULL); if (!p) return;
    n = ((legacy_t)p)(b, L"%d", NULL, 5);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], L'5'); CHECK_EQ_I(b[1], 0);
}
int main(void) { tc_begin(); TC_RUN(count_c_l); TC_RUN(plain_l); return tc_end(); }
