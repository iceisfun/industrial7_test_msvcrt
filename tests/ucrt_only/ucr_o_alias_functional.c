/* TC
 * id:    ucr_o_alias_functional
 * area:  ucrt_only
 * funcs: srand rand
 * procs: _o_malloc _o_free _o_calloc _o_realloc _o_toupper _o_tolower _o_atoi _o_strtod _o_srand _o_rand _o__itoa_s _o_strcpy_s _o_memcpy_s
 * crt:   win10
 * tier:  p1
 * conf:  observed
 * ref:   ucrtbase _o_ aliases are the same functions as their unprefixed counterparts
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

typedef void *(__cdecl *malloc_t)(size_t);
typedef void *(__cdecl *calloc_t)(size_t, size_t);
typedef void *(__cdecl *realloc_t)(void *, size_t);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *ctype_t)(int);
typedef int (__cdecl *atoi_t)(const char *);
typedef double (__cdecl *strtod_t)(const char *, char **);
typedef void (__cdecl *srand_t)(unsigned);
typedef int (__cdecl *rand_t)(void);
typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
typedef int (__cdecl *strcpy_s_t)(char *, size_t, const char *);
typedef int (__cdecl *memcpy_s_t)(void *, size_t, const void *, size_t);

#define P(t, n) static t p##n
P(malloc_t, _o_malloc); P(calloc_t, _o_calloc); P(realloc_t, _o_realloc); P(free_t, _o_free);
P(ctype_t, _o_toupper); P(ctype_t, _o_tolower); P(atoi_t, _o_atoi); P(strtod_t, _o_strtod);
P(srand_t, _o_srand); P(rand_t, _o_rand); P(itoa_s_t, _o__itoa_s); P(strcpy_s_t, _o_strcpy_s); P(memcpy_s_t, _o_memcpy_s);

static volatile int v_a = 'a', v_A = 'A';

TC_CASE(heap)
{
    unsigned char *p, *q; int i;
    p = (unsigned char *)p_o_calloc(16, 4); CHECK(p != NULL);
    if (p) { for (i = 0; i < 64; i++) CHECK_EQ_I(p[i], 0); p[0] = 0x5a; p[63] = 0xa5; }
    q = (unsigned char *)p_o_realloc(p, 256); CHECK(q != NULL);
    if (q) { CHECK_EQ_I(q[0], 0x5a); CHECK_EQ_I(q[63], 0xa5); p_o_free(q); }
    p = (unsigned char *)p_o_malloc(32); CHECK(p != NULL);
    if (p) { memset(p, 1, 32); CHECK_EQ_I(p[31], 1); p_o_free(p); }
}
TC_CASE(ctype) { CHECK_EQ_I(p_o_toupper(v_a), 'A'); CHECK_EQ_I(p_o_tolower(v_A), 'a'); CHECK_EQ_I(p_o_toupper('1'), '1'); }
TC_CASE(conv)
{
    static const char *volatile s = "42", *volatile d = "1.5";
    CHECK_EQ_I(p_o_atoi(s), 42);
    CHECK_BITS_D(p_o_strtod(d, NULL), 1.5);
}
TC_CASE(rand_seq)
{
    int a[5], b[5], i;
    srand(1); for (i = 0; i < 5; i++) a[i] = rand();
    p_o_srand(1); for (i = 0; i < 5; i++) b[i] = p_o_rand();
    for (i = 0; i < 5; i++) CHECK_EQ_I(a[i], b[i]);
    for (i = 0; i < 5; i++) CHECK(b[i] >= 0 && b[i] <= 32767);
}
TC_CASE(secure_normal)
{
    char b[16]; char d[8]; unsigned char m[4]; static const unsigned char src[4] = {1, 2, 3, 4};
    CHECK_EQ_I(p_o__itoa_s(12345, b, sizeof b, 10), 0); CHECK_STR(b, "12345");
    CHECK_EQ_I(p_o__itoa_s(255, b, sizeof b, 16), 0); CHECK_STR(b, "ff");
    CHECK_EQ_I(p_o_strcpy_s(d, sizeof d, "abc"), 0); CHECK_STR(d, "abc");
    CHECK_EQ_I(p_o_memcpy_s(m, sizeof m, src, 4), 0); CHECK_MEM(m, src, 4);
}
#define L(n) do { p##n = (void *)tc_crt_proc(#n); if (!p##n) { tc_write("TC FAIL missing " #n "\n"); return 1; } } while (0)
int main(void)
{
    tc_begin();
    L(_o_malloc); L(_o_calloc); L(_o_realloc); L(_o_free); L(_o_toupper); L(_o_tolower); L(_o_atoi); L(_o_strtod);
    L(_o_srand); L(_o_rand); L(_o__itoa_s); L(_o_strcpy_s); L(_o_memcpy_s);
    TC_RUN(heap); TC_RUN(ctype); TC_RUN(conv); TC_RUN(rand_seq); TC_RUN(secure_normal);
    return tc_end();
}
