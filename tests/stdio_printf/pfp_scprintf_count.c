/* TC
 * id:    pfp_scprintf_count
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf: "returns the number of characters that would be generated, not including the terminating null"; printf type field p (hex digits, uppercase on x64: 16 digits)
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(plain)  { CHECK_EQ_I(_scprintf("abc"), 3); CHECK_EQ_I(_scprintf(""), 0); }
TC_CASE(with_int) { CHECK_EQ_I(_scprintf("abc%d", 5), 4); CHECK_EQ_I(_scprintf("%5d", 42), 5); CHECK_EQ_I(_scprintf("%d", -12345), 6); }
TC_CASE(pointer)
{
    void *volatile p = (void *)(uintptr_t)0x1234;
    CHECK_EQ_I(_scprintf("%p", p), (int)(sizeof(void *) * 2));
}
TC_CASE(long_string)
{
    static const char s[] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    CHECK_EQ_I(_scprintf("%s%s", s, s), 200);
}
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(with_int); TC_RUN(pointer); TC_RUN(long_string); return tc_end(); }
