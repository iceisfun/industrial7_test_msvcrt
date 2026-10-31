/* TC
 * id:    ucr_ctype_tables
 * area:  ucrt_only
 * funcs: ___mb_cur_max_func
 * procs: __pctype_func __pwctype_func
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isalpha/isupper etc., ctype.h _UPPER/_LOWER/_DIGIT/_SPACE; MS docs MB_CUR_MAX
 * note:  C locale; index -1 (EOF) is valid for the narrow table.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(const unsigned short *, __pctype_func, (void));
TC_DECL_PROC(const unsigned short *, __pwctype_func, (void));
#define __pctype_func tc_p___pctype_func
#define __pwctype_func tc_p___pwctype_func

#define U_ 0x1
#define L_ 0x2
#define D_ 0x4
#define S_ 0x8

TC_CASE(upper) { const unsigned short *t = __pctype_func(); CHECK(t != NULL); if (t) { CHECK(t['A'] & U_); CHECK(!(t['A'] & L_)); CHECK(t['A'] & (U_ | L_ | 0x100));  /* isalpha mask: _UPPER|_LOWER|_ALPHA */ } }
TC_CASE(lower) { const unsigned short *t = __pctype_func(); if (t) { CHECK(t['a'] & L_); CHECK(!(t['a'] & U_)); } }
TC_CASE(digit) { const unsigned short *t = __pctype_func(); if (t) { CHECK(t['0'] & D_); CHECK(!(t['0'] & 0x100)); CHECK(!(t['a'] & D_)); } }
TC_CASE(space_and_eof)
{
    const unsigned short *t = __pctype_func();
    if (!t) return;
    CHECK(t[' '] & S_); CHECK(t['\t'] & S_); CHECK(!(t['x'] & S_));
    CHECK_EQ_U(t[-1], 0);   /* EOF index is valid and classifies as nothing */
}
TC_CASE(wide_table)
{
    const unsigned short *w = __pwctype_func();
    CHECK(w != NULL);
    if (w) { CHECK(w[L'A'] & U_); CHECK(w[L'a'] & L_); CHECK(w[L'7'] & D_); CHECK(w[L' '] & S_); }
}
TC_CASE(mb_cur_max) { CHECK_EQ_I(___mb_cur_max_func(), 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(__pctype_func); TC_LOAD_PROC(__pwctype_func); if (!tc_p___pctype_func || !tc_p___pwctype_func) { tc_write("TC FAIL missing proc\n"); return 1; } TC_RUN(upper); TC_RUN(lower); TC_RUN(digit); TC_RUN(space_and_eof); TC_RUN(wide_table); TC_RUN(mb_cur_max); return tc_end(); }
