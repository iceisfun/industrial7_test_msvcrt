/* TC
 * id:    pfs_fprintf_str_file
 * area:  stdio_printf
 * funcs: fprintf
 * procs: remove
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf; fopen text vs binary mode (LF -> CRLF)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define STR2_(x) #x
#define STR_(x) STR2_(x)
#define FN "tc_pfs_str_file_a" STR_(TC_CRT_VER) ".tmp"
static volatile const char *vs = "str";
static volatile const wchar_t *vw = L"wide";

static size_t slurp(char *buf, size_t max) { FILE *f = fopen(FN, "rb"); size_t n; if (!f) return (size_t)-1; n = fread(buf, 1, max, f); fclose(f); return n; }

TC_CASE(flavors) {
  FILE *f = fopen(FN, "wb"); char b[64]; size_t n; int r1, r2;
  CHECK(f != NULL); if (!f) return;
  r1 = fprintf(f, "%s %c ", (const char *)vs, 'x');
  r2 = fprintf(f, "%ls %S", (const wchar_t *)vw, (const wchar_t *)vw);
  fclose(f);
  CHECK_EQ_I(r1, 6); CHECK_EQ_I(r2, 9);
  n = slurp(b, 63); CHECK_EQ_U(n, 15); b[n < 63 ? n : 63] = 0;
  CHECK_STR(b, "str x wide wide");
  remove(FN);
}
TC_CASE(nul_char) {
  FILE *f = fopen(FN, "wb"); char b[16]; size_t n; int r; volatile int z = 0;
  CHECK(f != NULL); if (!f) return;
  r = fprintf(f, "a%cb", z); fclose(f);
  CHECK_EQ_I(r, 3);
  n = slurp(b, 16); CHECK_EQ_U(n, 3); CHECK(b[0]=='a' && b[1]==0 && b[2]=='b');
  remove(FN);
}
TC_CASE(text_mode_crlf) {
  FILE *f = fopen(FN, "w"); char b[16]; size_t n; int r;
  CHECK(f != NULL); if (!f) return;
  r = fprintf(f, "%s\n", (const char *)vs); fclose(f);
  CHECK_EQ_I(r, 4);
  n = slurp(b, 16); CHECK_EQ_U(n, 5); CHECK(b[3]=='\r' && b[4]=='\n');
  remove(FN);
}
TC_CASE(binary_mode_lf) {
  FILE *f = fopen(FN, "wb"); char b[16]; size_t n; int r;
  CHECK(f != NULL); if (!f) return;
  r = fprintf(f, "%s\n", (const char *)vs); fclose(f);
  CHECK_EQ_I(r, 4);
  n = slurp(b, 16); CHECK_EQ_U(n, 4); CHECK_EQ_I(b[3], '\n');
  remove(FN);
}
int main(void) { tc_begin(); TC_RUN(flavors); TC_RUN(nul_char); TC_RUN(text_mode_crlf); TC_RUN(binary_mode_lf); return tc_end(); }
