/* TC
 * id:    str_msvcrt_win7_neg_strcpy_s_family
 * area:  string
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s strncat_s _set_invalid_parameter_handler
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s, strcat_s, strncpy_s, strncat_s
 * note:  resolved via GetProcAddress because mingw import libs for old CRTs are incomplete.
 */
#include "tc.h"
#include <errno.h>

#define TRUNC ((size_t)-1)
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, strncat_s, (char *, size_t, const char *, size_t));

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(cpy_null_dest)
{
    int r; if (!tc_p_strcpy_s) TC_SKIP("missing");
    setup(); r = tc_p_strcpy_s(NULL, 8, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cpy_too_small)
{
    char b[4] = "xyz"; int r; if (!tc_p_strcpy_s) TC_SKIP("missing");
    setup(); r = tc_p_strcpy_s(b, 2, "ab");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(ncpy_truncate)
{
    char b[8] = "zzzzzzz"; int r; if (!tc_p_strncpy_s) TC_SKIP("missing");
    setup(); r = tc_p_strncpy_s(b, 3, "abcdef", TRUNC);
    CHECK_EQ_I(r, 80); CHECK_STR(b, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(ncat_truncate)
{
    char b[8] = "abc"; int r; if (!tc_p_strncat_s) TC_SKIP("missing");
    setup(); r = tc_p_strncat_s(b, 5, "defgh", TRUNC);
    CHECK_EQ_I(r, 80); CHECK_STR(b, "abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strncat_s);
    TC_RUN(cpy_null_dest); TC_RUN(cpy_too_small); TC_RUN(ncpy_truncate); TC_RUN(ncat_truncate);
    return tc_end();
}
