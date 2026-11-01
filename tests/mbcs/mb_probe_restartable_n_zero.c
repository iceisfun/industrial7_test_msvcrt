/* TC
 * id:    mb_probe_restartable_n_zero
 * area:  mbcs
 * funcs: 
 * procs: mbrtowc mbrlen
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.29.6.3.2; MS docs mbrtowc
 * note:  PROBES: mbrtowc/mbrlen with n == 0 return (size_t)-2 (C11 says an incomplete sequence; MS implementations are not documented to agree).
 *        Observed Win10 22H2 ucrtbase: both return 0 (errno untouched); deviates from C11 7.29.6.3.2. Stable.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

typedef struct { unsigned char b[16]; } mbs_t;
#define ST(p) ((mbstate_t *)(p))
static int set932(void) { return setlocale(LC_ALL, ".932") != NULL; }
static void restore_c(void) { setlocale(LC_ALL, "C"); }
TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
TC_DECL_PROC(size_t, mbrlen, (const char *, size_t, void *));
TC_DECL_PROC(size_t, mbsrtowcs, (wchar_t *, const char **, size_t, void *));
TC_DECL_PROC(size_t, wcsrtombs, (char *, const wchar_t **, size_t, void *));

TC_CASE(mbrtowc_n0)
{
    mbs_t st; wchar_t wc = 0; volatile char a = 'A'; char s[2];
    if (!tc_p_mbrtowc) TC_SKIP("mbrtowc not exported");
    s[0] = a; s[1] = 0; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 0, ST(&st)), 0);   /* observed; C11 says -2 */
}
TC_CASE(mbrlen_n0)
{
    mbs_t st; volatile char a = 'A'; char s[2];
    if (!tc_p_mbrlen) TC_SKIP("mbrlen not exported");
    s[0] = a; s[1] = 0; memset(&st, 0, sizeof st);
    CHECK_EQ_U(tc_p_mbrlen(s, 0, ST(&st)), 0);   /* observed; C11 says -2 */
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_LOAD_PROC(mbrlen);
    TC_RUN(mbrtowc_n0); TC_RUN(mbrlen_n0);
    return tc_end();
}
