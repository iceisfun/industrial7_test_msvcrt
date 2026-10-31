/* TC
 * id:    pfs_wide_legacy_specifiers_option
 * area:  stdio_printf
 * funcs: __stdio_common_vswprintf
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Format specification syntax: printf and wprintf (_CRT_STDIO_LEGACY_WIDE_SPECIFIERS / ISO wide specifiers); corecrt_stdio_config.h option bits
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define OPT_LEGACY 0x4ULL
static int wf(unsigned long long opt, wchar_t *b, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = __stdio_common_vswprintf(opt, b, 64, fmt, NULL, ap); va_end(ap); return r; }
static volatile const char *vn = "nar";
static volatile const wchar_t *vw = L"wid";

TC_CASE(default_mode) {
  /* options==0: ISO C wide semantics: %s is narrow, %S is wide */
  wchar_t b[64]; wf(0, b, L"%s|%S", (const char *)vn, (const wchar_t *)vw); CHECK_WSTR(b, L"nar|wid");
}
TC_CASE(legacy_mode) {
  /* _CRT_INTERNAL_PRINTF_LEGACY_WIDE_SPECIFIERS: MS legacy: %s is wide, %S is narrow (the header default) */
  wchar_t b[64]; wf(OPT_LEGACY, b, L"%s|%S", (const wchar_t *)vw, (const char *)vn); CHECK_WSTR(b, L"wid|nar");
}
TC_CASE(explicit_unaffected) {
  wchar_t b[64];
  wf(0, b, L"%hs|%ls", (const char *)vn, (const wchar_t *)vw); CHECK_WSTR(b, L"nar|wid");
  wf(OPT_LEGACY, b, L"%hs|%ls", (const char *)vn, (const wchar_t *)vw); CHECK_WSTR(b, L"nar|wid");
}
TC_CASE(chars_flip) {
  wchar_t b[64];
  wf(0, b, L"%c%C", 'n', L'w'); CHECK_WSTR(b, L"nw");
  wf(OPT_LEGACY, b, L"%c%C", L'w', 'n'); CHECK_WSTR(b, L"wn");
}
int main(void) { tc_begin(); TC_RUN(default_mode); TC_RUN(legacy_mode); TC_RUN(explicit_unaffected); TC_RUN(chars_flip); return tc_end(); }
