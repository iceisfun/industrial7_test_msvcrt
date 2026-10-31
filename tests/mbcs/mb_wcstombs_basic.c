/* TC
 * id:    mb_wcstombs_basic
 * area:  mbcs
 * funcs: wcstombs setlocale
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.8.2; MS docs wcstombs
 * note:  DBCS boundary behaviour (char not split when it does not fit) moved to mb_probe_wcstombs_dbcs_boundary.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

TC_CASE(dst_null)
{
    const wchar_t *volatile s = L"hello";
    CHECK_EQ_U(wcstombs(NULL, s, 0), 5);
}
TC_CASE(full)
{
    char b[16]; const wchar_t *volatile s = L"abc"; size_t n;
    b[3] = 0x55;
    n = wcstombs(b, s, 10);
    CHECK_EQ_U(n, 3); CHECK_STR(b, "abc");
}
TC_CASE(limited)
{
    char b[16]; const wchar_t *volatile s = L"abc"; size_t n;
    b[2] = 0x55;
    n = wcstombs(b, s, 2);
    CHECK_EQ_U(n, 2); CHECK_EQ_U((unsigned char)b[0], 'a'); CHECK_EQ_U((unsigned char)b[1], 'b'); CHECK_EQ_U((unsigned char)b[2], 0x55);
}
TC_CASE(cp932)
{
    char b[16]; const wchar_t s[] = { 0x3042, L'a', 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    n = wcstombs(b, s, 16);
    CHECK_EQ_U(n, 3); CHECK_EQ_U((unsigned char)b[0], 0x82); CHECK_EQ_U((unsigned char)b[1], 0xA0); CHECK_EQ_U((unsigned char)b[2], 0x61); CHECK_EQ_U(b[3], 0);
    CHECK_EQ_U(wcstombs(NULL, s, 0), 3);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(dst_null); TC_RUN(full); TC_RUN(limited); TC_RUN(cp932);
    return tc_end();
}
