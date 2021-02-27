

https://stackoverflow.com/questions/20326025/linux-assembly-how-to-call-syscall

https://refspecs.linuxfoundation.org/elf/x86_64-abi-0.99.pdf

p.124
1. User-level applications use as integer registers for passing the sequence
%rdi, %rsi, %rdx, %rcx, %r8 and %r9. The kernel interface uses %rdi,
%rsi, %rdx, %r10, %r8 and %r9.
2. A system-call is done via the syscall instruction. The kernel destroys
registers %rcx and %r11.
3. The number of the syscall has to be passed in register %rax.
4. System-calls are limited to six arguments, no argument is passed directly on
the stack.
5. Returning from the syscall, register %rax contains the result of the
system-call. A value in the range between -4095 and -1 indicates an error,
it is -errno.
6. Only values of class INTEGER or class MEMORY are passed to the kernel.


```
google: linux x86_64 syscall table
```
https://filippo.io/linux-syscall-table/
```
open    sys_open	fs/open.c
%rax:   2	
%rdi:   const char __user * filename
%rsi:   int flags	
%rdx:   umode_t mode		
```

gdb:
```
0x400208 <_start>:	    mov    rax,0x2
0x40020f <_start+7>:	movabs rdi,0x601000
0x400219 <_start+17>:	mov    rsi,0x0
0x400220 <_start+24>:	syscall 
```



Continue stepping. Check the rax register after the syscall:
```
(gdb) info registers rax
rax            0xfffffffffffffffe  -2
```

https://stackoverflow.com/a/40386730/2179970
```
On Linux, a failed system call using the syscall assembly instruction will return the value -errno in the rax register. So in your case 0-0xfffffffffffffff2 == 0xE which is 14. So your errno is 14.

How do you find what errno 14 means? You should google search "Linux error code table" or look in errno.h and you'll find the answer.
```

https://www.thegeekstuff.com/2010/10/linux-error-codes/
Error number 2 is:
```
2	ENOENT	No such file or directory
```

what file was the program looking for?
```
(gdb) x/s 0x601000
0x601000:	"d.d"
```

The file name is `d.d`. Create a file with this name. Run it again. Now the program goes further, but again only outputs:
```
! --
```

```
(gdb) x/50i $rip
0x400208 <_start>:	    mov    rax,0x2                  ; setup sys_open syscall
0x40020f <_start+7>:	movabs rdi,0x601000             ; select file name at address 0x601000 "d.d"
0x400219 <_start+17>:	mov    rsi,0x0                  ; set flags to 0 for this syscall
0x400220 <_start+24>:	syscall 
0x400222 <_start+26>:	test   rax,rax                  ; test rax, which contains the return value
0x400225 <_start+29>:	mov    r13,0x1                  ; move 1 into register r13
0x40022c <_start+36>:	js     0x4002e4 <_start+220>    ; js = jump if signed (negative)
0x400232 <_start+42>:	mov    r12,rax                  ; store rax into r12
0x400235 <_start+45>:	movabs rdi,0x601038             ; 0x601038 is a 32-bit buffer
0x40023f <_start+55>:	call   0x400457 <_start+591>    ; this functions clears the 32-bit buffer
0x400244 <_start+60>:	mov    rdi,r12                  ; store r12 (which contains rax) into rdi
0x400247 <_start+63>:	movabs rsi,0xdeadfacedeadbeef   ; store 0xdeadfacedeadbeef into rsi
0x400251 <_start+73>:	movabs rdx,0x123456701234567    ; store 0x123456701234567 into rdx
0x40025b <_start+83>:	mov    rcx,0x80                 ; store 0x80 into rcx
0x400262 <_start+90>:	movabs r8,0x601038              ; store 32-bit buffer address into r8
0x40026c <_start+100>:	call   0x4003e4 <_start+476>    ; call the verify function
```

Examine the function at 0x4003e4:
```
(gdb) x/50i  0x4003e4
0x4003e4 <_start+476>:	push   r12                      ; push r12-r15 and rbp onto the stack
0x4003e6 <_start+478>:	push   r13
0x4003e8 <_start+480>:	push   r14
0x4003ea <_start+482>:	push   r15
0x4003ec <_start+484>:	push   rbp
0x4003ed <_start+485>:	sub    rsp,0x10                 ; Make space on the stack to store params
0x4003f1 <_start+489>:	mov    QWORD PTR [rsp],rsi      ; move rsi (0xdeadfacedeadbeef) onto the stack
0x4003f5 <_start+493>:	mov    DWORD PTR [rsp+0x8],0x40 ; move 0x40 onto the stack
```

