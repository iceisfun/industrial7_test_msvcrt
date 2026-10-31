/* TC
 * id:    pfv_neg_fprintf_readonly_stream
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs fprintf: returns a negative value on output error; CRT write on a read-only stream sets _IOERR and errno EBADF
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static FILE *open_ro(void)
{
    FILE *f = fopen("tc_pfv_ro_a.tmp", "w"); if (!f) return NULL; fclose(f);
    return fopen("tc_pfv_ro_a.tmp", "r");
}
TC_CASE(ret_negative) { FILE *f = open_ro(); int n; CHECK(f != NULL); if (!f) return; n = fprintf(f, "hello"); CHECK(n < 0); fclose(f); remove("tc_pfv_ro_a.tmp"); }
TC_CASE(errno_ebadf) { FILE *f = open_ro(); CHECK(f != NULL); if (!f) return; errno = 0; fprintf(f, "hello"); CHECK_EQ_I(errno, EBADF); fclose(f); remove("tc_pfv_ro_a.tmp"); }
TC_CASE(ferror_set) { FILE *f = open_ro(); CHECK(f != NULL); if (!f) return; fprintf(f, "hello"); CHECK(ferror(f) != 0); fclose(f); remove("tc_pfv_ro_a.tmp"); }
int main(void) { tc_begin(); TC_RUN(ret_negative); TC_RUN(errno_ebadf); TC_RUN(ferror_set); return tc_end(); }
