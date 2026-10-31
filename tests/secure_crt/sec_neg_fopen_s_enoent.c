/* TC
 * id:    sec_neg_fopen_s_enoent
 * area:  secure_crt
 * funcs: fopen_s _wfopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fopen_s: returns errno value (ENOENT) when the file cannot be opened; not an invalid-parameter condition
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
#define MISSING "tc_sec_neg_nodir\\no_such_file.tmp"
TC_CASE(read_missing_file_ENOENT) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = "tc_sec_neg_missing.tmp"; ini(); CHECK_EQ_I(fopen_s(&f, (const char *)n, "r"), ENOENT); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(pfile_null_after_failure) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = "tc_sec_neg_missing.tmp"; ini(); fopen_s(&f, (const char *)n, "rb"); CHECK(f == NULL); }
TC_CASE(handler_not_called) { FILE *f; const char *volatile n = "tc_sec_neg_missing.tmp"; ini(); fopen_s(&f, (const char *)n, "r"); fopen_s(&f, MISSING, "w"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(write_in_missing_directory_ENOENT) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = MISSING; ini(); CHECK_EQ_I(fopen_s(&f, (const char *)n, "w"), ENOENT); CHECK(f == NULL); }
TC_CASE(wfopen_s_missing_ENOENT) { FILE *f = (FILE *)(ULONG_PTR)0x1; const wchar_t *volatile n = L"tc_sec_neg_missing.tmp"; ini(); CHECK_EQ_I(_wfopen_s(&f, (const wchar_t *)n, L"r"), ENOENT); CHECK(f == NULL); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(r_plus_missing_ENOENT) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = "tc_sec_neg_missing.tmp"; ini(); CHECK_EQ_I(fopen_s(&f, (const char *)n, "r+"), ENOENT); CHECK(f == NULL); }
int main(void) { tc_begin(); TC_RUN(read_missing_file_ENOENT); TC_RUN(pfile_null_after_failure); TC_RUN(handler_not_called); TC_RUN(write_in_missing_directory_ENOENT); TC_RUN(wfopen_s_missing_ENOENT); TC_RUN(r_plus_missing_ENOENT); return tc_end(); }
