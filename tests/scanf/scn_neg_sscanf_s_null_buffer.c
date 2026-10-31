/* TC
 * id:    scn_neg_sscanf_s_null_buffer
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs scanf_s, wscanf_s: NULL destination for %s/%c/%[ invokes the invalid parameter handler
 * note:  conf inferred: docs do not explicitly state NULL destination handling for sscanf_s; handler-once is expected from CRT parameter validation.
 * note:  Only the handler invocation count is asserted; return value and errno are in scn_neg_sscanf_s_null_buffer_ret_probe.
 */
#include "tc.h"
#include <stdio.h>

static char *volatile vdst;
static volatile unsigned vsz;

TC_CASE(s_null)    { vdst = NULL; vsz = 8; CHECK(tc_iph_install()); tc_iph_count = 0; sscanf_s("abc", "%s", vdst, vsz); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(c_null)    { vdst = NULL; vsz = 1; CHECK(tc_iph_install()); tc_iph_count = 0; sscanf_s("a", "%c", vdst, vsz); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(set_null)  { vdst = NULL; vsz = 8; CHECK(tc_iph_install()); tc_iph_count = 0; sscanf_s("abc", "%[a-z]", vdst, vsz); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_control) { char b[8]; vdst = b; vsz = 8; CHECK(tc_iph_install()); tc_iph_count = 0; CHECK_EQ_I(sscanf_s("abc", "%s", vdst, vsz), 1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void) { tc_begin(); TC_RUN(s_null); TC_RUN(c_null); TC_RUN(set_null); TC_RUN(valid_control); return tc_end(); }
