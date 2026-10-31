/* TC
 * id:    sec_strnset_s_edge_probe
 * area:  secure_crt
 * funcs: _strnset_s _wcsnset_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _strnset_s, _wcsnset_s
 * note:  PROBE: _strnset_s with count==0 returns 0 and leaves the string unchanged; _wcsnset_s with count larger than the buffer size (but string shorter) stops at the NUL and returns 0 (docs do not list count>size as an error).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(strnset_zero_count) { char b[8] = "abc"; CHECK_EQ_I(_strnset_s(b, sizeof b, '*', 0), 0); CHECK_STR(b, "abc"); }
TC_CASE(wcsnset_count_exceeds_size) { wchar_t b[8] = L"***def"; CHECK_EQ_I(_wcsnset_s(b, 8, L'#', 20), 0); CHECK_WSTR(b, L"######"); CHECK_EQ_I(b[6], 0); }
int main(void) { tc_begin(); TC_RUN(strnset_zero_count); TC_RUN(wcsnset_count_exceeds_size); return tc_end(); }
