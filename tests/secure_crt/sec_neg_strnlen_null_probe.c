/* TC
 * id:    sec_neg_strnlen_null_probe
 * area:  secure_crt
 * funcs:
 * procs: strnlen wcsnlen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strnlen, strnlen_s, wcsnlen ("strnlen_s returns 0 if str is NULL"; strnlen itself documented only for valid input)
 * note:  PROBE: what the plain exports strnlen(NULL,5) / wcsnlen(NULL,5) do. The documented NULL->0 promise is for
 *        strnlen_s/wcsnlen_s, which the headers implement as inline wrappers (str == NULL ? 0 : strnlen(...)); that wrapper
 *        suggests the exports themselves do NOT check and fault (most likely: access violation 0xC0000005, no
 *        invalid-parameter handler). Each call runs in a child; asserted outcome = access violation. If real Windows
 *        returns 0 instead, flip this probe (tests/string/str_neg_strnlen_null_probe asserts the opposite for ucrt).
 */
#include "tc.h"
#include <errno.h>

typedef size_t (__cdecl *snl_t)(const char *, size_t);
typedef size_t (__cdecl *wnl_t)(const wchar_t *, size_t);
static snl_t p_strnlen;
static wnl_t p_wcsnlen;
static const char *volatile v_n;
static const wchar_t *volatile v_wn;

/* NULL calls run in a child so a crash cannot take down the parent; child exits 10 = returned 0 with no handler call, 11 = other */
static int child_main(const char *n)
{
    size_t r; tc_iph_count = 0;
    if (tc_strequ(n, "strnlen")) { if (!p_strnlen) return 12; r = p_strnlen(v_n, 5); }
    else { if (!p_wcsnlen) return 12; r = p_wcsnlen(v_wn, 5); }
    return (r == 0 && tc_iph_count == 0) ? 10 : 11;
}
static void probe(const char *n) { DWORD code = 0; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(strnlen_null_probe) { probe("strnlen"); }
TC_CASE(wcsnlen_null_probe) { probe("wcsnlen"); }

int main(void) { const char *c; tc_begin(); tc_iph_install();
    p_strnlen = (snl_t)tc_crt_proc("strnlen"); p_wcsnlen = (wnl_t)tc_crt_proc("wcsnlen");
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(strnlen_null_probe); TC_RUN(wcsnlen_null_probe); return tc_end(); }
