/* TC
 * id:    sec_neg_freopen_s_args
 * area:  secure_crt
 * funcs: freopen_s fopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs freopen_s, _wfreopen_s
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_sec_neg_freopen" TC_S1(TC_CRT_VER) ".tmp"
static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
TC_CASE(null_pfile_EINVAL) { FILE *s = NULL; FILE **volatile pp = NULL; ini(); fopen_s(&s, FN, "w"); CHECK(s != NULL); if (!s) return;
    CHECK_EQ_I(freopen_s((FILE **)pp, FN, "r", s), EINVAL); CHECK_EQ_I(tc_iph_count, 1); fclose(s); remove(FN); }
TC_CASE(null_stream_EINVAL) { FILE *f = NULL; FILE *volatile s = NULL; ini(); CHECK_EQ_I(freopen_s(&f, FN, "r", (FILE *)s), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_mode_EINVAL) { FILE *s = NULL, *f = NULL; const char *volatile m = NULL; ini(); fopen_s(&s, FN, "w"); CHECK(s != NULL); if (!s) return;
    CHECK_EQ_I(freopen_s(&f, FN, (const char *)m, s), EINVAL); CHECK_EQ_I(tc_iph_count, 1); fclose(s); remove(FN); }
TC_CASE(missing_file_ENOENT_no_handler) { FILE *s = NULL, *f = (FILE *)(ULONG_PTR)0x1; ini(); fopen_s(&s, FN, "w"); CHECK(s != NULL); if (!s) return; fclose(s); remove(FN);
    fopen_s(&s, "tc_sec_neg_freopen_o" TC_S1(TC_CRT_VER) ".tmp", "w"); CHECK(s != NULL); if (!s) return;
    CHECK_EQ_I(freopen_s(&f, FN, "r", s), ENOENT); CHECK(f == NULL); CHECK_EQ_I(tc_iph_count, 0); remove("tc_sec_neg_freopen_o" TC_S1(TC_CRT_VER) ".tmp"); }
int main(void) { tc_begin(); TC_RUN(null_pfile_EINVAL); TC_RUN(null_stream_EINVAL); TC_RUN(null_mode_EINVAL); TC_RUN(missing_file_ENOENT_no_handler); return tc_end(); }
