/* TC
 * id:    exp_string_basics_exported
 * area:  exports
 * funcs: strlen strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strspn strcspn strpbrk strtok strcoll strxfrm strerror _strdup _stricmp _strnicmp _strlwr _strupr _strrev _strnset _strset memcpy memmove memset memcmp memchr _memicmp _swab
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs string functions; C11 7.24
 * note:  Plain export presence.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)

/* taking the address makes the linker import each name from the CRT under test */
static void *volatile tc_sink;
static void touch_imports(void)
{
    tc_sink = (void *)strlen;
    tc_sink = (void *)strcpy;
    tc_sink = (void *)strncpy;
    tc_sink = (void *)strcat;
    tc_sink = (void *)strncat;
    tc_sink = (void *)strcmp;
    tc_sink = (void *)strncmp;
    tc_sink = (void *)strchr;
    tc_sink = (void *)strrchr;
    tc_sink = (void *)strstr;
    tc_sink = (void *)strspn;
    tc_sink = (void *)strcspn;
    tc_sink = (void *)strpbrk;
    tc_sink = (void *)strtok;
    tc_sink = (void *)strcoll;
    tc_sink = (void *)strxfrm;
    tc_sink = (void *)strerror;
    tc_sink = (void *)_strdup;
    tc_sink = (void *)_stricmp;
    tc_sink = (void *)_strnicmp;
    tc_sink = (void *)_strlwr;
    tc_sink = (void *)_strupr;
    tc_sink = (void *)_strrev;
    tc_sink = (void *)_strnset;
    tc_sink = (void *)_strset;
    tc_sink = (void *)memcpy;
    tc_sink = (void *)memmove;
    tc_sink = (void *)memset;
    tc_sink = (void *)memcmp;
    tc_sink = (void *)memchr;
    tc_sink = (void *)_memicmp;
    tc_sink = (void *)_swab;
}


TC_CASE(copy_group)
{
    HAS("strcpy");
    HAS("strncpy");
    HAS("strcat");
    HAS("strncat");
    HAS("_strdup");
}

TC_CASE(compare_group)
{
    HAS("strcmp");
    HAS("strncmp");
    HAS("strcoll");
    HAS("strxfrm");
    HAS("_stricmp");
    HAS("_strnicmp");
}

TC_CASE(search_group)
{
    HAS("strlen");
    HAS("strchr");
    HAS("strrchr");
    HAS("strstr");
    HAS("strspn");
    HAS("strcspn");
    HAS("strpbrk");
    HAS("strtok");
}

TC_CASE(case_group)
{
    HAS("_strlwr");
    HAS("_strupr");
    HAS("_strrev");
    HAS("_strnset");
    HAS("_strset");
}

TC_CASE(memory_group)
{
    HAS("memcpy");
    HAS("memmove");
    HAS("memset");
    HAS("memcmp");
    HAS("memchr");
    HAS("_memicmp");
}

TC_CASE(misc_group)
{
    HAS("strerror");
    HAS("_swab");
}

int main(void)
{
    tc_begin();
    touch_imports();
    TC_RUN(copy_group);
    TC_RUN(compare_group);
    TC_RUN(search_group);
    TC_RUN(case_group);
    TC_RUN(memory_group);
    TC_RUN(misc_group);
    return tc_end();
}
