/* TC
 * id:    chr_probe_ungetwc_ansi_text
 * area:  stdio_char
 * funcs: ungetwc fgetwc
 * procs: _ungetwc_nolock _fgetwc_nolock
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.29.3.10; MS docs ungetwc
 * note:  PROBE: on a stream opened with plain "r" (ANSI text, no ccs), ungetwc(L'x') returns L'x' and the next fgetwc
 *        returns L'x' followed by the unconsumed rest of the file (i.e. the pushed-back wide char occupies one narrow
 *        char, not two raw bytes). Also probes _ungetwc_nolock the same way. Believed true; undocumented.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_probe_ungetwc_ansi_text_" TC_S(TC_CRT_VER) ".tmp"
static volatile wint_t vc = L'u';
TC_DECL_PROC(wint_t, _ungetwc_nolock, (wint_t, FILE *));
TC_DECL_PROC(wint_t, _fgetwc_nolock, (FILE *));

TC_CASE(ungetwc_ansi)
{
    FILE *f; wr(FN, "xy", 2); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'x');
    CHECK_EQ_U(ungetwc(vc, f), L'u'); CHECK_EQ_U(fgetwc(f), L'u'); CHECK_EQ_U(fgetwc(f), L'y'); CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); remove(FN);
}
TC_CASE(ungetwc_after_eof_ansi)
{
    FILE *f; wr(FN, "a", 1); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fgetwc(f), L'a'); CHECK_EQ_U(fgetwc(f), WEOF); CHECK(feof(f) != 0);
    vc = L'x'; CHECK_EQ_U(ungetwc(vc, f), L'x'); CHECK(feof(f) == 0);
    CHECK_EQ_U(fgetwc(f), L'x'); CHECK_EQ_U(fgetwc(f), WEOF);
    fclose(f); remove(FN);
}
TC_CASE(nolock_ansi)
{
    FILE *f;
    if (!tc_p__ungetwc_nolock || !tc_p__fgetwc_nolock) TC_SKIP("exports missing");
    wr(FN, "xy", 2); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    _lock_file(f);
    vc = L'u';
    CHECK_EQ_U(tc_p__fgetwc_nolock(f), L'x');
    CHECK_EQ_U(tc_p__ungetwc_nolock(vc, f), L'u'); CHECK_EQ_U(tc_p__fgetwc_nolock(f), L'u'); CHECK_EQ_U(tc_p__fgetwc_nolock(f), L'y');
    _unlock_file(f);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_ungetwc_nolock); TC_LOAD_PROC(_fgetwc_nolock); TC_RUN(ungetwc_ansi); TC_RUN(ungetwc_after_eof_ansi); TC_RUN(nolock_ansi); return tc_end(); }
