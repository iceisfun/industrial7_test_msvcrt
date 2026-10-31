/* TC
 * id:    m99_neg_nan_tag_not_payload
 * area:  math_c99
 * funcs: nan nanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.11.2 (nan returns a quiet NaN; tag contents implementation-defined)
 * note:  Any tag, including malformed or very long ones, must give a NaN and never crash or read past the terminating NUL. Payload is not asserted.
 */
#include "tc.h"
#include <math.h>

static char big[4097];

TC_CASE(empty_tag) { volatile const char *t = ""; double r = nan((const char *)t); float f = nanf((const char *)t); CHECK(r != r); CHECK(f != f); }
TC_CASE(long_tag)
{
    int i; double r; float f;
    tc_iph_install(); tc_iph_count = 0;
    for (i = 0; i < 4096; i++) big[i] = (char)('0' + i % 10);
    big[4096] = 0;
    { volatile const char *t = big; r = nan((const char *)t); f = nanf((const char *)t); }
    CHECK(r != r); CHECK(f != f); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(odd_tags)
{
    volatile const char *a = "0x", *b = "1e5", *c = " 7", *d = "7 ", *e = "abc!";
    double r;
    r = nan((const char *)a); CHECK(r != r);
    r = nan((const char *)b); CHECK(r != r);
    r = nan((const char *)c); CHECK(r != r);
    r = nan((const char *)d); CHECK(r != r);
    r = nan((const char *)e); CHECK(r != r);
}
TC_CASE(guard_page)
{
    unsigned char *p = (unsigned char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD old; int i; double r; float f; volatile const char *t;
    if (!p) TC_SKIP("VirtualAlloc failed");
    if (!VirtualProtect(p + 4096, 4096, PAGE_NOACCESS, &old)) { VirtualFree(p, 0, MEM_RELEASE); TC_SKIP("VirtualProtect failed"); }
    for (i = 0; i < 4095; i++) p[i + 1] = (unsigned char)('1' + i % 9);   /* last 4095 bytes of page 0 */
    p[4095] = 0;
    t = (const char *)(p + 4095 - 8);  /* 8 digits then NUL at the last byte of the page */
    r = nan((const char *)t); f = nanf((const char *)t);
    CHECK(r != r); CHECK(f != f);
    t = (const char *)(p + 4095);      /* empty string right at the edge */
    r = nan((const char *)t); CHECK(r != r);
    VirtualFree(p, 0, MEM_RELEASE);
}
TC_CASE(two_calls)
{
    volatile const char *a = "1", *b = "2"; double x = nan((const char *)a), y = nan((const char *)b);
    CHECK(x != x); CHECK(y != y);
}
int main(void)
{
    tc_begin();
    TC_RUN(empty_tag); TC_RUN(long_tag); TC_RUN(odd_tags); TC_RUN(guard_page); TC_RUN(two_calls);
    return tc_end();
}
