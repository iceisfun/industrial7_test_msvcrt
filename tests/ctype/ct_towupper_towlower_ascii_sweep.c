/* TC
 * id:    ct_towupper_towlower_ascii_sweep
 * area:  ctype
 * funcs: towupper towlower
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.3.1; MS docs towupper, towlower
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t v;

TC_CASE(towupper_a_z) { int c; for (c = 'a'; c <= 'z'; c++) { v = (wint_t)c; CHECK_EQ_I((towupper)(v), c - 32); } }
TC_CASE(towlower_A_Z) { int c; for (c = 'A'; c <= 'Z'; c++) { v = (wint_t)c; CHECK_EQ_I((towlower)(v), c + 32); } }
TC_CASE(other_ascii_unchanged)
{
    int c;
    for (c = 0; c < 128; c++) {
        v = (wint_t)c;
        if (!(c >= 'a' && c <= 'z')) CHECK_EQ_I((towupper)(v), c);
        if (!(c >= 'A' && c <= 'Z')) CHECK_EQ_I((towlower)(v), c);
    }
}

int main(void) { tc_begin(); TC_RUN(towupper_a_z); TC_RUN(towlower_A_Z); TC_RUN(other_ascii_unchanged); return tc_end(); }
