/* TC
 * id:    gct_iscsym_iscsymf_narrow
 * area:  ctype
 * funcs: 
 * procs: __iscsym __iscsymf iscsym iscsymf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs __iscsym, __iscsymf, iswcsym family
 */
#include "tc.h"
#include <ctype.h>
TC_DECL_PROC(int, __iscsym, (int));
TC_DECL_PROC(int, __iscsymf, (int));
TC_DECL_PROC(int, iscsym, (int));
TC_DECL_PROC(int, iscsymf, (int));
static volatile int v;
static int alpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int digit(int c) { return c >= '0' && c <= '9'; }

TC_CASE(loaded) { CHECK(tc_p___iscsym && tc_p___iscsymf); }
TC_CASE(symf) { int c; for (c = 0; c < 128; c++) { v = c; if ((tc_p___iscsymf(v) != 0) != (alpha(c) || c == '_')) { CHECK_EQ_I(c, -1); } } }
TC_CASE(sym)  { int c; for (c = 0; c < 128; c++) { v = c; if ((tc_p___iscsym(v) != 0) != (alpha(c) || digit(c) || c == '_')) { CHECK_EQ_I(c, -1); } } }
TC_CASE(undecorated_agree)
{
    int c; if (!tc_p_iscsym || !tc_p_iscsymf) TC_SKIP("undecorated alias not exported");
    for (c = 0; c < 128; c++) {
        v = c;
        if ((tc_p_iscsym(v) != 0) != (tc_p___iscsym(v) != 0)) { CHECK_EQ_I(c, -1); }
        if ((tc_p_iscsymf(v) != 0) != (tc_p___iscsymf(v) != 0)) { CHECK_EQ_I(c, -2); }
    }
}
TC_CASE(boundaries)
{
    static const int b[] = { '/', ':', '@', '[', '`', '{', '^' }; size_t i;
    for (i = 0; i < sizeof b / sizeof b[0]; i++) { v = b[i]; CHECK(!tc_p___iscsym(v)); CHECK(!tc_p___iscsymf(v)); }
    v = '_'; CHECK(tc_p___iscsym(v)); CHECK(tc_p___iscsymf(v));
    v = '0'; CHECK(tc_p___iscsym(v)); CHECK(!tc_p___iscsymf(v));
}
TC_CASE(eof) { v = -1; CHECK_EQ_I(tc_p___iscsym(v), 0); CHECK_EQ_I(tc_p___iscsymf(v), 0); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(__iscsym); TC_LOAD_PROC(__iscsymf); TC_LOAD_PROC(iscsym); TC_LOAD_PROC(iscsymf);
    TC_RUN(loaded); if (!tc_p___iscsym || !tc_p___iscsymf) return tc_end(); TC_RUN(symf); TC_RUN(sym); TC_RUN(undecorated_agree); TC_RUN(boundaries); TC_RUN(eof);
    return tc_end();
}
