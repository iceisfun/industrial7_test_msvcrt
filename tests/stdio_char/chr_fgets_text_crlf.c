/* TC
 * id:    chr_fgets_text_crlf
 * area:  stdio_char
 * funcs: fgets
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: CR-LF translated to LF on input), fgets
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_fgets_text_crlf_a" TCV_S(TC_CRT_VER) ".tmp"
static void mk(const char *d, size_t n) { FILE *f = fopen(FN, "wb"); fwrite(d, 1, n, f); fclose(f); }

TC_CASE(text_translates)
{
    char b[16]; FILE *f; mk("ab\r\ncd\r\n", 8);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "ab\n");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "cd\n");
    CHECK(fgets(b, 16, f) == NULL);
    fclose(f); remove(FN);
}
TC_CASE(binary_keeps_cr)
{
    char b[16]; FILE *f; mk("ab\r\ncd\r\n", 8);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "ab\r\n");
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "cd\r\n");
    fclose(f); remove(FN);
}
TC_CASE(lone_cr_kept)
{
    char b[16]; FILE *f; mk("a\rb\n", 4);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 16, f) == b); CHECK_STR(b, "a\rb\n");
    fclose(f); remove(FN);
}
TC_CASE(small_n)
{
    char b[3]; FILE *f; mk("ab\r\ncd\r\n", 8);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 3, f) == b); CHECK_STR(b, "ab");
    CHECK(fgets(b, 3, f) == b); CHECK_STR(b, "\n");
    CHECK(fgets(b, 3, f) == b); CHECK_STR(b, "cd");
    CHECK(fgets(b, 3, f) == b); CHECK_STR(b, "\n");
    CHECK(fgets(b, 3, f) == NULL);
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(text_translates); TC_RUN(binary_keeps_cr); TC_RUN(lone_cr_kept); TC_RUN(small_n); return tc_end(); }
