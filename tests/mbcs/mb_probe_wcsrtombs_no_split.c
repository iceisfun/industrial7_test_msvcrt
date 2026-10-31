/* TC
 * id:    mb_probe_wcsrtombs_no_split
 * area:  mbcs
 * funcs: setlocale
 * procs: wcsrtombs
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.29.6.4.2
 * note:  PROBES: wcsrtombs with a len that cannot hold a whole 2-byte cp932 character stops before it (C11: never splits), returns 1 and leaves *src at the character.
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

TC_CASE(no_split)
{
    mbs_t st; unsigned char d[8]; static const wchar_t s[] = { L'a', 0x3042, 0 }; const wchar_t *src = s;
    if (!tc_p_wcsrtombs) TC_SKIP("wcsrtombs not exported");
    if (!set932()) TC_SKIP("cp932 locale unavailable");
    memset(&st, 0, sizeof st); memset(d, 0, sizeof d);
    CHECK_EQ_U(tc_p_wcsrtombs((char *)d, &src, 2, ST(&st)), 1);
    CHECK_EQ_I(d[0], 'a');
    CHECK_EQ_P(src, s + 1);
    restore_c();
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcsrtombs);
    TC_RUN(no_split);
    return tc_end();
}
