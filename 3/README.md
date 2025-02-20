# Level 3

Open **x3** in Ghidra. Study the `_start` function.

```c
void _start(void)
{
  uint result;
  long verified;
  long extraout_RDX;
  long extraout_RDX_00;
  long extraout_RDX_01;
  int __status;
  
  printf(&question_mark);
  file_pointer = fdopen(0,&r);
  result = read_input(file_pointer);
  if (extraout_RDX == 0) {
    user_input1 = result;
    printf(&question_mark);
    result = read_input(file_pointer);
    if (extraout_RDX_00 == 0) {
      user_input2 = result;
      printf(&question_mark);
      result = read_input(file_pointer);
      if (extraout_RDX_01 == 0) {
        user_input3 = result;
        verified = verify_input((ulong)user_input1,(ulong)user_input2,(ulong)result);
        if (verified == 0) {
          printf(&exclamation_plus);
          target_func(&user_input1,0xc);
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

Focus in on the `verify_input` function (source: [verify_input.c](verify_input.c)). Rename variables and refactor to make it more readable:
```c
long verify_input(int a, int b, int c)
{
    long result;

    // Equation 1
    result = (a + c) * 2 + b * 3 - 1;    

    if (result == 0)
    {
        // Equation 2
        result = ((b + a * 2) * 2 + c * 3) - 1;
        if (result == 0)
        {
            // Equation 3
            result = (a * 9 + c * 7 + b * 6) - 1;
        }
    }

    // The return value needs to equal zero as well
    return result;
}
```

There are three equations and three variables in this function. The goal is to return a result of 0. Therefore, all three equations should equal zero. Solve for `a`, `b`, and `c`:
```
Eq1: 0 = (a + c) * 2 + b * 3 - 1
Eq2: 0 = ((b + a * 2) * 2 + c * 3) - 1
Eq3: 0 = (a * 9 + c * 7 + b * 6) - 1
```

Simplify the equations:
```
Eq1: 0 = 2a + 2c + 3b - 1
Eq2: 0 = 2b + 4a + 3c - 1
Eq3: 0 = 9a + 7c + 6b - 1
```

Rearrange the variables:
```
Eq1: 0 = 2a + 3b + 2c - 1
Eq2: 0 = 4a + 2b + 3c - 1
Eq3: 0 = 9a + 6b + 7c - 1
```

Eliminate `b` by solving for 2*(Eq1) - 3*(Eq2):
```
Multiply:
Eq1: (0 =  2a + 3b + 2c - 1) * 2
   => 0 =  4a + 6b + 4c - 2
Eq2: (0 =  4a + 2b + 3c - 1) * 3
   => 0 = 12a + 6b + 9c - 3

Subtract and solve for c:
      0 = -8a + 0b - 5c + 1
      0 = -8a - 5c + 1
     5c = 1 - 8a
      c = (1 - 8a)/5
```

Eliminate `b` by solving for 2*(Eq2) - (Eq3):
```
Multiply:
Eq2: (0 =  4a + 2b + 3c - 1) * 3
   => 0 = 12a + 6b + 9c - 3
Eq3:  0 =  9a + 6b + 7c - 1

Subtract and solve for c:
      0 =  3a + 0b + 2c - 2
      0 =  3a + 2c - 2
      c = (2 - 3a)/2
```

Take the resulting equations for `c` and solve for `a`:
```
c = (1 - 8a)/5
c = (2 - 3a)/2
=> (1 - 8a)/5 = (2 - 3a)/2
=> 2*(1 - 8a) = 5*(2 - 3a)
=> 2 - 16a = 10 - 15a
=> 2 - 10  = 16a - 15a
=>       a = -8
```

Plug-in `a` to solve for `c`:
```
c = (1 - 8a)/5
  = (1 - 8(-8)) / 5
  = (1 + 64) / 5
  = 65 / 5
  = 13
```

With `a` and `c`, solve for `b`:
```
Eq1: 0 = 2a + 3b + 2c - 1
     0 = 2(-8) + 3b + 2(13) - 1
     0 = -16 + 3b + 26 - 1
     0 = 3b + 9
   -3b = 9
     b = -3
```

Final values are:
```
a = -8
b = -3
c = 13
```

## `read_input()`

Looking at the `read_input` function (source: [read_input.c](read_input.c)), it hints that it is expecting hex input.

For example the function will exit if the counter is greater than 16, therefore limiting user input to 16 hex characters, which is 64 bits, which is the size of a long variable:
```c
counter = counter + 1;
if (counter > 0x10) // 0x10 = 16 decimal
{
    goto LAB_00400722; // exit
}
```

The function also checks if the character is less than "9" ASCII:
```c
temp = char_input - 0x30;  // 0x30 = "0" ASCII
// By subtracting "0" ASCII, temp is converted from ASCII to decimal
// For example, "9" ASCII (0x39) - 0x30 = 9 decimal
// Then it checks if the value is greater than 9
if (temp > 9)
{
    ...
}
```

If the `temp` value above is greater than 9, then the function checks if the value is at least less than the value "f" in ASCII:
```c
// 0x61 is "a" in ASCII
// 0x61 + 5 is "f" in ASCII
// if the char is greater than "f" then it exits
if (char_input - 0x61 > 5)
{
    goto LAB_00400722; // exit
}
```

## Convert from Decimal to Hex

Convert `a`, `b`, and `c` to their 64 bit hex equivalents:
```
a = -8 = 0xFFFFFFFFFFFFFFF8
b = -3 = 0xFFFFFFFFFFFFFFFD
c = 13 = 0xD
```

Plug these values in and grab the flag:
```
$ ./x3
? FFFFFFFFFFFFFFF8
? FFFFFFFFFFFFFFFD
? D
! + 8DE14EA92C145855AE74426319C1ABD74DFAC053027ED65E1136788E6076447F
```

Verify that the flag is correct:
```
$ ./p3 
> 8DE14EA92C145855AE74426319C1ABD74DFAC053027ED65E1136788E6076447F
@ 4/p4
@ 4/x4
] +
```