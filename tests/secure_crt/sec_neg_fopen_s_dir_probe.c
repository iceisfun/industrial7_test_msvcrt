/* TC
 * id:    sec_neg_fopen_s_dir_probe
 * area:  secure_crt
 * funcs: fopen_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   opening a directory for writing
 * note:  PROBE: fopen_s(".", "w") believed to fail with EACCES (13); older CRTs might differ.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(open_dot_w_fails_EACCES) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = "."; tc_iph_install(); errno = 0; CHECK_EQ_I(fopen_s(&f, (const char *)n, "w"), EACCES); }
TC_CASE(pfile_null) { FILE *f = (FILE *)(ULONG_PTR)0x1; const char *volatile n = "."; tc_iph_install(); fopen_s(&f, (const char *)n, "w"); CHECK(f == NULL); }
int main(void) { tc_begin(); TC_RUN(open_dot_w_fails_EACCES); TC_RUN(pfile_null); return tc_end(); }
