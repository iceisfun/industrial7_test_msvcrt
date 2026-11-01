/* TC
 * id:    pfw_neg_swprintf_p_small_buffer_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vswprintf_p
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _swprintf_p (error reporting on truncation / count 0)
 * note:  OBSERVED on ucrtbase for case errno_set (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "errno_set errno == ERANGE || errno == EINVAL"; zzp_probe2 "vswprintf_p opt4 small: r=-2
 *        errno=0 iph=0"): a too-small buffer in __stdio_common_vswprintf_p returns -2 and leaves errno untouched; the other two cases (negative
 *        return, untouched slots after count) were right. Old expectation kept for pre-UCRT CRTs (unverified). Stable.
 *        NOTE: -2 is the raw ucrtbase __stdio_common_v*printf_p export result (observed, not documented); MSVC headers map any negative result to -1.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vp(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf_p(4, b, n, f, NULL, ap); }
#else
typedef int (__cdecl *vp_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vp_t p_vp;
static int sh_vp(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vp ? p_vp(b, n, f, ap) : -99; }
#endif
static int swp(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vp(b, n, f, ap); va_end(ap); return r; }

TC_CASE(errno_set)
{
    wchar_t b[8]; int r; tc_iph_count = 0; errno = 0;
    r = swp(b, 4, L"%1$s%2$s", L"abcde", L"fghij");
    CHECK(r < 0);
#if TC_CRT_VER >= 140
    CHECK_EQ_I(errno, 0);   /* ucrtbase observed: errno untouched, r == -2 */
#else
    CHECK(errno == ERANGE || errno == EINVAL);
#endif
}
TC_CASE(no_room_for_nul)
{
    wchar_t b[8]; int r, i; for (i = 0; i < 8; i++) b[i] = L'Q';
    r = swp(b, 6, L"%2$s%1$s", L"ab", L"cdef");   /* 6 chars, count 6 */
    CHECK(r < 0); CHECK_EQ_I(b[6], L'Q'); CHECK_EQ_I(b[7], L'Q');
}
TC_CASE(count_zero)
{
    wchar_t a[10]; wchar_t *b = a + 1; int r, i; for (i = 0; i < 10; i++) a[i] = L'Q';   /* padded: b[count-1] must stay inside the array */
    r = swp(b, 0, L"%1$s", L"x");
    CHECK(r < 0); CHECK_EQ_I(b[0], L'Q'); CHECK_EQ_I(b[1], L'Q');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vp = (vp_t)tc_crt_proc("_vswprintf_p"); CHECK(p_vp != NULL);
#endif
    TC_RUN(errno_set); TC_RUN(no_room_for_nul); TC_RUN(count_zero); return tc_end();
}
