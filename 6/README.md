# Level 6

## _start()

Examine the `_start()` function in Ghidra:

```c
void _start(void)
{
    // prompt
    printf(&s_?);
    file_descriptor = fdopen(0, &s_r);
    // read user input
    result = get_user_input(&buffer_32, 0x21, file_descriptor);
    if (result != (char *)0x0)
    {
        // need to RE this function
        lVar1 = important_func(&buffer, 0x1a, &buffer_32);
        if (lVar1 == 0)
        {
            // clear buffer
            memset(&DAT_006010d1, 0, 0x20);
            // Goal to get here
            printf(&s_ !_ +);
            sVar2 = strlen(&buffer_32);
            target_func1((ulong *)&DAT_006010d1, &buffer_32, sVar2);
            target_func2(&DAT_006010d1);
            __status = 0;
            goto LAB_004005aa;
        }
    }
    printf(&DAT_00601062);
    __status = 1;
LAB_004005aa:
    fclose(file_descriptor);
    exit(__status);
}
```

The goal is to get `important_func()` to return zero so that the program can enter the `if` statement and run the target functions.

## important_func()

```c
// param_0x1a passed hardcoded value 0x1a:
// 0x40050e <_start+94>:	mov    rsi,0x1a
long important_func(char *buffer, ulong param_0x1a, byte *user_input)
{
    long result;
    ulong uVar1;
    // see verify_only_alphabetical() section
    result = verify_only_alphabetical((char *)user_input);
    if (result == 0)
    {
        // see set_buffer_to_all_ones() section
        set_buffer_to_all_ones(buffer, param_0x1a);
        // see add_input_to_binary_tree() section
        result = add_input_to_binary_tree(buffer, param_0x1a, user_input);
        if ((result == 0) &&
            // see verify_buffer_does_not_contain_0xff() section
            (uVar1 = verify_buffer_does_not_contain_0xff(buffer, param_0x1a), 
            uVar1 == 0))
        {
            return 0;
        }
    }
    return -1;
}
```

`important_func` contains several key subfunctions:
* `verify_only_alphabetical()`
* `set_buffer_to_all_ones()`
* `add_input_to_binary_tree()`
* `verify_buffer_does_not_contain_0xff()`

## verify_only_alphabetical()

This function verifies that only lowercase alphabet characters are given in the user input. 

## set_buffer_to_all_ones()

Now examine the `set_buffer_to_all_ones()`. GDB proves that this does as expected:
```
(gdb) x/10wx $rdi-0x20
0x6010eb:       0x00000000      0xffff0000      0xffffffff      0xffffffff
0x6010fb:       0xffffffff      0xffffffff      0xffffffff      0xffffffff
0x60110b:       0x00000000      0x00000000
```

Now back to `target_0xff()` (0x4006ae).

```
(gdb) si
=> 0x4006a3 <_start+499>:       xor    eax,eax
0x7fffffffe050: 0x006010f1      0x00000000      0x00400673      0x00000000
0x7fffffffe060: 0x006010b0      0x00000000      0x0000001a      0x00000000
0x7fffffffe070: 0x006010f1      0x00000000      0x004005f3      0x00000000
0x7fffffffe080: 0x00000000      0x00000000      0xffffe0a0      0x00007fff
0x00000000004006a3 in _start ()
(gdb) i r rax
rax            0x6010f1            6295793
(gdb) x/b $rax
0x6010f1:       0x61
```

`target_0xff()` checks if the all_ones_buffer contains 0xff at some index between 0 and 0x1a inclusive.

Then `target_return_zero2()` places the user_input at the address returned by `target_0xff`.

Still need to test this.

================================
* set up GDB to print all necessary registers
* follow the variables exactly from start to finish..!!
================================

https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions
```
The Microsoft x64 calling convention[18][19] is followed on Windows and pre-boot UEFI (for long mode on x86-64). The first four arguments are placed onto the registers. That means RCX, RDX, R8, R9 for integer, struct or pointer arguments (in that order)
...
Additional arguments are pushed onto the stack (right to left). 
...
Integer return values (similar to x86) are returned in RAX if 64 bits or less
```

Above is for microsoft ^^

