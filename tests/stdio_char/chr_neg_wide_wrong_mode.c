/* TC
 * id:    chr_neg_wide_wrong_mode
 * area:  stdio_char
 * funcs: fgetwc fputwc fgetws fputws
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs fgetwc, fputwc, fgetws, fputws (return WEOF/NULL on error)
 * note:  Wide I/O in the wrong direction must fail (WEOF/NULL, documented) and set the error indicator (not explicitly documented, hence inferred).
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#define FN "tc_chr_neg_wide_wrong_mode_" TCS(TC_CRT_VER) ".tmp"
static void mk(const char *n) { FILE *f = fopen(n, "wb"); if (f) { fwrite("abc\n", 1, 4, f); fclose(f); } }
TC_CASE(fgetwc_on_w)  { FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return; CHECK(fgetwc(f) == WEOF); CHECK(ferror(f) != 0); fclose(f); remove(FN); }
TC_CASE(fgetws_on_w)  { wchar_t b[8]; FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return; CHECK(fgetws(b, 8, f) == NULL); CHECK(ferror(f) != 0); fclose(f); remove(FN); }
TC_CASE(fputwc_on_r)  { FILE *f; mk(FN); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return; CHECK(fputwc(L'x', f) == WEOF); CHECK(ferror(f) != 0); fclose(f); remove(FN); }
TC_CASE(fputws_on_r)  { FILE *f; mk(FN); f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return; CHECK(fputws(L"x", f) == WEOF); CHECK(ferror(f) != 0); fclose(f); remove(FN); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fgetwc_on_w); TC_RUN(fgetws_on_w); TC_RUN(fputwc_on_r); TC_RUN(fputws_on_r); return tc_end(); }
