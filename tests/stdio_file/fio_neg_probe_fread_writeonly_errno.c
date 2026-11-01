/* TC
 * id:    fio_neg_probe_fread_writeonly_errno
 * area:  stdio_file
 * kind:  negative
 * funcs: fread fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS _filbuf legacy behaviour
 * note:  PROBE: errno == EBADF after fread on a stream opened "w". Some CRT generations may leave errno untouched.
 *        TRIAGE win10, class A): fread on a stream opened "w" leaves errno untouched (0) on ucrtbase 10.0.19045 (observed got=0). EBADF kept for non-UCRT (unverified).
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>
#include <errno.h>
static const char * volatile fname = "tc_fio_neg_probe_fread_writeonly_errno_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(errno_ebadf)
{
    char buf[8]; FILE *fp;
    fp = fopen(fname, "w"); CHECK(fp != NULL); if (!fp) return;
    errno = 0;
    fread(buf, 1, 4, fp);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(errno, 0);                  /* observed ucrtbase 10.0.19045: errno untouched */
#else
    CHECK_EQ_I(errno, EBADF);
#endif
    fclose(fp);
}
int main(void)
{
    tc_begin();
    TC_RUN(errno_ebadf);
    DeleteFileA(fname);
    return tc_end();
}
