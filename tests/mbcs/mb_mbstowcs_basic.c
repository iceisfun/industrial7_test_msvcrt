/* TC
 * id:    mb_mbstowcs_basic
 * area:  mbcs
 * funcs: mbstowcs setlocale
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.8.1; MS docs mbstowcs
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

#define SENT 0x7777
static void fill(wchar_t *w, int n) { int i; for (i = 0; i < n; i++) w[i] = SENT; }

TC_CASE(dst_null_count)
{
    const char *volatile s = "hello";
    CHECK_EQ_U(mbstowcs(NULL, s, 0), 5);
    CHECK_EQ_U(mbstowcs(NULL, s, 100), 5);
}
TC_CASE(full)
{
    wchar_t w[16]; const char *volatile s = "abc"; size_t n;
    fill(w, 16);
    n = mbstowcs(w, s, 10);
    CHECK_EQ_U(n, 3); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[2], 'c'); CHECK_EQ_U(w[3], 0);
}
TC_CASE(limited)
{
    wchar_t w[16]; const char *volatile s = "abc"; size_t n;
    fill(w, 16);
    n = mbstowcs(w, s, 2);
    CHECK_EQ_U(n, 2); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 'b'); CHECK_EQ_U(w[2], SENT);
}
TC_CASE(count_zero)
{
    wchar_t w[4]; const char *volatile s = "abc"; size_t n;
    fill(w, 4);
    n = mbstowcs(w, s, 0);
    CHECK_EQ_U(n, 0); CHECK_EQ_U(w[0], SENT);
}
TC_CASE(cp932)
{
    wchar_t w[8]; const char s[] = { (char)0x82, (char)0xA0, 'A', 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    fill(w, 8);
    n = mbstowcs(w, s, 8);
    CHECK_EQ_U(n, 2); CHECK_EQ_U(w[0], 0x3042); CHECK_EQ_U(w[1], 'A'); CHECK_EQ_U(w[2], 0);
    CHECK_EQ_U(mbstowcs(NULL, s, 0), 2);
    setlocale(LC_ALL, "C");
}
TC_CASE(empty)
{
    wchar_t w[4]; const char *volatile s = ""; size_t n;
    fill(w, 4);
    n = mbstowcs(w, s, 4);
    CHECK_EQ_U(n, 0); CHECK_EQ_U(w[0], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(dst_null_count); TC_RUN(full); TC_RUN(limited); TC_RUN(count_zero); TC_RUN(cp932); TC_RUN(empty);
    return tc_end();
}
