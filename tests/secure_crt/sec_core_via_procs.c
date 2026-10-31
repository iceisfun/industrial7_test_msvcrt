/* TC
 * id:    sec_core_via_procs
 * area:  secure_crt
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s strncat_s strtok_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strcpy_s, strcat_s, strncpy_s, strncat_s, strtok_s
 * note:  Called via GetProcAddress so msvcr80 (missing from mingw import lib) is covered.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, strncat_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(char *, strtok_s, (char *, const char *, char **));

TC_CASE(strcpy_s_ok) { char d[8]; CHECK_EQ_I(tc_p_strcpy_s(d, 8, "abc"), 0); CHECK_STR(d, "abc"); }
TC_CASE(strcat_s_ok) { char d[8] = "ab"; CHECK_EQ_I(tc_p_strcat_s(d, 8, "cd"), 0); CHECK_STR(d, "abcd"); }
TC_CASE(strncpy_s_truncate_80) { char d[4]; CHECK_EQ_I(tc_p_strncpy_s(d, 4, "abcdef", (size_t)-1), 80); CHECK_STR(d, "abc"); }
TC_CASE(strncat_s_ok) { char d[8] = "ab"; CHECK_EQ_I(tc_p_strncat_s(d, 8, "cdef", 2), 0); CHECK_STR(d, "abcd"); }
TC_CASE(strtok_s_tokens)
{
    char s[] = "a,b;c", *ctx = NULL, *t;
    t = tc_p_strtok_s(s, ",;", &ctx); CHECK_STR(t, "a");
    t = tc_p_strtok_s(NULL, ",;", &ctx); CHECK_STR(t, "b");
    t = tc_p_strtok_s(NULL, ",;", &ctx); CHECK_STR(t, "c");
    t = tc_p_strtok_s(NULL, ",;", &ctx); CHECK(t == NULL);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strncat_s);
    TC_LOAD_PROC(strtok_s);
    if (!(tc_p_strcpy_s && tc_p_strcat_s && tc_p_strncpy_s && tc_p_strncat_s && tc_p_strtok_s)) { tc_write("TC FAIL " TC_TEST_ID "::load missing export\n"); return 1; }
    TC_RUN(strcpy_s_ok); TC_RUN(strcat_s_ok); TC_RUN(strncpy_s_truncate_80); TC_RUN(strncat_s_ok); TC_RUN(strtok_s_tokens);
    return tc_end();
}
