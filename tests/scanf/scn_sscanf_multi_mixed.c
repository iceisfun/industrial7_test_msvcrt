/* TC
 * id:    scn_sscanf_multi_mixed
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(date) { int a=0,b=0,c=0,d=0,e=0,f=0; int n; v = "2024-03-05 12:30:45"; n = sscanf((const char *)v, "%d-%d-%d %d:%d:%d", &a,&b,&c,&d,&e,&f); CHECK_EQ_I(n, 6); CHECK_EQ_I(a,2024); CHECK_EQ_I(b,3); CHECK_EQ_I(c,5); CHECK_EQ_I(d,12); CHECK_EQ_I(e,30); CHECK_EQ_I(f,45); }
TC_CASE(name_age) { char k[16], val[16], k2[16]; int age = 0; int n; v = "name=bob age=42"; n = sscanf((const char *)v, "%[^=]=%s %[^=]=%d", k, val, k2, &age); CHECK_EQ_I(n, 4); CHECK_STR(k, "name"); CHECK_STR(val, "bob"); CHECK_STR(k2, "age"); CHECK_EQ_I(age, 42); }
TC_CASE(key_value) { char k[16], val[16]; int n; v = "key: value"; n = sscanf((const char *)v, "%[^:]: %s", k, val); CHECK_EQ_I(n, 2); CHECK_STR(k, "key"); CHECK_STR(val, "value"); }
TC_CASE(ip) { int a=0,b=0,c=0,d=0; int n; v = "192.168.1.10"; n = sscanf((const char *)v, "%d.%d.%d.%d", &a,&b,&c,&d); CHECK_EQ_I(n, 4); CHECK_EQ_I(a,192); CHECK_EQ_I(b,168); CHECK_EQ_I(c,1); CHECK_EQ_I(d,10); }
TC_CASE(percent) { int a = 0, pos = -1; int n; v = "10%"; n = sscanf((const char *)v, "%d%%%n", &a, &pos); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 10); CHECK_EQ_I(pos, 3); }
int main(void) { tc_begin(); TC_RUN(date); TC_RUN(name_age); TC_RUN(key_value); TC_RUN(ip); TC_RUN(percent); return tc_end(); }
