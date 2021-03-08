#include <stdio.h>
#include <assert.h>
#include "helper.h"

typedef unsigned long ulong;

// target_0xff()
//  params:
//      char *all_ones_buffer       ; rdi
//      max_0x1a                    ; rsi = 0x1a
//      user_input                  ; rdx, also on stack from r12
// function starts at 0x4006ae
char *target_0xff(char *all_ones_buffer, ulong max_0x1a, char user_input)
{
    // Test input is: "abcdefg"
    // 0x4006ae <_start+510>:	push   r12                      ; r12 is user_input 'a'
    // 0x4006b0 <_start+512>:	xor    r12,r12                  ; set r12 to 0 (counter)
    // 0x4006b3 <_start+515>:	mov    al,BYTE PTR [rdi+r12*1]  ; *rdi = 0xff 0xff..
    // 0x4006b7 <_start+519>:	cmp    al,0xff                  ; al(rax) == 0xff
    // 0x4006b9 <_start+521>:	je     0x4006d2 <_start+546>    ; jump to end if al == 0xff
    // 0x4006bb <_start+523>:	cmp    dl,al                    ; else: compare dl(0x61 = 'a') and al
    // 0x4006bd <_start+525>:	je     0x4006d2 <_start+546>    ; jump to end if dl == al
    // 0x4006bf <_start+527>:	lea    r12,[r12+r12*1+0x1]      ; else update counter: 2x + 1
    // 0x4006c4 <_start+532>:	jb     0x4006c9 <_start+537>    ;
    // 0x4006c6 <_start+534>:	inc    r12                      ;
    // 0x4006c9 <_start+537>:	cmp    r12,rsi                  ;
    // 0x4006cc <_start+540>:	jb     0x4006b3 <_start+515>    ;
    // 0x4006ce <_start+542>:	xor    eax,eax                  ;
    // 0x4006d0 <_start+544>:	jmp    0x4006d6 <_start+550>    ;
    // 0x4006d2 <_start+546>:	lea    rax,[rdi+r12*1]          ; load rax w/ rdi + offset
    // 0x4006d6 <_start+550>:	pop    r12                      ; set r12 back to user_input 'a'
    // 0x4006d8 <_start+552>:	ret

    unsigned char byte;
    ulong counter;

    counter = 0;
    do
    {
        byte = *(all_ones_buffer + counter);    // a1b: {0x61, 0xff, ...}
                                                // byte = 0x61
        // debug("bVar1: 0x%02x\n", (unsigned char)bVar1);
        // debug("bVar1 == 0xff: %d\n", ((unsigned char)bVar1 == (unsigned char)0xff));
        debug("bVar1: 0x%02x\n", byte);
        if ((byte == 0xff) || (user_input == byte))
        {
            // Target condition
            return all_ones_buffer + counter;
        }
        counter = counter * 2 + 1;
        if (byte <= user_input)         // 0x4006c4 <_start+532>:	jb     0x4006c9 <_start+537> 
        {
            counter += 1;
        }
    } while (counter < max_0x1a);
    // Fail condition
    return 0;
}

long target_return_zero(char *all_ones_buffer, ulong counter, char *user_input)
{
    long return_value;
    while (1)
    {
        if (*user_input == 0)
        {
            return 0;
        }            
        
        // *********************
        // target_return_zero2
        // *********************

        // (gdb) x/20i 0x40068c
        // 0x40068c <_start+476>:	push   r12
        // 0x40068e <_start+478>:	mov    r12,rdx
        // 0x400691 <_start+481>:	call   0x4006ae <_start+510>
        // 0x400696 <_start+486>:	test   rax,rax
        // 0x400699 <_start+489>:	je     0x4006a7 <_start+503>
        // 0x40069b <_start+491>:	cmp    BYTE PTR [rax],0xff
        // 0x40069e <_start+494>:	jne    0x4006a7 <_start+503>
        // 0x4006a0 <_start+496>:	mov    BYTE PTR [rax],r12b
        // 0x4006a3 <_start+499>:	xor    eax,eax
        // 0x4006a5 <_start+501>:	jmp    0x4006ab <_start+507>
        // 0x4006a7 <_start+503>:	or     rax,0xffffffffffffffff
        // 0x4006ab <_start+507>:	pop    r12
        // 0x4006ad <_start+509>:	ret

        char user_input_char = (char) *user_input;
        char *result = target_0xff(all_ones_buffer, counter, user_input_char);
        // if result is zero OR (*result) is not 0xff: fail (-1)
        if ((result == (char *)0x0) || ((unsigned char)*result != 0xff))
        {
            debug("target_return_zero2: fail condition: {0x%02x}\n", result);
            if (result != 0x0) {
                debug("result points to: {0x%02x}\n", (unsigned char)*result);
            }
            return_value = -1;
        }
        else
        {
            // Goal to get here
            debug("target_return_zero2: goal condition\n");
            *result = user_input_char;
            return_value = 0;
        }
        // *********************

        if (return_value != 0)
            break;
        user_input = user_input + 1;
    }
    return -1;
}

