/* TC
 * id:    mb_neg_wcsrtombs_unmappable
 * area:  mbcs
 * funcs: 
 * procs: wcsrtombs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.6.4.2 (EILSEQ); MS docs wcsrtombs
 * note:  src-on-error and NULL src moved to mb_neg_probe_restartable_src_on_error / _null_src.*/
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

/* own state buffer: larger than any CRTs mbstate_t (ucrt: 8 bytes), avoids depending on the mingw typedef */
typedef struct { unsigned char b[16]; } mbs_t;
#define ST(p) ((mbstate_t *)(p))
static int set932(void) { return setlocale(LC_ALL, ".932") != NULL; }
static void restore_c(void) { setlocale(LC_ALL, "C"); }
TC_DECL_PROC(size_t, wcsrtombs, (char *, const wchar_t **, size_t, void *));
#define HAVE_ALL (tc_p_wcsrtombs)

TC_CASE(unmappable)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; char d[8]; static const wchar_t s[] = { L'a', 0x4E00, L'b', 0 }; const wchar_t *src = s; memset(&st, 0, sizeof st);
    errno = 0;
    CHECK_EQ_U(tc_p_wcsrtombs(d, &src, 8, ST(&st)), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcsrtombs);
    TC_RUN(unmappable);
    return tc_end();
}
