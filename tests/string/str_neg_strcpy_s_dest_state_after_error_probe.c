/* TC
 * id:    str_neg_strcpy_s_dest_state_after_error_probe
 * area:  string
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcpy_s/strcat_s/strncpy_s: on ERANGE the destination's first byte is set to 0 (documented cases 'documented_*'); contents of the remaining bytes are not documented
 * note:  PROBE (cases probe_*): in the release CRT the copy loop writes bytes until the buffer is exhausted and then only dest[0] is reset, so the remaining bytes hold the partially copied source (e.g. strcpy_s(b,3,"hello") leaves {0,'e','l'}). The DEBUG CRT instead fills the buffer with 0xFE; release is assumed here. Cases documented_* assert only ERANGE and dest[0]==0.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));
TC_DECL_PROC(int, strncpy_s, (char *, size_t, const char *, size_t));
static const char *volatile hello = "hello";
static const char *volatile cdef = "cdef";

TC_CASE(documented_strcpy_s) { char b[4]; memset(b, 'x', 4); tc_iph_count = 0; CHECK_EQ_I(tc_p_strcpy_s(b, 3, hello), ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[3], 'x'); }
TC_CASE(documented_strncpy_s) { char b[4]; memset(b, 'x', 4); tc_iph_count = 0; CHECK_EQ_I(tc_p_strncpy_s(b, 3, hello, 5), ERANGE); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[3], 'x'); }
TC_CASE(documented_strcat_s) { char b[8]; strcpy(b, "ab"); tc_iph_count = 0; CHECK_EQ_I(tc_p_strcat_s(b, 4, cdef), ERANGE); CHECK_EQ_I(b[0], 0); }
TC_CASE(probe_strcpy_s_partial) { char b[4]; memset(b, 'x', 4); tc_p_strcpy_s(b, 3, hello); CHECK_EQ_I(b[1], 'e'); CHECK_EQ_I(b[2], 'l'); CHECK_EQ_I(b[3], 'x'); }
TC_CASE(probe_strncpy_s_partial) { char b[4]; memset(b, 'x', 4); tc_p_strncpy_s(b, 3, hello, 5); CHECK_EQ_I(b[1], 'e'); CHECK_EQ_I(b[2], 'l'); CHECK_EQ_I(b[3], 'x'); }
TC_CASE(probe_strcat_s_partial) { char b[8]; strcpy(b, "ab"); tc_p_strcat_s(b, 4, cdef); CHECK_EQ_I(b[1], 'b'); CHECK_EQ_I(b[2], 'c'); CHECK_EQ_I(b[3], 'd'); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(strcpy_s); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncpy_s);
    if (!tc_p_strcpy_s || !tc_p_strcat_s || !tc_p_strncpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__import missing\n"); return 1; }
    TC_RUN(documented_strcpy_s); TC_RUN(documented_strncpy_s); TC_RUN(documented_strcat_s);
    TC_RUN(probe_strcpy_s_partial); TC_RUN(probe_strncpy_s_partial); TC_RUN(probe_strcat_s_partial);
    return tc_end();
}
