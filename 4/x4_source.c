#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <byteswap.h>

#define INT_BITS 64 

typedef unsigned long ulong;
  

// Function to left rotate n by d bits
ulong rol(ulong n, unsigned int d) 
{ 
    // In n<<d, last d bits are 0. To put first 3 bits of n at  
    // last, do bitwise or of n<<d with n >>(INT_BITS - d)
    return (n << d)|(n >> (INT_BITS - d)); 
} 


// Function to right rotate n by d bits
ulong ror(ulong n, unsigned int d) 
{ 
    // In n>>d, first d bits are 0. To put last 3 bits of n at  
    // first, do bitwise or of n>>d with n <<(INT_BITS - d) 
    return (n >> d)|(n << (INT_BITS - d)); 
} 


// Function starts at: 0x00400368 <_start+352>
void unknown_func(ulong *rdi, ulong rsi)
{
    rsi -= 0x30;                        // 0x400368 <_start+352>:	sub    rsi,0x30
    if (rsi == 0)                       // 0x40036c <_start+356>:	je     0x40037a <_start+370>
    {
        goto LABEL_370;
    }
    else {
        rsi -= 1;                       // 0x40036e <_start+358>:	dec    rsi
        if (rsi == 0)                   // 0x400371 <_start+361>:	je     0x400397 <_start+399>
        {
            goto LABEL_399;
        }
        else
        {
            rsi -= 1;                   // 0x400373 <_start+363>:	dec    rsi
            if (rsi == 0)               // 0x400376 <_start+366>:	je     0x4003b8 <_start+432>
            {
                goto LABEL_432;
            }
            else
            {
                goto LABEL_471;         // 0x400378 <_start+368>:	jmp    0x4003df <_start+471>
            }            
        }        
    }
    
    uint ecx;
    ulong rax;

    LABEL_370:
    ecx = rdi[1];                       // 0x40037a <_start+370>:	mov    ecx,DWORD PTR [rdi+0x8]    

    // The instruction `jecxz` is jump if ECX register is zero
    if (ecx == 0)                       // 0x40037d <_start+373>:	jecxz  0x4003df <_start+471>
    {
        goto LABEL_471;
    }
    else
    {
        rax = rdi[0];                   // 0x400380 <_start+376>:	mov    rax,QWORD PTR [rdi]
        rax = rol(rax, ecx & 0xff);     // 0x400383 <_start+379>:	rol    rax,cl
        rax = rax >> 1;                 // 0x400386 <_start+382>:	shr    rax,1
        rax = rax << 1;                 // 0x400389 <_start+385>:	shl    rax,1
        rax = ror(rax, ecx & 0xff);     // 0x40038c <_start+388>:	ror    rax,cl
        rdi[0] = rax;                   // 0x40038f <_start+391>:	mov    QWORD PTR [rdi],rax
        rdi[1] -= 1;                    // 0x400392 <_start+394>:	dec    DWORD PTR [rdi+0x8]
        goto LABEL_467;                 // 0x400395 <_start+397>:	jmp    0x4003db <_start+467>
    }

    LABEL_399:
    ecx = rdi[1];                       // 0x400397 <_start+399>:	mov    ecx,DWORD PTR [rdi+0x8]
    if (ecx >= 0x40)                    // 0x40039a <_start+402>:	cmp    ecx,0x40
    {
        // Instruction `jae` is jump if above or equal
        goto LABEL_471;                 // 0x40039d <_start+405>:	jae    0x4003df <_start+471>
    }
    else
    {
        rax = rdi[0];                   // 0x40039f <_start+407>:	mov    rax,QWORD PTR [rdi]
        ecx += 1;                       // 0x4003a2 <_start+410>:	inc    ecx
        rax = rol(rax, ecx & 0xff);     // 0x4003a4 <_start+412>:	rol    rax,cl
        rax = rax >> 1;                 // 0x4003a7 <_start+415>:	shr    rax,1
        rax = rax << 1;                 // 0x4003aa <_start+418>:	shl    rax,1
        rax = ror(rax, ecx & 0xff);     // 0x4003ad <_start+421>:	ror    rax,cl
        rdi[0] = rax;                   // 0x4003b0 <_start+424>:	mov    QWORD PTR [rdi],rax
        rdi[1] = ecx;                   // 0x4003b3 <_start+427>:	mov    DWORD PTR [rdi+0x8],ecx
        goto LABEL_467;                 // 0x4003b6 <_start+430>:	jmp    0x4003db <_start+467>
    }


    LABEL_432:
    ecx = rdi[1];                       // 0x4003b8 <_start+432>:	mov    ecx,DWORD PTR [rdi+0x8]
    if (ecx >= 0x40)                    // 0x4003bb <_start+435>:	cmp    ecx,0x40
    {
        goto LABEL_471;                 // 0x4003be <_start+438>:	jae    0x4003df <_start+471>
    }
    else
    {
        rax = rdi[0];                   // 0x4003c0 <_start+440>:	mov    rax,QWORD PTR [rdi]
        ecx += 1;                       // 0x4003c3 <_start+443>:	inc    ecx
        rax = rol(rax, ecx & 0xff);     // 0x4003c5 <_start+445>:	rol    rax,cl
        rax = rax >> 1;                 // 0x4003c8 <_start+448>:	shr    rax,1
        rax = rax << 1;                 // 0x4003cb <_start+451>:	shl    rax,1
        rax = rax | 0x1;                // 0x4003ce <_start+454>:	or     rax,0x1
        rax = ror(rax, ecx & 0xff);     // 0x4003d2 <_start+458>:	ror    rax,cl
        rdi[0] = rax;                   // 0x4003d5 <_start+461>:	mov    QWORD PTR [rdi],rax
        rdi[1] = ecx;                   // 0x4003d8 <_start+464>:	mov    DWORD PTR [rdi+0x8],ecx
    }

    LABEL_467:
    // clear the eax register (lower half of rax)
    rax = rax & 0x00000000;             // 0x4003db <_start+467>:	xor    eax,eax
    goto LABEL_475;                     // 0x4003dd <_start+469>:	jmp    0x4003e3 <_start+475>

    LABEL_471:
    // Set rax to -1
    rax = -1;                           // 0x4003df <_start+471>:	or     rax,0xffffffffffffffff
    
    LABEL_475:
    return;                             // 0x4003e3 <_start+475>:	ret 
}

