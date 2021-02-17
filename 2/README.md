# Level 2

Examine the **x2** file:
```
$ file x2
x2: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, not stripped
```

Run both files:
```
$ ./x2
? test
! -

$ ./p2
> test
] -
```

Both programs wait for user input and then give a response, similar to level 1.

Import the file into Ghidra and analyze it. There is a `_start` function with similar structure to level 1. This `_start` function has a few variables and functions renamed:
```c
void _start(void)
{
  long result;
  int __status;
  
  memset(&buffer,0,0x21);
  printf(&question_mark);
  DAT_006010d0 = fdopen(0,&r_string);
  result = read_user_input(&buffer,0x22,DAT_006010d0);
  if (result != 0) {
    // Compare hashed user input to the global string: 89349536319392163324855876422573
    result = verify_input(&buffer,s_89349536319392163324855876422573_00601065);

    if (result == 0) {
      printf(&exclamation_plus);
      target_func(&buffer); // <== Goal to get here
      __status = 0;
      goto LAB_004005c8;
    }
  }
  printf(&exclamation_minus);
  __status = 1;
LAB_004005c8:
  fclose(DAT_006010d0);
  /* WARNING: Subroutine does not return */
  exit(__status);
}
```

Attempt to recreate the `verify_input` function for further testing. Full source is here: [x2_source.c](x2_source.c). 

The interesting functions are `scramble` and `hash_func`. `scramble` takes in the user input and shuffles it. `hash_func` provides the shuffle index.

`scramble()`
```c
void scramble(char *user_input, char *buffer)
{
    ...
    counter = 0;
    do
    {
        result = hash_func(counter);
        *(buffer + result) = *(user_input + counter);        
        counter = counter + 1;
    } while (counter != 0x20);
}
```

`hash_func()`
```c
unsigned long hash_func(long counter)
{
    unsigned long temp1;
    unsigned long temp2;

    temp1 = (counter + 0x55U & 0x1f) >> 1;
    if ((counter + 0x55U & 1) != 0)
    {
        temp1 = temp1 | 0x10;
    }
    temp2 = temp1 >> 1;
    if ((temp1 & 1) != 0)
    {
        temp2 = temp2 | 0x10;
    }
    temp1 = temp2 >> 1;
    if ((temp2 & 1) != 0)
    {
        temp1 = temp1 | 0x10;
    }
    return temp1 * 3 & 0x1f;
}
```

Using just this `hash_func`, one can predetermine the shuffle order. 
```c
for (int i = 0; i < 0x20; i++)
{
    unsigned long temp;
    temp = hash_func(i);
    printf("%d: %lu\n", i, temp);
}
```

The code above produces the following. On the left is the index and the right is the shuffled index.
```
{
    0: 2,
    1: 14,
    2: 26,
    3: 9,
    4: 21,
    5: 1,
    6: 13,
    7: 25,
    8: 5,
    9: 17,
    10: 29,
    11: 0,
    12: 12,
    13: 24,
    14: 4,
    15: 16,
    16: 28,
    17: 8,
    18: 20,
    19: 3,
    20: 15,
    21: 27,
    22: 7,
    23: 19,
    24: 31,
    25: 11,
    26: 23,
    27: 6,
    28: 18,
    29: 30,
    30: 10,
    31: 22
}
```

Ultimately, the program is looking for a match to this string of numbers:
```
89349536319392163324855876422573
```

Shuffle these numbers according to the shuffle map found. Write a [python script](shuffle.py) to do this automatically:
```python
shuffle_map = {
    0: 2,
    1: 14,
    2: 26,
 ...
}

number = "89349536319392163324855876422573"
shuffled = [number[shuffle_index] for shuffle_index in shuffle_map.values()]
print("".join(shuffled))
```

This prints out:
```
31415926535897932384626433832795
```

Plug this number in for the win.

## Try String "89349536319392163324855876422573" (Fail #1)
Unlike level 1, the comparison string is not the answer:
```
$ ./x2
? 89349536319392163324855876422573     
! -
```

## Jump To `target_func` (Fail #2)
Is it possible to just *jump* to the target function? Run the program in **gdb**.

```
$ gdb x2
(gdb) info functions
All defined functions:

Non-debugging symbols:
...
0x0000000000400500  _start

(gdb) break _start
Breakpoint 1 at 0x400500

(gdb) run
```

Set a breakpoint just before the `verify_input` function gets called. In Ghidra, this function is at address `00400581`:
```
00400581        CALL        verify_input
```

It is the same address in gdb:
```
(gdb) x/40i $rip
=> 0x400500 <_start>:	movabs rdi,0x6010d8
   ...
   0x400581 <_start+129>:	call   0x400648 <_start+328>
```

Set a breakpoint at this address and continue:
```
(gdb) break *0x400581
(gdb) c
Continuing.
? test
=> 0x400581 <_start+129>:	call   0x400648 <_start+328>
...
Breakpoint 2, 0x0000000000400581 in _start ()
```

At this point, the program received the user input and is about to run the `verify_input` function. However, instead of continuing normal execution, change the instruction pointer to the target function. 

Back in Ghidra, the `target_func` gets called address `004005c0`. The code where the exclamation-plus `"! +"` gets printed is right before this at address `004005a5`:
```
004005a5        MOV        RDI=>exclamation_plus,exclamation_plusand altering it
004005b1        CALL       printf
004005b6        MOV        RDI=>buffer,buffer
004005c0        CALL       target_func
```

Set the instruction pointer to address `004005a5` and continue:
```
(gdb) set $rip = 0x004005a5
(gdb) x $rip
=> 0x4005a5 <_start+165>:	movabs rdi,0x601086
(gdb) c
Continuing.
! + C38D793C318C41C21A37EE61D85120C93F7CCB8B01F08BF373E04C486AC4E5CF
[Inferior 1 (process 81065) exited normally]
```

Got a code. Try entering the code in **p2**:
```
$ ./p2
> C38D793C318C41C21A37EE61D85120C93F7CCB8B01F08BF373E04C486AC4E5CF
] --
```

Did not work.

On taking a closer look at `target_func`, it is digesting and altering the user input in order to produce the output code. So it makes sense that incorrect input would produce an incorrect code. Therefore, this approach does not work.