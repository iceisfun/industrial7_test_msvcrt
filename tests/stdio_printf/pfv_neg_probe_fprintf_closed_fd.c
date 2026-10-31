/* TC
 * id:    pfv_neg_probe_fprintf_closed_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _close / fprintf (no documented statement for this combination)
 * note:  PROBING: fprintf to an unbuffered FILE whose fd was _close()d behind its back. Guess: negative return, errno EBADF, ferror set; invalid-parameter count recorded as observation, expected 1 on UCRT.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

static FILE *mk(void)
{
    FILE *f = fopen("tc_pfv_cfd.tmp", "w");
    if (!f) return NULL;
    setvbuf(f, NULL, _IONBF, 0);
    _close(_fileno(f));
    return f;
}
static void done(FILE *f) { fclose(f); remove("tc_pfv_cfd.tmp"); }
TC_CASE(ret_negative) { FILE *f = mk(); int n; CHECK(f != NULL); if (!f) return; n = fprintf(f, "x"); CHECK(n < 0); done(f); }
TC_CASE(errno_ebadf) { FILE *f = mk(); CHECK(f != NULL); if (!f) return; errno = 0; fprintf(f, "x"); CHECK_EQ_I(errno, EBADF); done(f); }
TC_CASE(ferror_set) { FILE *f = mk(); CHECK(f != NULL); if (!f) return; fprintf(f, "x"); CHECK(ferror(f) != 0); done(f); }
TC_CASE(iph_count)
{
    FILE *f = mk(); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; fprintf(f, "x");
#ifdef TC_UCRT
    CHECK_EQ_I(tc_iph_count, 1);
#else
    CHECK(tc_iph_count == 0 || tc_iph_count == 1);
#endif
    done(f);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(ret_negative); TC_RUN(errno_ebadf); TC_RUN(ferror_set); TC_RUN(iph_count); return tc_end(); }
