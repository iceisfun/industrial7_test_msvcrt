/* TC
 * id:    sec_wcsset_s_basic
 * area:  secure_crt
 * funcs: _wcsset_s _wcsnset_s _wcslwr_s _wcsupr_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strset_s/_wcsset_s, _strlwr_s/_wcslwr_s
 * note:  ASCII only
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(wcsset) { wchar_t b[8] = L"abcde"; CHECK_EQ_I(_wcsset_s(b, 8, L'z'), 0); CHECK_WSTR(b, L"zzzzz"); }
TC_CASE(wcsnset_count) { wchar_t b[8] = L"abcdef"; CHECK_EQ_I(_wcsnset_s(b, 8, L'*', 3), 0); CHECK_WSTR(b, L"***def"); }
TC_CASE(wcslwr_ascii) { wchar_t b[16] = L"HeLLo 123 WORLD"; CHECK_EQ_I(_wcslwr_s(b, 16), 0); CHECK_WSTR(b, L"hello 123 world"); }
TC_CASE(wcsupr_ascii) { wchar_t b[16] = L"HeLLo 123 world"; CHECK_EQ_I(_wcsupr_s(b, 16), 0); CHECK_WSTR(b, L"HELLO 123 WORLD"); }
int main(void) { tc_begin(); TC_RUN(wcsset); TC_RUN(wcsnset_count); TC_RUN(wcslwr_ascii); TC_RUN(wcsupr_ascii); return tc_end(); }
