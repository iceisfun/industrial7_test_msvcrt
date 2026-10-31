/* TC
 * id:    ct_neg_probe_setlocale_lone_dot
 * area:  ctype
 * funcs: setlocale tolower
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (locale name syntax locale[.code_page])
 * note:  PROBE (isolated guess): setlocale(LC_CTYPE, ".") - empty language and empty code page - fails with NULL and
 *        leaves the C ctype state intact. Real Windows might instead treat it as "system default" and succeed.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static void one(const char *nm)
{
    volatile int c = 0xC9;
    setlocale(LC_ALL, "C");
    CHECK(setlocale(LC_CTYPE, nm) == NULL);
    CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
    CHECK_EQ_I(tolower(c), 0xC9);
}
TC_CASE(dot) { one("."); }
int main(void) { tc_begin(); TC_RUN(dot); setlocale(LC_ALL, "C"); return tc_end(); }
