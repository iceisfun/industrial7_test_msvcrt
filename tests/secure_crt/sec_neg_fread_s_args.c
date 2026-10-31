/* TC
 * id:    sec_neg_fread_s_args
 * area:  secure_crt
 * funcs: fread_s fopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fread_s: EINVAL for NULL buffer/stream; size or count 0 returns 0 (ERANGE case moved to sec_neg_fread_s_range_probe)
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_sec_neg_fread" TC_S1(TC_CRT_VER) ".tmp"
static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
static FILE *mk(void) { FILE *f = NULL; fopen_s(&f, FN, "wb+"); if (f) { fwrite("0123456789abcdef", 1, 16, f); rewind(f); } return f; }
TC_CASE(null_buffer_nonzero_count_EINVAL_returns_0) { FILE *f = mk(); void *volatile b = NULL; ini(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread_s((void *)b, 8, 4, 2, f), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); fclose(f); remove(FN); }
TC_CASE(null_stream_EINVAL_returns_0) { char b[8]; FILE *volatile f = NULL; ini(); CHECK_EQ_U(fread_s(b, 8, 1, 4, (FILE *)f), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero_returns_0) { char b[8]; FILE *f = mk(); volatile size_t z = 0; ini(); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread_s(b, 8, z, 4, f), 0); CHECK_EQ_I(tc_iph_count, 0); fclose(f); remove(FN); }
TC_CASE(handler_counts) { char b[8]; FILE *f = mk(); void *volatile nb = NULL; FILE *volatile nf = NULL; ini(); CHECK(f != NULL); if (!f) return;
    fread_s((void *)nb, 8, 4, 2, f); fread_s(b, 8, 1, 4, (FILE *)nf);
    CHECK_EQ_I(tc_iph_count, 2);
    CHECK_EQ_U(fread_s(b, 8, 1, 4, f), 4); CHECK_EQ_I(tc_iph_count, 2); fclose(f); remove(FN); }
int main(void) { tc_begin(); TC_RUN(null_buffer_nonzero_count_EINVAL_returns_0); TC_RUN(null_stream_EINVAL_returns_0); TC_RUN(size_zero_returns_0); TC_RUN(handler_counts); return tc_end(); }
