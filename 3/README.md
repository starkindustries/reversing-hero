# Level 3

Open **x3** in Ghidra. Study the `_start` function.

```c
void _start(void)

{
  uint result;
  long lVar1;
  long extraout_RDX;
  long extraout_RDX_00;
  long extraout_RDX_01;
  int __status;
  
  printf(&question_mark);
  file_pointer = fdopen(0,&r);
  result = read_input(file_pointer);
  if (extraout_RDX == 0) {
    g_result1 = result;
    printf(&question_mark);
    result = read_input(file_pointer);
    if (extraout_RDX_00 == 0) {
      g_result2 = result;
      printf(&question_mark);
      result = read_input(file_pointer);
      if (extraout_RDX_01 == 0) {
        g_result3 = result;
        lVar1 = unknown_func((ulong)g_result1,(ulong)g_result2,(ulong)result);
        if (lVar1 == 0) {
          printf(&exclamation_plus);
          target_func(&g_result1,0xc);
          __status = 0;
          goto LAB_00400515;
        }
      }
    }
  }
  printf(&exclamation_minus);
  __status = 1;
LAB_00400515:
  fclose(file_pointer);
                    /* WARNING: Subroutine does not return */
  exit(__status);
}
```