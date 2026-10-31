/* TC
 * id:    mb_neg_probe_restartable_null_src
 * area:  mbcs
 * funcs: 
 * procs: mbsrtowcs wcsrtombs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs mbsrtowcs, wcsrtombs (parameter validation)
 * note:  PROBES: a NULL src pointer-to-pointer is rejected via the invalid-parameter handler with -1/EINVAL. Not explicitly documented for the non-_s functions; older CRTs might simply crash.
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

/* child: exit 0 if the CRT returned -1/EINVAL and called the handler; 1 otherwise; a crash gives an exception status */
static int child_main(const char *name)
{
    mbs_t st; wchar_t wd[4]; char d[4]; size_t r; memset(&st, 0, sizeof st);
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    TC_LOAD_PROC(mbsrtowcs);
    TC_LOAD_PROC(wcsrtombs);
    if (name[0] == 'm') { if (!tc_p_mbsrtowcs) return 2; r = tc_p_mbsrtowcs(wd, NULL, 4, ST(&st)); }
    else { if (!tc_p_wcsrtombs) return 2; r = tc_p_wcsrtombs(d, NULL, 4, ST(&st)); }
    return (r == (size_t)-1 && errno == EINVAL && tc_iph_count >= 1) ? 0 : 1;
}
TC_CASE(mbsrtowcs_null_src)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("mbs", &code, 20000));
    if (code == 2) TC_SKIP("mbsrtowcs not exported");
    CHECK_EQ_U(code, 0);
}
TC_CASE(wcsrtombs_null_src)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("wcs", &code, 20000));
    if (code == 2) TC_SKIP("wcsrtombs not exported");
    CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(mbsrtowcs_null_src); TC_RUN(wcsrtombs_null_src);
    return tc_end();
}
