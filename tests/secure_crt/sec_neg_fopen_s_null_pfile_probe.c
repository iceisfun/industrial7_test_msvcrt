/* TC
 * id:    sec_neg_fopen_s_null_pfile_probe
 * area:  secure_crt
 * funcs: fopen_s _wfopen_s
 * procs:
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fopen_s
 * note:  PROBE: whether *pFile is set to NULL when filename or mode is NULL (EINVAL). Believed yes (CRT writes *pFile = NULL before validating the rest). The MS docs
 *        error table for fopen_s may instead say the contents of pFile are unchanged - exactly what real Windows does is the question.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
TC_CASE(null_filename_pfile_null) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = NULL; ini(); fopen_s(&f, (const char *)n, "r"); CHECK(f == NULL); }
TC_CASE(null_mode_pfile_null) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile m = NULL; ini(); fopen_s(&f, "tc_sec_x.tmp", (const char *)m); CHECK(f == NULL); }
TC_CASE(wnull_filename_pfile_null) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile n = NULL; ini(); _wfopen_s(&f, (const wchar_t *)n, L"r"); CHECK(f == NULL); }
TC_CASE(wnull_mode_pfile_null) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile m = NULL; ini(); _wfopen_s(&f, L"tc_sec_x.tmp", (const wchar_t *)m); CHECK(f == NULL); }
int main(void) { tc_begin(); TC_RUN(null_filename_pfile_null); TC_RUN(null_mode_pfile_null); TC_RUN(wnull_filename_pfile_null); TC_RUN(wnull_mode_pfile_null); return tc_end(); }
