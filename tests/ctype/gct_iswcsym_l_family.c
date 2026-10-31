/* TC
 * id:    gct_iswcsym_l_family
 * area:  ctype
 * funcs: 
 * procs: __iswcsym __iswcsymf
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs __iswcsym, __iswcsymf
 * note:  Despite the id, only the non-_l forms are asserted here (export of _iswcsym_l in msvcr90-120 is unverified; see
 *        gct_iswcsym_l_forms_probe). msvcr80 and msvcrt export no __iswcsym per import libs; verify __iswcsym in msvcr90 on Windows.
 */
#include "tc.h"
TC_DECL_PROC(int, __iswcsym, (unsigned short));
TC_DECL_PROC(int, __iswcsymf, (unsigned short));
static int alpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int digit(int c) { return c >= '0' && c <= '9'; }

TC_CASE(loaded) { CHECK(tc_p___iswcsym && tc_p___iswcsymf); }
TC_CASE(csymf)
{
    int c; if (!tc_p___iswcsymf) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) { int want = alpha(c) || c == '_'; if ((tc_p___iswcsymf((unsigned short)c) != 0) != want) { CHECK_EQ_I(c, -1); } }
    CHECK(!tc_p___iswcsymf('$')); CHECK(!tc_p___iswcsymf('-')); CHECK(!tc_p___iswcsymf(' ')); CHECK(!tc_p___iswcsymf('7'));
}
TC_CASE(csym)
{
    int c; if (!tc_p___iswcsym) TC_SKIP("unavailable");
    for (c = 0; c < 128; c++) { int want = alpha(c) || digit(c) || c == '_'; if ((tc_p___iswcsym((unsigned short)c) != 0) != want) { CHECK_EQ_I(c, -1); } }
    CHECK(!tc_p___iswcsym('$')); CHECK(!tc_p___iswcsym('-')); CHECK(!tc_p___iswcsym(' ')); CHECK(!tc_p___iswcsym('.'));
}
TC_CASE(digit_boundary)
{
    int c; if (!tc_p___iswcsym || !tc_p___iswcsymf) TC_SKIP("unavailable");
    for (c = '0'; c <= '9'; c++) { CHECK(tc_p___iswcsym((unsigned short)c)); CHECK(!tc_p___iswcsymf((unsigned short)c)); }
}
TC_CASE(underscore_neighbours)
{
    if (!tc_p___iswcsym || !tc_p___iswcsymf) TC_SKIP("unavailable");
    CHECK(tc_p___iswcsym('_')); CHECK(tc_p___iswcsymf('_'));
    CHECK(!tc_p___iswcsym(0x5E)); CHECK(!tc_p___iswcsym(0x60)); CHECK(!tc_p___iswcsymf(0x5E)); CHECK(!tc_p___iswcsymf(0x60));
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__iswcsym); TC_LOAD_PROC(__iswcsymf);
    TC_RUN(loaded); TC_RUN(csymf); TC_RUN(csym); TC_RUN(digit_boundary); TC_RUN(underscore_neighbours);
    return tc_end();
}
