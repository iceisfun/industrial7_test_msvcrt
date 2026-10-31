/* TC
 * id:    scn_neg_snscanf_count_huge_probe
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf _snwscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snscanf (count is size_t)
 * note:  Probes count == SIZE_MAX: believed to behave like sscanf (stop at the NUL, no overread). Older CRTs (<140) store count in the int _cnt field of a fake FILE, so (size_t)-1 may become -1 and read nothing (ret EOF); real Windows tells.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#ifdef _UCRT
#include <stdarg.h>
static int tcx_snwscanf(const wchar_t *s, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswscanf(_CRT_INTERNAL_LOCAL_SCANF_OPTIONS, s, n, f, NULL, ap); va_end(ap); return r; }
#undef _snwscanf
#define _snwscanf tcx_snwscanf
#endif

static volatile size_t cnt;

TC_CASE(count_size_max_stops_at_nul) { int a = 0, b = 99, r; cnt = (size_t)-1; r = _snscanf("12 ab", cnt, "%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 99); }
TC_CASE(count_size_max_eof_empty) { int a = 99, r; cnt = (size_t)-1; r = _snscanf("", cnt, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 99); }
TC_CASE(wide_count_size_max) { int a = 0, b = 99, r; cnt = (size_t)-1; r = _snwscanf(L"12 ab", cnt, L"%d %d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 99); }

int main(void) { tc_begin(); TC_RUN(count_size_max_stops_at_nul); TC_RUN(count_size_max_eof_empty); TC_RUN(wide_count_size_max); return tc_end(); }
