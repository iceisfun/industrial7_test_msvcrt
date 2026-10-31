/* TC
 * id:    sec_putenv_s_win32env_probe
 * area:  secure_crt
 * funcs: _putenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _putenv_s
 * note:  PROBE: _putenv_s also updates the Win32 process environment (GetEnvironmentVariableA sees the value; empty value removes it). Believed true for all these CRTs, not spelled out for _putenv_s.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

TC_CASE(win32_sees_value) { char b[16]; CHECK_EQ_I(_putenv_s("TC_SEC_VAR4", "abc"), 0); CHECK_EQ_U(GetEnvironmentVariableA("TC_SEC_VAR4", b, sizeof b), 3); CHECK_STR(b, "abc"); }
TC_CASE(win32_empty_removes) { char b[16]; _putenv_s("TC_SEC_VAR4", "abc"); CHECK_EQ_I(_putenv_s("TC_SEC_VAR4", ""), 0); CHECK_EQ_U(GetEnvironmentVariableA("TC_SEC_VAR4", b, sizeof b), 0); }
int main(void) { tc_begin(); TC_RUN(win32_sees_value); TC_RUN(win32_empty_removes); return tc_end(); }
