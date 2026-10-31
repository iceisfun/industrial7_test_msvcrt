/* TC
 * id:    sec_neg_default_handler_fatal_strn_wide
 * area:  secure_crt
 * funcs: strncpy_s strncat_s memmove_s wcscpy_s wcscat_s wcsncpy_s _mbscpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation / _set_invalid_parameter_handler: default handler terminates the process
 * note:  Each failing call runs in a child with the default handler; the child must not return normally (exit != 77, != 0).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <mbstring.h>

static volatile size_t v0 = 0, v3 = 3, v8 = 8;
static const wchar_t *volatile v_nws;
static char *volatile v_nd;
static unsigned char *volatile v_nmd;
static int child_main(const char *n)
{
    char d[8] = "ab"; wchar_t w[8] = L"ab"; unsigned char m[8] = "ab";
    if (tc_strequ(n, "strncpy_s")) strncpy_s(v_nd, v8, "abc", 3);
    else if (tc_strequ(n, "strncat_s")) strncat_s(d, v3, "abcdef", 6);
    else if (tc_strequ(n, "memmove_s")) memmove_s(d, v3, "0123456789", 8);
    else if (tc_strequ(n, "wcscpy_s")) wcscpy_s(w, v3, L"abcdef");
    else if (tc_strequ(n, "wcscat_s")) wcscat_s(w, v8, v_nws);
    else if (tc_strequ(n, "wcsncpy_s")) wcsncpy_s(w, v0, L"abc", 3);
    else if (tc_strequ(n, "mbscpy_s")) _mbscpy_s(v_nmd, v8, (const unsigned char *)"abc");
    (void)m;
    return 77;
}
static void term(const char *n)
{
    DWORD code = 77; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
    CHECK(code != TC_STATUS_ACCESS_VIOLATION); /* a crash from a test bug must not count as the handler terminating */
}
TC_CASE(child_strncpy_s_null_dest_terminates) { term("strncpy_s"); }
TC_CASE(child_strncat_s_erange_terminates) { term("strncat_s"); }
TC_CASE(child_memmove_s_erange_terminates) { term("memmove_s"); }
TC_CASE(child_wcscpy_s_erange_terminates) { term("wcscpy_s"); }
TC_CASE(child_wcscat_s_null_src_terminates) { term("wcscat_s"); }
TC_CASE(child_wcsncpy_s_size0_terminates) { term("wcsncpy_s"); }
TC_CASE(child_mbscpy_s_null_dest_terminates) { term("mbscpy_s"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(child_strncpy_s_null_dest_terminates); TC_RUN(child_strncat_s_erange_terminates);
    TC_RUN(child_memmove_s_erange_terminates); TC_RUN(child_wcscpy_s_erange_terminates);
    TC_RUN(child_wcscat_s_null_src_terminates); TC_RUN(child_wcsncpy_s_size0_terminates);
    TC_RUN(child_mbscpy_s_null_dest_terminates);
    return tc_end();
}
