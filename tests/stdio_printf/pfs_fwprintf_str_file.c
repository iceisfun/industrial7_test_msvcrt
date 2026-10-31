/* TC
 * id:    pfs_fwprintf_str_file
 * area:  stdio_printf
 * funcs: fwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf (wide output to ANSI-mode stream is converted to multibyte)
 * note:  stream opened "w" (ANSI text mode); no newline is written so no CRLF translation.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define STR2_(x) #x
#define STR_(x) STR2_(x)
#define FN "tc_pfs_str_file_w" STR_(TC_CRT_VER) ".tmp"
static volatile const char *vn = "narrow";
static volatile const wchar_t *vw = L"abc";

TC_CASE(all) {
  FILE *f = fopen(FN, "w"); char b[64]; size_t n; int r1, r2, r3;
  CHECK(f != NULL); if (!f) return;
  r1 = fwprintf(f, L"%s|", (const wchar_t *)vw);
  r2 = fwprintf(f, L"%hs|%S|", (const char *)vn, (const char *)vn);
  r3 = fwprintf(f, L"%c", L'k');
  fclose(f);
  CHECK_EQ_I(r1, 4); CHECK_EQ_I(r2, 14); CHECK_EQ_I(r3, 1);
  f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
  n = fread(b, 1, 63, f); fclose(f); b[n] = 0;
  CHECK_EQ_U(n, 19);
  CHECK_STR(b, "abc|narrow|narrow|k");
  remove(FN);
}
int main(void) { tc_begin(); TC_RUN(all); return tc_end(); }
