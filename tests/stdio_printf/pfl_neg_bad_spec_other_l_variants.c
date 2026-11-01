/* TC
 * id:    pfl_neg_bad_spec_other_l_variants
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf_l _vsprintf_l _vfprintf_l _vswprintf_l _create_locale _free_locale fopen fclose
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf format specification syntax; "Parameter validation"
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "snprintf_l_pct_y r == -1 got=3", "fprintf_l_pct_y ... got=1", "vsprintf_l_pct_bang ... got=1",
 *        zzp_probe2: "_snprintf_l a%yb: r=3 errno=0 iph=0 b=[ayb]", "_sprintf_l %!: r=1 b=[!]", "_swprintf_l %lq: r=1 w=[q]"): ucrt does NOT reject an unknown
 *        conversion character ("%y", "%lq", "%!") in the _l entry points: the % and any length prefix are dropped and the character is printed
 *        literally, no invalid-parameter call, errno unchanged. The old belief (-1/EINVAL/handler) was wrong. C11 7.21.6.1p9: an invalid
 *        conversion specification is undefined behaviour, so ucrt is conforming. Stable (C locale object, no env dependence).
 *        Temp file tc_pfl_neg_bad_spec_other_l_variants.tmp.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static _locale_t loc;
static const char *fname = "tc_pfl_neg_bad_spec_other_l_variants.tmp";
static const char * volatile v_f;
static const wchar_t * volatile v_wf;

TC_CASE(snprintf_l_pct_y)
{
    char b[16]; int r; v_f = "a%yb";
    tc_iph_count = 0; errno = 0;
    r = pl_snprintf_l(b, 16, v_f, loc, 1);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "ayb"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fprintf_l_pct_y)
{
    FILE *f; int r; v_f = "%y";
    f = fopen(fname, "w"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = pl_fprintf_l(f, v_f, loc, 1);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
    { char rb[8] = {0}; FILE *g = fopen(fname, "r"); CHECK(g != NULL); if (g) { size_t k = fread(rb, 1, 7, g); fclose(g); CHECK_EQ_I((int)k, 1); CHECK_STR(rb, "y"); } }
    remove(fname);
}
TC_CASE(swprintf_l_pct_lq)
{
    wchar_t b[16]; int r; v_wf = L"%lq";
    tc_iph_count = 0; errno = 0;
    r = pl_swprintf_l(b, v_wf, loc, 1);
    CHECK_EQ_I(r, 1); CHECK(b[0] == L'q' && b[1] == 0); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(vsprintf_l_pct_bang)
{
    char b[16]; int r; v_f = "%!";
    tc_iph_count = 0; errno = 0;
    r = pl_sprintf_l(b, v_f, loc, 1);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "!"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(valid_afterwards)
{
    char b[16]; wchar_t w[16]; tc_iph_count = 0;
    CHECK_EQ_I(pl_snprintf_l(b, 16, "%d%%", loc, 7), 2); CHECK_STR(b, "7%");
    CHECK_EQ_I(pl_swprintf_l(w, L"%ls!", loc, L"ok"), 3); CHECK_WSTR(w, L"ok!");
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(snprintf_l_pct_y); TC_RUN(fprintf_l_pct_y); TC_RUN(swprintf_l_pct_lq); TC_RUN(vsprintf_l_pct_bang); TC_RUN(valid_afterwards);
    _free_locale(loc);
    remove(fname);
    return tc_end();
}
