/* TC
 * id:    pfp_neg_fprintf_null_stream
 * area:  stdio_printf
 * funcs: fprintf fprintf_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf, fprintf_s, Parameter validation: NULL stream/format -> handler, -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <stdarg.h>

TC_CASE(fprintf_null_stream)
{
    FILE *volatile s = NULL; volatile void *p = (void *)(size_t)0xABC; int r;
    tc_iph_count = 0; errno = 0;
    r = fprintf(s, "%p", p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(fprintf_s_null_stream)
{
    FILE *volatile s = NULL; volatile void *p = (void *)(size_t)0xABC; int r;
    tc_iph_count = 0; errno = 0;
    r = fprintf_s(s, "%p", p);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(fprintf_null_format)
{
    const char *name = "tc_pfp_neg_fprintf_null_stream_1.tmp";
    FILE *f = fopen(name, "w"); const char *volatile fmt = NULL; int r;
    CHECK(f != NULL);
    if (!f) return;
    tc_iph_count = 0;
    r = fprintf(f, fmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(name);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(fprintf_null_stream); TC_RUN(fprintf_s_null_stream); TC_RUN(fprintf_null_format); return tc_end(); }
