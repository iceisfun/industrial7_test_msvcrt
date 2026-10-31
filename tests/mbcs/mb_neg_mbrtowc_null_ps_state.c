/* TC
 * id:    mb_neg_mbrtowc_null_ps_state
 * area:  mbcs
 * funcs: 
 * procs: mbrtowc
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.29.6.3.2; MS docs mbrtowc (NULL ps uses an internal static state)
 * note:  NULL mbstate_t pointer must be accepted without the invalid-parameter handler (MS docs: internal state is used).*/
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
TC_DECL_PROC(size_t, mbrtowc, (wchar_t *, const char *, size_t, void *));
#define HAVE_ALL (tc_p_mbrtowc)

TC_CASE(ps_null)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    wchar_t wc = 0; volatile char a = 'Z'; char s[2]; s[0] = a; s[1] = 0;
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_U(tc_p_mbrtowc(&wc, s, 1, NULL), 1);
    CHECK_EQ_I(wc, L'Z');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(s_null)
{
    if (!HAVE_ALL) TC_SKIP("function not exported by this CRT");
    mbs_t st; memset(&st, 0, sizeof st);
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_U(tc_p_mbrtowc(NULL, NULL, 1, ST(&st)), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbrtowc);
    TC_RUN(ps_null); TC_RUN(s_null);
    return tc_end();
}
