COMPRESSION DE TEXTE
====================

Réduire la taille d'un fichier quelconque

1. Compression avec pertes

-> JPEG, MP3 : supprimer les informations qui sont le moins perceptibles pour l'humain

On ne s'y intéressera pas ici

2. Compression sans pertes

Il s'agit ici de ranger la MÊME information dans une plus petite boîte
Exemple : zip, rar

Intro: compression de texte
---------------------------

- Approche naïve 1 pour de la compression de texte : aa, ab, ... sur 10 bits -> 30%

Compression de données en général :
---------------------------------

- Format des données : octet
- Analogie : texte

Code de base : ASCII, largeur fixe, même proba pour chaque char

la capacité de compression vient justement du fait que la source n'est pas aléatoire : elle suit une certaine logique et donc que chaque symbole ou groupe de symbole n'a pas la même proba

=> Donc à priori, compresser en boucle ne sert à rien

-> Abbréviations (codage par dict)
    |
   \/
-> code de Huffman (+dynamique) 30 %

Importance du modèle

Transformées ==> simplification du modèle + RLE -> 30 %

Author site: https://sites.google.com/site/yuta256/sais

# Voie d'amélioration

ne plus faire un système stationnaire dans bitwise_ppm_flat
