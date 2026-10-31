/* TC
 * id:    pfw_scwprintf_length
 * area:  stdio_printf
 * funcs: _scwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf
 * note:  ucrtbase: header inline over __stdio_common_vswprintf; writes nothing; count excludes NUL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(plain_literal) { CHECK_EQ_I(_scwprintf(L"hello"), 5); }
TC_CASE(with_args)     { volatile int v = 123; CHECK_EQ_I(_scwprintf(L"%d-%ls", v, L"ab"), 6); }
TC_CASE(empty_result_0){ CHECK_EQ_I(_scwprintf(L""), 0); CHECK_EQ_I(_scwprintf(L"%ls", L""), 0); }
TC_CASE(width_padded_len) { volatile int v = 7; CHECK_EQ_I(_scwprintf(L"%10d", v), 10); CHECK_EQ_I(_scwprintf(L"%-12ls|", L"a"), 13); }
TC_CASE(long_600)
{
    static wchar_t s[601]; int i;
    for (i = 0; i < 600; i++) s[i] = L'a' + (i % 26);
    s[600] = 0;
    CHECK_EQ_I(_scwprintf(L"%ls", s), 600);
    CHECK_EQ_I(_scwprintf(L"[%ls]", s), 602);
}

int main(void) { tc_begin(); TC_RUN(plain_literal); TC_RUN(with_args); TC_RUN(empty_result_0); TC_RUN(width_padded_len); TC_RUN(long_600); return tc_end(); }
