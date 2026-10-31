/* TC
 * id:    pfr_neg_snprintf_s_default_handler_trunc_nonfatal
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _snprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s (_TRUNCATE truncates, returns -1, no handler; count > size is a parameter error)
 * note:  Child-process test with the default handler.
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *snprintf_s_t)(char *, size_t, size_t, const char *, ...);
static snprintf_s_t p_snprintf_s;
#if TC_CRT_VER >= 140
#define SNPF_S _snprintf_s  /* inline wrapper in ucrt headers */
#else
#define SNPF_S p_snprintf_s
#endif
#define TC_TRUNC ((size_t)-1)

TC_CASE(truncate_survives)  { DWORD c = 0; CHECK(tc_spawn_self("trunc", &c, 20000)); CHECK_EQ_U(c, 77); }
TC_CASE(count_gt_size_dies) { DWORD c = 0; CHECK(tc_spawn_self("big", &c, 20000)); CHECK(c != 77); CHECK(c != 0); }

int main(void)
{
    const char *c;
    tc_begin();
    p_snprintf_s = (snprintf_s_t)tc_crt_proc("_snprintf_s");
#if TC_CRT_VER < 140
    if (!p_snprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc _snprintf_s missing\n"); return 1; }
#endif
    if ((c = tc_child_name()) != NULL) {
        char b[16]; int r;
        if (c[0] == 't') {
            r = SNPF_S(b, 4, TC_TRUNC, "%s", "abcdef");
            return (r == -1 && b[0] == 'a' && b[3] == 0) ? 77 : 78;
        }
        SNPF_S(b, 4, 10, "%s", "abcdef");
        return 77;
    }
    TC_RUN(truncate_survives); TC_RUN(count_gt_size_dies);
    return tc_end();
}
