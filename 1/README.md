# Level 1

Notes: 
In the **x1** program, a string compare (`strcmp`) is comparing user input to an emoji string:
```
d(-_-)b//d(+_+)b\\d(-_-)b
```

If the user input and the string match, the program runs a special function that reveals the password:

```
$ ./x1
? d(-_-)b//d(+_+)b\\d(-_-)b
! + 3E437BBA43971D612049DE8AD54FDEF068931E8C6D26F63D83742F932E740B6D
```

Now try this password with the **p1** program:
```
$ ./p1
> 3E437BBA43971D612049DE8AD54FDEF068931E8C6D26F63D83742F932E740B6D
@ 2/p2
@ 2/x2
] +
```

The password was accepted and the level 2 folder and files have been created.