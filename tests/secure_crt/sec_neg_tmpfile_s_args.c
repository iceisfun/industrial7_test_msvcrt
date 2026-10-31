/* TC
 * id:    sec_neg_tmpfile_s_args
 * area:  secure_crt
 * funcs: tmpfile_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs tmpfile_s: EINVAL if pFilePtr is NULL
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_pfile_EINVAL) { FILE **volatile pp = NULL; tc_iph_install(); tc_iph_count = 0; errno = 0; CHECK_EQ_I(tmpfile_s((FILE **)pp), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(handler_count_one) { FILE **volatile pp = NULL; tc_iph_install(); tc_iph_count = 0; tmpfile_s((FILE **)pp); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(null_pfile_EINVAL); TC_RUN(handler_count_one); return tc_end(); }
