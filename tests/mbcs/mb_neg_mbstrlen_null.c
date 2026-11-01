/* TC
 * id:    mb_neg_mbstrlen_null
 * area:  mbcs
 * funcs: _mbstrlen
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbstrlen (invalid parameter handler for NULL)
 * note:  PROBE: the errno value (EINVAL) after _mbstrlen(NULL); docs only say NULL is an invalid parameter. Return value (size_t)-1 is the documented error return.
 *        Observed Win10 22H2 ucrtbase: in the C locale (mb_cur_max==1, the strlen path) _mbstrlen(NULL) is NOT validated and faults
 *        with 0xC0000005 (default or installed handler) -- class C: run in a child and the fault asserted. _setmbcp() does NOT change this
 *        (it does not select the multibyte path). Under setlocale(LC_ALL, ".932") (multibyte path) _mbstrlen(NULL) does not fault:
 *        it returns (size_t)-1, errno == EILSEQ and the invalid-parameter handler is NOT called (observed, child case null_arg_mb_locale). Stable.
 */
#include "tc.h"
#include <errno.h>
#include <mbstring.h>
#include <stdlib.h>
#include <locale.h>

TC_CASE(null_arg_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("nullarg", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);   /* observed: no NULL validation */
}
TC_CASE(null_arg_mb_locale_einval_path)
{
    DWORD code = 0; int ran = tc_spawn_self("nullmb", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0x5A);   /* child: r==(size_t)-1, errno==EILSEQ, tc_iph_count==0 */
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        const char *volatile p = NULL;
        if (!tc_iph_install()) return 78;
        if (tc_strequ(tc_child_name(), "nullmb")) {
            size_t r; if (!setlocale(LC_ALL, ".932")) return 90;
            tc_iph_count = 0; errno = 0; r = _mbstrlen(p);
            return (r == (size_t)-1 && errno == EILSEQ && tc_iph_count == 0) ? 0x5A : 0x5B;
        }
        (void)_mbstrlen(p);
        return 0;   /* only reached when the call did not fault */
    }
    TC_RUN(null_arg_access_violation); TC_RUN(null_arg_mb_locale_einval_path);
    return tc_end();
}
