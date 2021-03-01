
https://www.youtube.com/watch?v=LyNyf3UM9Yc&ab_channel=LiveOverflow

https://github.com/NationalSecurityAgency/ghidra/issues/19

Level 6 contains a bunch of functions:
```
0x4004b0    _start()
0x4005be    important_func()
0x400650    target_return_zero()
0x40068c    target_return_zero2()
0x4006ae    target_0xff()
0x400615    verify_only_alphabetical()
0x400632    set_buffer_to_all_ones()
```

All of the above functions are embedded in another like Russian
[Matryoshka dolls](https://en.wikipedia.org/wiki/Matryoshka_doll). Function `_start()` calls `important_func()`, which calls calls `target_return_zero()`, which calls `target_return_zero2()`, which calls `target_0xff()`.

** TODO: come back to these functions **

Start with `verify_only_alphabetical()` function. This function verifies that only lowercase alphabet characters are given in the user input. 

Now examine the `set_buffer_to_all_ones()`. GDB proves that this does as expected:
```
(gdb) x/10wx $rdi-0x20
0x6010eb:       0x00000000      0xffff0000      0xffffffff      0xffffffff
0x6010fb:       0xffffffff      0xffffffff      0xffffffff      0xffffffff
0x60110b:       0x00000000      0x00000000
```

Now back to `target_0xff()`.