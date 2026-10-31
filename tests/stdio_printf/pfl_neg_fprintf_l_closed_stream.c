/* TC
 * id:    pfl_neg_fprintf_l_closed_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: _fprintf_l _create_locale _free_locale fopen fclose
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fprintf, _fprintf_l (returns a negative value on error); errno constants (EBADF)
 * note:  Negative return is documented; errno == EBADF for a write to a read-only stream is observed CRT behaviour (not spelled out in the fprintf page).
 *        The stream is opened with fopen(...,"r"), i.e. read-only (a fclose'd FILE is never used: undefined behaviour).
 *        Whether the invalid-parameter handler also runs is not asserted. Temp file tc_pfl_neg_fprintf_l_ro.tmp.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static const char *fname = "tc_pfl_neg_fprintf_l_ro_" TC_CRT_DLL ".tmp";

TC_CASE(write_to_rdonly_stream_ret_neg)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "r"); CHECK(f != NULL);
    if (f) { r = _fprintf_l(f, "hello %d", l, 5); CHECK(r < 0); fclose(f); }
    _free_locale(l);
}
TC_CASE(errno_ebadf)
{
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "r"); CHECK(f != NULL);
    if (f) { errno = 0; _fprintf_l(f, "hello %d", l, 5); CHECK_EQ_I(errno, EBADF); fclose(f); }
    _free_locale(l);
}
TC_CASE(iph_not_expected_or_tolerated)
{
    /* Tolerated either way: only verifies the process survives and a second failing write also returns negative. */
    _locale_t l = _create_locale(LC_ALL, "C"); FILE *f; int r1 = 0, r2 = 0;
    CHECK(l != NULL); if (!l) return;
    f = fopen(fname, "r"); CHECK(f != NULL);
    if (f) { r1 = _fprintf_l(f, "a", l); r2 = _fprintf_l(f, "b", l); CHECK(r1 < 0); CHECK(r2 < 0); fclose(f); }
    _free_locale(l);
}

int main(void)
{
    FILE *f;
    tc_begin(); tc_iph_install();
    f = fopen(fname, "w"); if (f) { fputc('x', f); fclose(f); }
    TC_RUN(write_to_rdonly_stream_ret_neg); TC_RUN(errno_ebadf); TC_RUN(iph_not_expected_or_tolerated);
    DeleteFileA(fname);
    return tc_end();
}