// Function starts at: 0x004004f0 <_start+744>
void scramble(char *buffer32, ulong rsi)
{    
    // rdi contains the 32-bit buffer address
    ulong r8  = buffer32[0];            // 0x4004f0 <_start+744>:	mov    r8,QWORD PTR [rdi]
    ulong r9  = buffer32[1];            // 0x4004f3 <_start+747>:	mov    r9,QWORD PTR [rdi+0x8]
    ulong r10 = buffer32[2];            // 0x4004f7 <_start+751>:	mov    r10,QWORD PTR [rdi+0x10]
    ulong r11 = buffer32[3];            // 0x4004fb <_start+755>:	mov    r11,QWORD PTR [rdi+0x18]
    
    int counter = 0x100;                // 0x4004ff <_start+759>:	mov    rcx,0x100
    
    // only keep the last byte (i.e. the 8-bit ASCII character portion)
    rsi = rsi & 0xff;                   // 0x400506 <_start+766>:	movzx  rsi,sil

    do {    
        // *** R8 ***
        r8 = r8 ^ 0xb010c150e26d857b;   // 0x40050a <_start+770>:	movabs rax,0xb010c150e26d857b
                                        // 0x400514 <_start+780>:	xor    r8,rax
        r8 = r8 + rsi;                  // 0x400517 <_start+783>:	add    r8,rsi
        __bswap_64(r8);                 // 0x40051a <_start+786>:	bswap  r8        
        r8 = rol(r8, 0x7);      // 0x40051d <_start+789>:	rol    r8,0x7
        
        // *** R9 ***
        r9 = r9 ^ 0x4526e76d6d799032;   // 0x400521 <_start+793>:	movabs rax,0x4526e76d6d799032
                                        // 0x40052b <_start+803>:	xor    r9,rax
        r9 = r9 + r8;                   // 0x40052e <_start+806>:	add    r9,r8
        __bswap_64(r9);                 // 0x400531 <_start+809>:	bswap  r9
        r9 = rol(r9, 0x17);     // 0x400534 <_start+812>:	rol    r9,0x17
        
        // *** R10 ***
        r10 = r10 ^ 0xa27c2069adb0e0c3; // 0x400538 <_start+816>:	movabs rax,0xa27c2069adb0e0c3
                                        // 0x400542 <_start+826>:	xor    r10,rax
        r10 = r10 + r9;                 // 0x400545 <_start+829>:	add    r10,r9
        __bswap_64(r10);                // 0x400548 <_start+832>:	bswap  r10
        r10 = rol(r10, 0x29);   // 0x40054b <_start+835>:	rol    r10,0x29

        // *** R11 ***
        r11 = r11 ^ 0xf0d6ea65fdb172b6; // 0x40054f <_start+839>:	movabs rax,0xf0d6ea65fdb172b6
                                        // 0x400559 <_start+849>:	xor    r11,rax
        r11 = r11 + r10;                // 0x40055c <_start+852>:	add    r11,r10
        __bswap_64(r11);                // 0x40055f <_start+855>:	bswap  r11
        r11 = rol(r11, 0x2f);   // 0x400562 <_start+858>:	rol    r11,0x2f
        
        rsi = r11;                      // 0x400566 <_start+862>:	mov    rsi,r11
        counter = counter - 1;          // 0x400569 <_start+865>:	loop   0x40050a <_start+770>
    } while(counter != 0);

    buffer32[0] += r8;                  // 0x40056b <_start+867>:	add    QWORD PTR [rdi],r8
    buffer32[1] += r9;                  // 0x40056e <_start+870>:	add    QWORD PTR [rdi+0x8],r9
    buffer32[2] += r10;                 // 0x400572 <_start+874>:	add    QWORD PTR [rdi+0x10],r10
    buffer32[3] += r11;                 // 0x400576 <_start+878>:	add    QWORD PTR [rdi+0x18],r11
        
    return;                             // 0x40057a <_start+882>:	ret
}