long verify_only_alphabetical(char *user_input)
{
    char my_char;
    do
    {
        my_char = *user_input;
        // debug("verify, my_char{%c}\n", my_char);
        if (my_char == '\0')
        {
            return 0;
        }
        user_input = user_input + 1;
        // debug("verify, my_char-0x61:{0x%02x}\n", (unsigned char)(my_char - 0x61));
    } while ((unsigned char)(my_char - 0x61) <= (unsigned char)0x19);
    return -1;
}

void set_buffer_to_all_ones(char *buffer, long counter_0x1a)
{
    while (counter_0x1a != 0)
    {
        counter_0x1a = counter_0x1a + -1;
        *buffer = 0xff;
        buffer = buffer + 1;
    }
    return;
}

ulong verify_buffer_does_not_contain_0xff(char *buffer, long counter_0x1a)
{
    // 0x40063a <_start+394>:	mov    rcx,rsi
    // 0x40063d <_start+397>:	xor    edx,edx
    // 0x40063f <_start+399>:	xor    eax,eax
    // 0x400641 <_start+401>:	cmp    BYTE PTR [rdi],0xff
    // 0x400644 <_start+404>:	sete   dl
    // 0x400647 <_start+407>:	or     rax,rdx
    // 0x40064a <_start+410>:	inc    rdi
    // 0x40064d <_start+413>:	loop   0x400641 <_start+401>
    // 0x40064f <_start+415>:	ret
    ulong uVar1;

    uVar1 = 0;
    do
    {
        uVar1 = uVar1 | (*buffer == -1);
        buffer = buffer + 1;
        counter_0x1a = counter_0x1a + -1;
    } while (counter_0x1a != 0);
    return uVar1;
}

int main()
{
    // ********************************
    // Test char type casting values
    // ********************************
    assert((unsigned char)0xff > (unsigned char)0xfe);
    assert((unsigned char)0xff == (unsigned char)0xff);
    assert((char)0xff < (unsigned char)0xff);

    // ********************************
    // Test verify_only_alphabetical()
    // ********************************
    char buffer[] = "abcdefghijklmnopqrstuvwxyz";
    long result = verify_only_alphabetical(buffer);
    assert(result == 0);

    char b2[] = "_hello";
    result = verify_only_alphabetical(b2);
    assert(result == -1);

    // ********************************
    // Test set_buffer_to_all_ones()
    // ********************************
    char buffer3[0x1a];
    printf("buffer3: ");
    print_char_array(buffer3, 0x1a);
    set_buffer_to_all_ones(buffer3, 0x1a);
    printf("buffer3: ");
    print_char_array(buffer3, 0x1a);

    
    
    // ********************************
    // Test compare_char_array()
    // ********************************    
    char a[] = {0xff, 0xfe};
    char b[] = {0xff, 0xfe};
    int compare_result = compare_char_array(a, b, 2);
    assert(compare_result == 0);

    char c[] = {0xff, 0xfe, 0xaa};
    char d[] = {0xff, 0xfe, 0xab};
    compare_result = compare_char_array(c, d, 3);
    assert(compare_result == -1);
    
    // ********************************
    // Test target_return_zero()
    // ********************************
    char *input1 = "aaaaaaaaaaaaaaa";
    result = target_return_zero(buffer3, 0x1a, input1);
    printf("all ones buffer: ");
    print_char_array(buffer3, 0x1a);    
    char expected[] = {0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    compare_result = compare_char_array(buffer3, expected, 0x1a);
    assert(compare_result == 0);

    set_buffer_to_all_ones(buffer3, 0x1a);  // reset buffer3
    char *input2 = "zyxwvut";
    result = target_return_zero(buffer3, 0x1a, input2);
    printf("all ones buffer: ");
    print_char_array(buffer3, 0x1a); 
    char expected2[] = {0x7a, 0x79, 0xff, 0x78, 0xff, 0xff, 0xff, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x76, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    compare_result = compare_char_array(buffer3, expected2, 0x1a);
    assert(compare_result == 0);

    set_buffer_to_all_ones(buffer3, 0x1a);  // reset buffer3
    char *input3 = "abcdefghijklmno";
    result = target_return_zero(buffer3, 0x1a, input3);
    printf("all ones buffer: ");
    print_char_array(buffer3, 0x1a); 
    char expected3[] = {0x61, 0xff, 0x62, 0xff, 0xff, 0xff, 0x63, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x64, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    compare_result = compare_char_array(buffer3, expected3, 0x1a);
    assert(compare_result == 0);

    return 0;
}