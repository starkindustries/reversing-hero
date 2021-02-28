
Open in Ghidra. Break at the `_start()` function.

Try jumping to the target function in gdb: 
```
(gdb) set $rip=0x400428
(gdb) c
Continuing.
! + 192C51FB19EE1D504ACF59BC97BBD66B524BD6E4A27509D3B028437AA1A43E21
[Inferior 1 (process 113792) exited normally]
```

Unfortunately, this is an incorrect code:
```
$ ./p5
> 192C51FB19EE1D504ACF59BC97BBD66B524BD6E4A27509D3B028437AA1A43E21
] --
```

There are three if statements. Depending on which `if` failed, the program will print out a particular status: 
```c
! ---   // failed first if statement
! --    // failed second if statement
! -     // failed third if statement
```

```c
void _start(void)

{
  long result1;
  long result2;
  ulong uVar1;
  int __status;
  
  result1 = func1((long *)&global_hex_data,0x400);
  target_func_param_func((long)&global_hex_data,0x400);
  result2 = func1((long *)&global_hex_data,0x400);
  if (result2 == result1) {
    result1 = FUN_00400512((long)&global_hex_data,0x400);
    if (result1 == 0) {
      result1 = FUN_004004f1((byte *)target_func_param_func,0x52);
      uVar1 = (long)(result1 - 0x111U) >> 0x3f;
      if ((result1 - 0x111U ^ uVar1) - uVar1 == 1) {
        /* Goal to get here */
        memset(&empty_mem,0,0x20);
        target_func((ulong *)&empty_mem,(byte *)target_func_param_func,0x52);
        printf(&s_!_+);
        print_code(&empty_mem);
        __status = 0;
      }
      else {
        printf(&s_!_-);
        __status = 3;
      }
    }
    else {
      printf(s_!_--);
      __status = 2;
    }
  }
  else {
    printf(s_!_---);
    __status = 1;
  }
  exit(__status);
}
```

x86-64 Instruction Reference:
```
LODS - Load String      Load qword at address (R)SI into RAX.   p.3-539
XCHG - Exchange Register/Memory     Exchanges the contents of the destination (first) and source (second) operands. The operands can be two general purpose registers or a register and a memory location.   p.5-586
```

Breakpoint references:
```
0x4004da    func1()
0x400340    target_func_param_func()
```

```c
long func1(long *hex_data,long counter)
{
  long lVar1;
  
  lVar1 = 0;
  do {
    lVar1 = lVar1 + *hex_data;
    counter = counter + -1;
    hex_data = hex_data + 1;
  } while (counter != 0);
  return lVar1;
}
```

Examine instruction _start+338
```
0x4004e4 <_start+338>   lods   rax,QWORD PTR ds:[rsi] 
```

Before instruction _start+338:
```
(gdb) i r rax rsi
rax            0x0                 0
rsi            0x601147            6295879
(gdb) x/10wx $rsi
0x601147:       0x327aebf4      0xffcdef97      0x067bb7a5      0xff99e42f
...
```

After instruction +start+338:
```
(gdb) i r rax rsi
rax            0xffcdef97 327aebf4      -14091791146226700
rsi            0x60114f                 6295887
```

Notes: 
* The `lods` instruction also incremented `$rsi` by eight places because a long is 64 bits long. 
* The number from $rsi is flipped when stored in $rax due to endianness.
* The $ecx register is not explicitly written in the instruction, however, it is decremented in the `loop` instruction

While looking at the `global_hex_data` variable, ghidra shows that this data starts at address 0x601147 and ends at address 0x603146. Do some hex math:
```python
>>> x = 0x603146-0x601147+1
>>> x
8192        # number of bytes
>>> x/8
1024        # number of longs (8 bytes each)
```

This is in line with what the function is trying to do.

```
side note: 
0x1CEDC0FFEE
0xC1CADA3301
```

## `target_func_param_func()`
