/* TC
 * id:    pfv_neg_fprintf_null_stream
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf: stream or format NULL -> invalid parameter handler, returns -1, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static FILE *volatile nullfp = NULL;
static const char *volatile nullfmt = NULL;
TC_CASE(null_stream) { int n; errno = 0; tc_iph_count = 0; n = fprintf(nullfp, "abc"); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { FILE *f = fopen("tc_pfv_nf_a.tmp", "w"); int n; CHECK(f != NULL); if (!f) return; errno = 0; tc_iph_count = 0; n = fprintf(f, nullfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); fclose(f); remove("tc_pfv_nf_a.tmp"); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_stream); TC_RUN(null_format); return tc_end(); }
