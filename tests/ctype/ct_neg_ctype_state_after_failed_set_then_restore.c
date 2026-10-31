/* TC
 * id:    ct_neg_ctype_state_after_failed_set_then_restore
 * area:  ctype
 * funcs: setlocale isleadbyte ___mb_cur_max_func isalpha isprint
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs setlocale, isleadbyte; C11 7.4.1.2 (C locale: only ASCII letters)
 * note:  After switching to cp932 and back to "C", no stale lead-byte or katakana classification may remain.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static int have;
TC_CASE(switch_932)
{
    volatile int c = 0x81;
    have = setlocale(LC_ALL, ".932") != NULL;
    if (!have) TC_SKIP("cp932 unavailable");
    CHECK(isleadbyte(c) != 0);
}
TC_CASE(back_to_C)
{
    volatile int a = 0x81, b = 0xE0, k = 0xB1;
    if (!have) TC_SKIP("no cp932");
    CHECK_STR(setlocale(LC_ALL, "C"), "C");
    CHECK(isleadbyte(a) == 0);
    CHECK(isleadbyte(b) == 0);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    CHECK(isalpha(k) == 0);
    CHECK(isprint(k) == 0);
}
int main(void) { tc_begin(); TC_RUN(switch_932); TC_RUN(back_to_C); setlocale(LC_ALL, "C"); return tc_end(); }
