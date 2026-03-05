#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int main(void)
{
    // ─────────────────────────────────────────────
    // ETAPE 1 : créer un fichier avec du contenu
    // ─────────────────────────────────────────────
    int fd = open("fichier.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, "Hello, mmap !\n", 14);
    write(fd, "Deuxieme ligne.\n", 16);
    write(fd, "Troisieme ligne.\n", 17);
    close(fd);

    // ─────────────────────────────────────────────
    // ETAPE 2 : rouvrir + récupérer la taille
    // ─────────────────────────────────────────────
    fd = open("fichier.txt", O_RDONLY);

    struct stat st;
    fstat(fd, &st);                   // st.st_size = taille en octets
    size_t taille = st.st_size;

    printf("Taille du fichier : %zu octets\n\n", taille);

    // ─────────────────────────────────────────────
    // ETAPE 3 : mmap → le fichier devient un tableau
    // ─────────────────────────────────────────────
    char *map = mmap(
        NULL,        // adresse : le kernel choisit
        taille,      // combien d'octets mapper
        PROT_READ,   // lecture seule
        MAP_PRIVATE, // pas de partage
        fd,          // le fichier
        0            // depuis le début
    );
    close(fd);       // fd plus nécessaire après mmap

    // ─────────────────────────────────────────────
    // ETAPE 4 : utiliser map comme un tableau normal
    // ─────────────────────────────────────────────

    // Lire octet par octet
    printf("=== Lecture octet par octet ===\n");
    for (size_t i = 0; i < taille; i++)
        printf("%c", map[i]);
    printf("\n");

    // Accéder à un caractère précis
    printf("=== Accès direct ===\n");
    printf("map[0]  = '%c'\n", map[0]);    // 'H'
    printf("map[7]  = '%c'\n", map[7]);    // 'm'
    printf("map[13] = '%c'\n", map[13]);   // '\n'
    printf("\n");

    // Chercher une sous-chaine (comme strstr mais sur les octets bruts)
    printf("=== Recherche ===\n");
    // APRES (marche partout)
char *trouve = NULL;
for (size_t i = 0; i + 8 <= taille; i++)
{
    if (memcmp(map + i, "Deuxieme", 8) == 0)
    {
        trouve = map + i;
        break;
    }
}
if (trouve)
    printf("'Deuxieme' trouvé à l'offset %ld\n\n", trouve - map);
    // Afficher les octets en hexa (c'est ce qu'on fait avec un ELF)
    printf("=== Premiers octets en hexadécimal ===\n");
    for (size_t i = 0; i < 14; i++)
        printf("map[%2zu] = 0x%02x = '%c'\n",
            i,
            (unsigned char)map[i],
            map[i] == '\n' ? '?' : map[i]);

    // ─────────────────────────────────────────────
    // ETAPE 5 : libérer
    // ─────────────────────────────────────────────
    munmap(map, taille);  // map est invalide après ça
    printf("\nFait. map libéré avec munmap.\n");

    return 0;
}

/*
    COMPILER ET LANCER :

        cc -o demo demo_mmap.c && ./demo

    CE QUE TU DOIS RETENIR :

        - mmap() → prend un fichier, retourne un pointeur
        - ce pointeur, c'est juste le fichier vu comme un tableau d'octets
        - map[0] = premier octet du fichier
        - map[taille - 1] = dernier octet
        - tu peux caster ce pointeur en n'importe quelle structure
          (c'est exactement ce qu'on fait avec Elf64_Ehdr *ehdr = map)
        - munmap() libère quand t'as fini
*/