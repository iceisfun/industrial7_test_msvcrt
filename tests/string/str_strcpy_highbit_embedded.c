/* TC
 * id:    str_strcpy_highbit_embedded
 * area:  string
 * funcs: strcpy strncpy strcat strncat
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.24.2.3, 7.24.2.4, 7.24.3.1, 7.24.3.2 (bytes treated as unsigned char)
 */
#include "tc.h"
#include <string.h>

static char src[256];
static char dst[600];
static volatile char *vs = src, *vd = dst;

static void fill(void) { int i; for (i = 0; i < 255; i++) src[i] = (char)(i + 1); src[255] = 0; }

TC_CASE(strcpy_all)
{
    fill(); memset(dst, 0, sizeof dst);
    strcpy((char *)vd, (const char *)vs);
    CHECK_MEM(dst, src, 256);
}
TC_CASE(strncpy_all)
{
    fill(); memset(dst, 0x7f, sizeof dst);
    strncpy((char *)vd, (const char *)vs, 300);
    CHECK_MEM(dst, src, 256);
    CHECK_EQ_I(dst[299], 0);
    CHECK_EQ_I(dst[300], 0x7f);
}
TC_CASE(strcat_all)
{
    fill(); memset(dst, 0, sizeof dst);
    strcat((char *)vd, (const char *)vs);
    strcat((char *)vd, (const char *)vs);
    CHECK_MEM(dst, src, 255);
    CHECK_MEM(dst + 255, src, 256);
}
TC_CASE(strncat_all)
{
    fill(); memset(dst, 0, sizeof dst);
    strncat((char *)vd, (const char *)vs, 1000);
    strncat((char *)vd, (const char *)vs, 100);
    CHECK_MEM(dst, src, 255);
    CHECK_MEM(dst + 255, src, 100);
    CHECK_EQ_I(dst[355], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(strcpy_all); TC_RUN(strncpy_all); TC_RUN(strcat_all); TC_RUN(strncat_all);
    return tc_end();
}
