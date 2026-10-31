/* TC
 * id:    pfs_str_s_null_ptr
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf: null pointer prints (null)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile np = NULL;
TC_CASE(plain) { char b[64]; int n = sprintf(b, "%s", np); CHECK_STR(b, "(null)"); CHECK_EQ_I(n, 6); }
TC_CASE(right) { char b[64]; sprintf(b, "[%10s]", np); CHECK_STR(b, "[    (null)]"); }
TC_CASE(left) { char b[64]; sprintf(b, "[%-10s]", np); CHECK_STR(b, "[(null)    ]"); }
TC_CASE(around) { char b[64]; sprintf(b, "<%s>", np); CHECK_STR(b, "<(null)>"); }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(right); TC_RUN(left); TC_RUN(around); return tc_end(); }
