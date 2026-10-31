/* TC
 * id:    mb_neg_probe_s_family_dst_cleared
 * area:  mbcs
 * funcs: setlocale
 * procs: mbstowcs_s wcstombs_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs mbstowcs_s, wcstombs_s
 * note:  PROBE: after ERANGE (buffer too small) and after EILSEQ the destination buffer is set to an empty string (dst[0] == 0), as with the other *_s functions.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_DECL_PROC(errno_t, mbstowcs_s, (size_t *, wchar_t *, size_t, const char *, size_t));
TC_DECL_PROC(errno_t, wcstombs_s, (size_t *, char *, size_t, const wchar_t *, size_t));

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}
TC_CASE(mbstowcs_s_erange_cleared)
{
    wchar_t w[4]; size_t rv = 99; const char *volatile s = "abc";
    w[0] = 0x7777;
    tc_p_mbstowcs_s(&rv, w, 2, s, 3);
    CHECK_EQ_U(w[0], 0);
}
TC_CASE(mbstowcs_s_eilseq_cleared)
{
    wchar_t w[8]; size_t rv = 99; const char s[] = { (char)0x82, 0x20, 0 };
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    w[0] = 0x7777;
    tc_p_mbstowcs_s(&rv, w, 8, s, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(w[0], 0);
}
TC_CASE(wcstombs_s_erange_cleared)
{
    char b[8]; size_t rv = 99; const wchar_t *volatile s = L"abc";
    b[0] = 0x55;
    tc_p_wcstombs_s(&rv, b, 3, s, 3);
    CHECK_EQ_U(b[0], 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbstowcs_s); TC_LOAD_PROC(wcstombs_s);
    CHECK(tc_p_mbstowcs_s); CHECK(tc_p_wcstombs_s);
    if (!tc_p_mbstowcs_s || !tc_p_wcstombs_s) return tc_end();
    tc_iph_install();
    TC_RUN(mbstowcs_s_erange_cleared); TC_RUN(mbstowcs_s_eilseq_cleared); TC_RUN(wcstombs_s_erange_cleared);
    return tc_end();
}
