/* TC
 * id:    str_strnlen_dynamic
 * area:  string
 * funcs:
 * procs: strnlen
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strnlen, strnlen_s (VS2005+; exported by msvcr100+ and ucrtbase)
 * note:  Example of the `procs` pattern: mingw's import library for some CRTs lacks the symbol (or supplies a static copy), so resolve it from the DLL at run time to be sure the CRT under test is what runs.
 */
#include "tc.h"

typedef size_t (__cdecl *strnlen_t)(const char *, size_t);
static strnlen_t p_strnlen;

TC_CASE(exported) { CHECK(p_strnlen != NULL); }
TC_CASE(bounded)
{
    static const char s[] = "abcdef";
    if (!p_strnlen) TC_SKIP("strnlen not exported");
    CHECK_EQ_U(p_strnlen(s, 3), 3);
    CHECK_EQ_U(p_strnlen(s, 100), 6);
    CHECK_EQ_U(p_strnlen(s, 0), 0);
}
int main(void)
{
    tc_begin();
    p_strnlen = (strnlen_t)tc_crt_proc("strnlen");
    TC_RUN(exported); TC_RUN(bounded);
    return tc_end();
}
