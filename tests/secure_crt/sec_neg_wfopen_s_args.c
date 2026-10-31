/* TC
 * id:    sec_neg_wfopen_s_args
 * area:  secure_crt
 * funcs: _wfopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fopen_s, _wfopen_s
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
TC_CASE(null_pfile_EINVAL) { FILE **volatile pp = NULL; ini(); CHECK_EQ_I(_wfopen_s((FILE **)pp, L"tc_sec_x.tmp", L"r"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_filename_EINVAL) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile n = NULL; ini(); CHECK_EQ_I(_wfopen_s(&f, (const wchar_t *)n, L"r"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_mode_EINVAL) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile m = NULL; ini(); CHECK_EQ_I(_wfopen_s(&f, L"tc_sec_x.tmp", (const wchar_t *)m), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(invalid_mode_EINVAL) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile m = L"q"; ini(); CHECK_EQ_I(_wfopen_s(&f, L"tc_sec_x.tmp", (const wchar_t *)m), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(handler_counts) { FILE *f; const wchar_t *volatile n = NULL; ini(); _wfopen_s(&f, (const wchar_t *)n, L"r"); _wfopen_s(&f, L"tc_sec_x.tmp", NULL); CHECK_EQ_I(tc_iph_count, 2); }
int main(void) { tc_begin(); TC_RUN(null_pfile_EINVAL); TC_RUN(null_filename_EINVAL); TC_RUN(null_mode_EINVAL); TC_RUN(invalid_mode_EINVAL); TC_RUN(handler_counts); return tc_end(); }
