/* TC
 * id:    ct_neg_probe_setlocale_malformed_codepage
 * area:  ctype
 * funcs: setlocale tolower
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs setlocale (locale name syntax locale[.code_page])
 * note:  PROBE: malformed code page suffixes ".abc", ".-1" fail with NULL and leave the C ctype state intact.
 *        Lone "." and "C." are not asserted here (see ct_neg_probe_setlocale_lone_dot).
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
TC_CASE(dot_abc) { one(".abc"); }
TC_CASE(dot_neg1) { one(".-1"); }
int main(void) { tc_begin(); TC_RUN(dot_abc); TC_RUN(dot_neg1); setlocale(LC_ALL, "C"); return tc_end(); }
