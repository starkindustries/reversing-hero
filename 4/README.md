

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
0x40026c <_start+100>:	call   0x4003e4 <_start+476>    ; call function
```