After this instruction examine the current stack:
```
=> 0x4003fd <_start+501>:	mov    r12,rdi      ; current instruction
; current stack:
0x7fffffffe000:	0xdeadbeef	0xdeadface	0x00000040	0x00000000
0x7fffffffe010:	0x00000000	0x00000000	0x00000000	0x00000000
```

Continue analyzing the function:
```
0x4003fd <_start+501>:	mov    r12,rdi                  ; rdi contains the rax value from _start+60 and _start+42
0x400400 <_start+504>:	mov    r13,rdx                  ; rdx contains 0x123456701234567 from _start+73
0x400403 <_start+507>:	mov    r14,rcx                  ; rcx contains 0x80 from _start+83
0x400406 <_start+510>:	mov    r15,r8                   ; r8 contains the 32-bit buffer from _start+90
0x400409 <_start+513>:	mov    rdi,r12                  ; move r12 back into rdi again? why?
0x40040c <_start+516>:	call   0x40033e <_start+310>    ; call another function
```

Another function:
```
(gdb) x/10i $rip
0x40033e <_start+310>:	push   rax
0x40033f <_start+311>:	mov    rax,0x0                  ; request syscall 0: read
0x400346 <_start+318>:	mov    rsi,rsp                  ; buffer
0x400349 <_start+321>:	mov    rdx,0x1                  ; size to read: 0x1 (one 64-bit chunk)
0x400350 <_start+328>:	syscall                         ; execute syscall
0x400352 <_start+330>:	xor    rdi,rdi                  ; clear rdi register (set to 0)
0x400355 <_start+333>:	cmp    rax,0x1                  ; check if rax is 1
0x400359 <_start+337>:	je     0x40035f <_start+343>    ; jmp if rax == 1
0x40035b <_start+339>:	or     rdi,0xffffffffffffffff   ; if not, set rdi to -1
0x40035f <_start+343>:	pop    rax                      ; pop top of stack into rax (the byte read)
0x400360 <_start+344>:	movzx  rax,al
0x400364 <_start+348>:	or     rax,rdi                  ; if rdi was -1 then rax would be corrupted
0x400367 <_start+351>:	ret    
```

