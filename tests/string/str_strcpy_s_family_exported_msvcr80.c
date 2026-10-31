/* TC
 * id:    str_strcpy_s_family_exported_msvcr80
 * area:  string
 * funcs:
 * procs: strcpy_s strncpy_s strcat_s
 * crt:   msvcr80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strcpy_s, strncpy_s, strcat_s (Visual Studio 2005 introduced the secure CRT)
 * note:  PROBE: msvcr80.dll (VS2005) exports strcpy_s, strncpy_s and strcat_s even though mingw's libmsvcr80.a lacks them.
 *        Assertions only that GetProcAddress finds them, then a basic copy through the DLL pointer.
 */
#include "tc.h"

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));

TC_CASE(exported) { CHECK(tc_p_strcpy_s != NULL); CHECK(tc_p_strncpy_s != NULL); CHECK(tc_p_strcat_s != NULL); }
TC_CASE(basic_use)
{
    char b[16];
    if (!tc_p_strcpy_s || !tc_p_strncpy_s || !tc_p_strcat_s) TC_SKIP("exports missing");
    CHECK_EQ_I(tc_p_strcpy_s(b, sizeof b, "ab"), 0); CHECK_STR(b, "ab");
    CHECK_EQ_I(tc_p_strcat_s(b, sizeof b, "cd"), 0); CHECK_STR(b, "abcd");
    CHECK_EQ_I(tc_p_strncpy_s(b, sizeof b, "xyz123", 3), 0); CHECK_STR(b, "xyz");
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strncpy_s); TC_LOAD_PROC(strcat_s);
    TC_RUN(exported); TC_RUN(basic_use);
    return tc_end();
}
