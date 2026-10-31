/* TC
 * id:    chr_neg_ungetwc_weof
 * area:  stdio_char
 * funcs: ungetwc fgetwc
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.3.10p3; MS docs ungetwc
 * note:  ungetwc(WEOF) must fail and leave the stream untouched.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
static void mk(const char *n, const void *d, size_t len) { FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
#define FN "tc_chr_neg_ungetwc_weof_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(fails_and_unchanged)
{
    FILE *f; volatile wint_t w = WEOF;
    mk(FN, "abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetwc(f), L'a');
    CHECK(ungetwc(w, f) == WEOF);
    CHECK_EQ_I(fgetwc(f), L'b');
    fclose(f); remove(FN);
}
TC_CASE(does_not_clear_feof)
{
    FILE *f; volatile wint_t w = WEOF;
    mk(FN, "a", 1);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetwc(f), L'a');
    CHECK(fgetwc(f) == WEOF);
    CHECK(feof(f) != 0);
    CHECK(ungetwc(w, f) == WEOF);
    CHECK(feof(f) != 0);
    CHECK(fgetwc(f) == WEOF);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fails_and_unchanged); TC_RUN(does_not_clear_feof); return tc_end(); }
