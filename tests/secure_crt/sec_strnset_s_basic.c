/* TC
 * id:    sec_strnset_s_basic
 * area:  secure_crt
 * funcs: _strnset_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(strnset_count_less) { char b[8] = "abcdef"; CHECK_EQ_I(_strnset_s(b, sizeof b, '*', 3), 0); CHECK_STR(b, "***def"); }
TC_CASE(strnset_count_greater_stops_at_nul) { char b[8] = "abc"; b[4] = 'k'; CHECK_EQ_I(_strnset_s(b, sizeof b, '*', 6), 0); CHECK_STR(b, "***"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'k'); }
int main(void) { tc_begin(); TC_RUN(strnset_count_less); TC_RUN(strnset_count_greater_stops_at_nul); return tc_end(); }