// ****************
// verify()
// ****************
// Verify function params refer to following asm instructions:
// 0x400244 <_start+60>:	mov    rdi,r12                  ; file descriptor
// 0x400247 <_start+63>:	movabs rsi,0xdeadfacedeadbeef   ; deadbeef
// 0x400251 <_start+73>:	movabs rdx,0x123456701234567    ; numbers
// 0x40025b <_start+83>:	mov    rcx,0x80                 ; counter
// 0x400262 <_start+90>:	movabs r8,0x601038              ; buffer32
//
// Then the actual function gets called here:
// 0x40026c <_start+100>:	call   0x4003e4 <_start+476>    ; verify()
long verify(ulong file_descriptor, char *buffer32)
{
    //
    ulong deadbeef = 0xdeadfacedeadbeef;
    ulong numbers = 0x123456701234567;
    ulong counter_0x80 = 0x80;
    ulong char1 = 0;
    ulong lVar1;
    ulong local_38;
    ulong local_30;

    local_30 = 0x40;
    local_38 = deadbeef;
    do
    {
        // **********************************
        // read_chunk() function
        // Reads just 1 character at a time
        // **********************************
        ulong result = syscall(SYS_read, file_descriptor, buffer32, 0x1);
        if (result == 1)
        {
            printf("Syscall: {%s},{%ld}\t", buffer32, result);
        }

        // Check if result is -1
        if (result == 0xffffffffffffffff)
        {
            return -1;
        }

        // Scramble
        scramble(buffer32, char1);
        unknown_func(&local_38, char1);
        printf("Scramble: {%s}, {%ld}\n", buffer32, local_38);
        // lVar1 = unknown_func(&local_38, char1);
        // if (lVar1 != 0)
        // {
        //     return lVar1;
        // }
        if (local_38 == numbers)
        {
            return 0;
        }
        counter_0x80 = counter_0x80 + -1;
    } while (counter_0x80 != 0);
    return -1;
}

int main() // _start()
{
    // ************************
    // Syscall to open a file
    // ************************
    char filename[] = "d.d";
    int flags = 0;
    // umode_t mode = 0;        // umode not necessary?
    ulong file_descriptor = syscall(SYS_open, filename, flags);
    printf("File descriptor: %ld\n", file_descriptor);

    // ************************
    // Create a buffer and clear it
    // ************************
    int buffer_size = 0x20; // 0x20 is 32 decimal
    char buffer[buffer_size];

    for (int i = 0; i < 32; i++)
    {
        *(buffer + i) = 0;
    }
    printf("Buffer cleared: {%s}\n", buffer);

    // ************************
    // Verify d.d file contents
    // ************************
    int result = verify(file_descriptor, buffer); // verify
    if (result == 0)
    {
        printf("%s", "! + \n! -\n! --\nReversingHero\n");
        printf("TARGET FUNCTION EXECUTED\n");
    }

    return 0;
}