For linux:
https://en.wikipedia.org/wiki/X86_calling_conventions#List_of_x86_calling_conventions
```
section: List of x86 calling conventions

registers:
RDI, RSI, RDX, RCX, R8, R9, [XYZ]MM0–7      <== THIS IS THE REGISTER ORDER FOR PARAMS **
```

Ghidra
```
Edit > Tool Options > Listing Fields > Operands Field > uncheck markup register variable references
```

Focus on `target_return_zero` func. Set a breakpoint to just after this function completes:
```
break *0x4005f3
```

Run program. Enter following input:
```
abcdefghijklmno
```

Breakpoint hits. Examine the registers:
```
(gdb) i r rdi rsi rdx rax
rdi            0x6010f1            6295793
rsi            0x1a                26
rdx            0x65                101
rax            0xffffffffffffffff  -1       
```

Rax at -1 indicates incorrect input. 

rsi is set to the counter value 0x1a. rdx is set to 65 'e'. 

Look at where rdi points to:
```
(gdb) x/20wx $rdi
0x6010f1:	0xff62ff61	0xff63ffff	0xffffffff	0xff64ffff
0x601101:	0xffffffff	0xffffffff	0x0000ffff	0x00000000
```

print by bytes:
```
(gdb) x/26bx $rdi
0x6010f1:	0x61	0xff	0x62	0xff	0xff	0xff	0x63	0xff
0x6010f9:	0xff	0xff	0xff	0xff	0xff	0xff	0x64	0xff
0x601101:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601109:	0xff	0xff
```

Seems like one of the functions was placing the user input at specific locations in the `all_ones_buffer`.

```
1       breakpoint     keep y   0x00000000004004b0 <_start>
2       breakpoint     keep y   0x00000000004005f3 <_start+323>
3       breakpoint     keep y   0x0000000000400650 <_start+416>
```

input: bbbbbbbbbbbbbbbbbbbbbbbbb
```
0x6010f1:	0x62	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x6010f9:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601101:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601109:	0xff	0xff
```

input: aaaaaaaaaaaaaaa
```
0x6010f1:	0x61	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x6010f9:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601101:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601109:	0xff	0xff
```

input: zyxwvut
```
0x6010f1:	0x7a	0x79	0xff	0x78	0xff	0xff	0xff	0x77
0x6010f9:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0x76
0x601101:	0xff	0xff	0xff	0xff	0xff	0xff	0xff	0xff
0x601109:	0xff	0xff
```

This gives enough input to test.

Also note, the input does not like duplicate letters. e.g. "aaaaa" and "bbbb" only get entered once. This is because `target_0xff()` checks for `(user_input == byte)`.

The `target_0xff` func increments the counter like this:
```c
counter = counter * 2 + 1; // if (byte_at_counter > user_input)
counter = counter * 2 + 2; // if (byte_at_counter < user_input)
```

Therefore input `bac` produces:
```
0x6010f1:	0x62	0x61	0x63	0xff
...
```

This function is filling a balanced binary tree with letters...

Therefore, create a binary tree with the alphabet and print it out BFS (breadth first search) style:

```
abcdefghijklm n opqrstuvwxyz

abc d ef [g] hijklm [[n]] opqrst [u] vwxyz

nodes:  1  3  7  15  31
levels: 1  2  3   4   5
```

The alphabet w/ 26 chars will have 5 levels

```     
                   p
         h                    w
    d         l          t        y
 b    f     j    n    r    v    x   z
a c  e  g  i k  m o  q s  u
```

This tree is balanced and has all nodes filled from left to right

input `phwdltybfjnrvxzacegikmoqsu`:

```
0x6010f1:	0x70	0x68	0x77	0x64	0x6c	0x74	0x79	0x62
0x6010f9:	0x66	0x6a	0x6e	0x72	0x76	0x78	0x7a	0x61
0x601101:	0x63	0x65	0x67	0x69	0x6b	0x6d	0x6f	0x71
0x601109:	0x73	0x75

(gdb) c
Continuing.
! + 2EE60F74548732808B5FD71EBAA989BA73BF5FB17E55CB436BB80573B7260437
```

Plug in:

```
$ ./p6 
> 2EE60F74548732808B5FD71EBAA989BA73BF5FB17E55CB436BB80573B7260437
@ 7/p7
@ 7/x7
] +
```