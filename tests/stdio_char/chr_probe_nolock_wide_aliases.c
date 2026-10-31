/* TC
 * id:    chr_probe_nolock_wide_aliases
 * area:  stdio_char
 * funcs: _lock_file _unlock_file
 * procs: _putwc_nolock _getwc_nolock
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putwc_nolock / _getwc_nolock (documented as macros/aliases of _fputwc_nolock/_fgetwc_nolock)
 * note:  PROBE: the real DLLs export _putwc_nolock and _getwc_nolock as functions that behave like _fputwc_nolock/_fgetwc_nolock.
 *        The MS headers define them as macros, so the export itself is what is probed (mingw import libs list them).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define FN "tc_chr_probe_nolock_wide_aliases_" TC_S(TC_CRT_VER) ".tmp"
TC_DECL_PROC(wint_t, _putwc_nolock, (wchar_t, FILE *));
TC_DECL_PROC(wint_t, _getwc_nolock, (FILE *));

TC_CASE(exported) { CHECK(tc_p__putwc_nolock != NULL); CHECK(tc_p__getwc_nolock != NULL); }
TC_CASE(roundtrip)
{
    unsigned char b[8]; size_t n; FILE *f;
    if (!tc_p__putwc_nolock || !tc_p__getwc_nolock) TC_SKIP("not exported (see exported)");
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); CHECK_EQ_U(tc_p__putwc_nolock(L'k', f), L'k'); CHECK_EQ_U(tc_p__putwc_nolock(L'l', f), L'l'); _unlock_file(f);
    fclose(f); n = rd(FN, b, 8); CHECK_EQ_U(n, 2); CHECK_MEM(b, "kl", 2);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    _lock_file(f); CHECK_EQ_U(tc_p__getwc_nolock(f), L'k'); CHECK_EQ_U(tc_p__getwc_nolock(f), L'l'); CHECK_EQ_U(tc_p__getwc_nolock(f), WEOF); _unlock_file(f);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_putwc_nolock); TC_LOAD_PROC(_getwc_nolock); TC_RUN(exported); TC_RUN(roundtrip); return tc_end(); }