Note that the `read` syscall above uses register `rdi` as the file descriptor param (https://filippo.io/linux-syscall-table/). The `rdi` register was already previously set before the function call to the `rax` value of 0x3.

https://unix.stackexchange.com/a/41422/387369
```
There are three standard file descriptions, STDIN, STDOUT, and STDERR. They are assigned to 0, 1, and 2 respectively.
```

3 would be the next available file descriptor for the `d.d` file. The program reads in the first character of the file.

Now we're back to the previous function:

```
0x400411 <_start+521>:	mov    rbp,rax                  ; store rax (the char) into rbp
0x400414 <_start+524>:	or     rax,0xffffffffffffffff   ; set rax to -1
0x400418 <_start+528>:	cmp    rbp,rax                  ; check if the char == -1
0x40041b <_start+531>:	je     0x400449 <_start+577>    ; if -1, jump to end
0x40041d <_start+533>:	mov    rdi,r15                  ; move 32-bit buffer into rdi
0x400420 <_start+536>:	mov    rsi,rbp                  ; move the char into rsi
0x400423 <_start+539>:	call   0x4004f0 <_start+744>    ; call function
```

`scramble` function 0x4004f0
```
(gdb) x/50i $rip
0x4004f0 <_start+744>:	mov    r8,QWORD PTR [rdi]       ; rdi has the 32-bit buffer
0x4004f3 <_start+747>:	mov    r9,QWORD PTR [rdi+0x8]   
0x4004f7 <_start+751>:	mov    r10,QWORD PTR [rdi+0x10]
0x4004fb <_start+755>:	mov    r11,QWORD PTR [rdi+0x18]
0x4004ff <_start+759>:	mov    rcx,0x100                ; 0x100 = 256
0x400506 <_start+766>:	movzx  rsi,sil                  ; sil is rsi's lower portion
0x40050a <_start+770>:	movabs rax,0xb010c150e26d857b
0x400514 <_start+780>:	xor    r8,rax
0x400517 <_start+783>:	add    r8,rsi                   ; rsi still contains the char
0x40051a <_start+786>:	bswap  r8                       ; byte swap
```

Byte Swap reverses the order of the bytes:
https://c9x.me/x86/html/file_module_x86_id_21.html
```0xdeadfacedeadbeef
            |  .--------|--|--'
            |  |  .-----|--'
            |  |  |  .--'
            v  v  v  v
r8      0x e3 85 6d e2 50 c1 10 b0                      ; after byte swap
```

Continuing back to function:
```
0x40051d <_start+789>:	rol    r8,0x7                   ; rol = rotate left
0x400521 <_start+793>:	movabs rax,0x4526e76d6d799032
0x40052b <_start+803>:	xor    r9,rax
0x40052e <_start+806>:	add    r9,r8
0x400531 <_start+809>:	bswap  r9
0x400534 <_start+812>:	rol    r9,0x17
0x400538 <_start+816>:	movabs rax,0xa27c2069adb0e0c3
0x400542 <_start+826>:	xor    r10,rax
0x400545 <_start+829>:	add    r10,r90xdeadfacedeadbeef
0x400548 <_start+832>:	bswap  r10
0x40054b <_start+835>:	rol    r10,0x29
0x40054f <_start+839>:	movabs rax,0xf0d6ea65fdb172b6
0x400559 <_start+849>:	xor    r11,rax
0x40055c <_start+852>:	add    r11,r10
0x40055f <_start+855>:	bswap  r11
0x400562 <_start+858>:	rol    r11,0x2f
0x400566 <_start+862>:	mov    rsi,r11
0x400569 <_start+865>:	loop   0x40050a <_start+770>
0x40056b <_start+867>:	add    QWORD PTR [rdi],r8
0x40056e <_start+870>:	add    QWORD PTR [rdi+0x8],r9
0x400572 <_start+874>:	add    QWORD PTR [rdi+0x10],r10
0x400576 <_start+878>:	add    QWORD PTR [rdi+0x18],r11
0x40057a <_start+882>:	ret
```

```
0x400428 <_start+544>:	mov    rdi,rsp
0x40042b <_start+547>:	mov    rsi,rbp
0x40042e <_start+550>:	call   0x400368 <_start+352>
0x400433 <_start+555>:	test   rax,rax
0x400436 <_start+558>:	jne    0x400449 <_start+577>
0x400438 <_start+560>:	xor    eax,eax
0x40043a <_start+562>:	cmp    QWORD PTR [rsp],r13
0x40043e <_start+566>:	je     0x400449 <_start+577>
0x400440 <_start+568>:	dec    r14
0x400443 <_start+571>:	jne    0x400409 <_start+513>
0x400445 <_start+573>:	or     rax,0xffffffffffffffff
0x400449 <_start+577>:	add    rsp,0x10
0x40044d <_start+581>:	pop    rbp
0x40044e <_start+582>:	pop    r15
0x400450 <_start+584>:	pop    r14
0x400452 <_start+586>:	pop    r13
0x400454 <_start+588>:	pop    r12
0x400456 <_start+590>:	ret    
0x400457 <_start+591>:	xor    eax,eax
0x400459 <_start+593>:	mov    rcx,0x20
0x400460 <_start+600>:	rep stos BYTE PTR es:[rdi],al
0x400462 <_start+602>:	ret 
```

## _start Function
```c
undefined  [16] _start(void)
{
  long lVar1;
  long lVar2;
  
  syscall();
  clear_buffer(&G_BUFFER32);
  lVar1 = verify(2,0xdeadfacedeadbeef,0x123456701234567,0x80,(ulong *)&G_BUFFER32);
  lVar2 = 2;
  if (lVar1 == 0) {
    lVar2 = 0;
                    /* print ReversingHero string */
    syscall();
    target_func((long)&G_BUFFER32,0x20);
    syscall();
  }
  syscall();
  if (lVar2 == 1) {
    syscall();
  }
  if (lVar2 == 2) {
    syscall();
  }
  syscall();
  syscall();
  return CONCAT88(1,0xffffffffffffffff);
}
```

To set a string in memory with gdb:
```
(gdb) x/s $rdi
0x601038:	""

(gdb) set {char[32]} $rdi = "hello_world"
(gdb) x/s $rdi
0x601038:	"hello_world"
```

This local var is located at address `deadbeef + 1` or `$rdi + 0x8`. Need to investigate this closer. This is used inside the unknown_func. Look at asm instruction:
```
00400392 ff 4f 08        DEC        dword ptr [rdi + 0x8]
```

and in ghidra output:
```
ulong local_30 = 0x40;
```


location at $rdi+0x8 contains the 0x40 variable. note: still inside the unknown_func(). the 0x40 var is another input to this function.
```
=> 0x40037a <_start+370>:       mov    ecx,DWORD PTR [rdi+0x8]
0x7fffffffe058: 0x00400433      0x00000000      0xdeadbeef      0xdeadface
0x7fffffffe068: 0x00000040      0x00000000      0x00000000      0x00000000
0x7fffffffe078: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe088: 0x00000001      0x00000000      0x00000003      0x00000000
0x000000000040037a in _start ()
(gdb) i r rdi
rdi            0x7fffffffe060      140737488347232
(gdb) x $rdi+0x8
0x7fffffffe068: 0x00000040
(gdb) x/10wx $rsp
0x7fffffffe058: 0x00400433      0x00000000      0xdeadbeef      0xdeadface
0x7fffffffe068: 0x00000040      0x00000000      0x00000000      0x00000000
0x7fffffffe078: 0x00000000      0x00000000
```

the rotate left/right rotates by the amount in the counter variable. 

The shift right and shift left will clear the last bit in rax:
```
(gdb) i r rax
rax            0xdeadfacedeadbeef  -2400987259912470801
(gdb) si
=> 0x400389 <_start+385>:       shl    rax,1
0x7fffffffe058: 0x00400433      0x00000000      0xdeadbeef      0xdeadface
0x7fffffffe068: 0x00000040      0x00000000      0x00000000      0x00000000
0x7fffffffe078: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe088: 0x00000001      0x00000000      0x00000003      0x00000000
0x0000000000400389 in _start ()
(gdb) i r rax
rax            0x6f56fd676f56df77  8022878406898540407
(gdb) si
=> 0x40038c <_start+388>:       ror    rax,cl
0x7fffffffe058: 0x00400433      0x00000000      0xdeadbeef      0xdeadface
0x7fffffffe068: 0x00000040      0x00000000      0x00000000      0x00000000
0x7fffffffe078: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe088: 0x00000001      0x00000000      0x00000003      0x00000000
0x000000000040038c in _start ()
(gdb) i r rax
rax            0xdeadfacedeadbeee  -2400987259912470802
```


Traps I fell into.. Trusting ghidra source too much. Not considering additional variables outside of the given ghidra source: for example the counter 0x40 for the unknown_func()

Running the unknown func up to 64 times clears the buffer. This was run using x4_source:
```
for (int i = 0; i < 100; i++)
{
    char1 = 0x30;
    printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}\n", deadbeef, char1, counter);
    ulong result = unknown_func(&deadbeef, char1, &counter);
    printf("deadbeef: {0x%02lx}, char: {0x%02lx}, counter: {%d}, result: {%ld}\n\n", deadbeef, char1, counter, result);
}
```

Try running this on the actual program:
```
$ python
...
>>> "0"*64
'0000000000000000000000000000000000000000000000000000000000000000'
```

Theory seems to be correct:
```
Breakpoint 2, 0x0000000000400368 in _start ()
Continuing.
=> 0x400433 <_start+555>:       test   rax,rax
0x7fffffffe060: 0xde000000      0xdeadface          <== stack (deadbeef value)

Breakpoint 4, 0x0000000000400433 in _start ()
(gdb) i r ecx
ecx            0x29                41               <== counter value
```

```
0x30 (0) clears the bit at counter index and then decrements counter
0x31 (1) clears the bit at counter index and then increments counter
0x31 (2) sets the bit at counter index and then increments counter

Desired target is:
0x123456701234567

Convert that number to 64-bit binary:
0000000100100011010001010110011100000001001000110100010101100111

Then the desired commands should create this binary number. 
First clear the long with 64x 0's. 
Then set or clear the next 64 items based on the desired target.

Python to do this:
>>> target = "0000000100100011010001010110011100000001001000110100010101100111"
>>> target = target.replace('1', '2').replace('0', '1')
'1111111211211122121112121221122211111112112111221211121212211222'
>>> 64 * "0" + target
00000000000000000000000000000000000000000000000000000000000000001111111211211122121112121221122211111112112111221211121212211222
```

Bingo:
```
$ ./x4
! + 14EDAB4DB6967DC9B0E4194648C5AE8829E9AB95D165E698509809B6D2A44C17
$ ./p4
> 14EDAB4DB6967DC9B0E4194648C5AE8829E9AB95D165E698509809B6D2A44C17
@ 5/p5
@ 5/x5
] +
```