#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define offset 95
#define LEN 26

typedef struct tries
{
    struct tries *letters[LEN];
    short isEnd;
}Node;
// Initializam un nod nou din TRIES
Node* getNode()
{

    Node *nod = (Node*)malloc(sizeof(Node));
    if(nod == NULL)
    {
        fprintf(stderr, "Eroare la alocarea de memorie");
        exit(1);
    }
    int i;
    for(i = 0; i < LEN; i++)
        nod->letters[i] = NULL;

    nod->isEnd = 0;
    return nod;
}
// Pentru fiecare litera din cuvantul dat, daca nu avem pointer-ul
// pentru litera respectiva il initializam

void insert( Node *root, char *str)
{
    int len = strlen(str);
    int i;
    Node *move = root;
        for(i = 0; i < len; i++)
    {
        if(move->letters[str[i] - offset] == NULL)
            move->letters[str[i] - offset] = getNode();

        move = move->letters[str[i] - offset];
    }
    move->isEnd = 1;
}

// Returneaza un pointer catre ultimul caracter al cuvantului cautat
// Modul 1 este folosit pentru a returna un pointer nenul si daca cuvantul nu exista
// adica daca exista acel prefix in TRIES

Node* search( Node *root , char *str , int mode)
{
    if(root == NULL) return NULL;
    int len = strlen(str);
    Node *move = root;
    int i = -1;
    for(i = 0; i < len; i++)
    {

        move = move->letters[str[i] - offset];
        if(move == NULL) return NULL;

    }
    if(mode && move != NULL) return move;
    if( move != NULL  && move->isEnd == 1)
        return move;
    return NULL;
}

// afisam toate cuvintele din TRIES

void printContent(Node *root, char *str, int level)
{
    if(root == NULL) return;
    int i;
    if(root->isEnd && level )
    {
        str[level] = '\0';
        printf("\t%s \n", str);
    }
    for(i = 0; i < LEN; i++)
    {
        if(root->letters[i] != NULL )
        {
            str[level] = offset + i;
            printContent(root->letters[i], str, level + 1);
        }
    }
}

// afissam sufixele unei chei date
// cautam cheia in TRIES si daca este
// gasit afisasm toate sufixele sale

void printSufixes(Node *root, char *sufixOfKey)
{
    if(root == NULL) return;
    char *str = malloc(sizeof(char) * 40);
    Node *sufixOf = search(root, sufixOfKey, 1);
    if(sufixOf == NULL) return;
    //int i;
    printf("%s : \n", sufixOfKey);
    printContent(sufixOf, str ,0);
    free(str);
}
// returneaza numarul de legaturi ale nodului primit ca parametru

int getLinks(Node *nod)
{
    int i, count = 0;
    for(i = 0; i < LEN; i++)
        if(nod->letters[i] != NULL) count++;
    return count;
}
// gasim adresa ultimei litere ce apare in cel putin doua cuvinte
// daca cheia apare doar intr-un cuvant atunci returnam adresa primei litere
Node* getIntersection(Node *root, char *key)
{
    Node *move = root;
    Node *atIntersection = root;
    Node *stopBleeding = root;
    int links = 0, i = 0;
    char c = key[0];
    while(move != NULL)
    {
        links = getLinks(move);
        if(links > 1)
        {
            atIntersection = move->letters[ key[i] - offset ];
            stopBleeding = move;
            c = key[i];
        }
            move = move->letters[ key[i] - offset];
        i++;
    }
    //Anulam legatura cuvantului cu partea sa libera
    //( partea care apartine doar cuvantului pe care
    // vrem sa-l stergem )
    if(stopBleeding != root) stopBleeding->letters[c - offset] = NULL;
    return atIntersection;
}
// Sterge toate literele de la starting point spre frunze
// deleteALLPaths(&root) - sterge intreg TRIES
void deleteALLPaths(Node **startingPoint)
{
    Node *copy = *startingPoint;
    int i;
    for(i = 0; i < LEN; i++)
    {
        if( (*startingPoint)->letters[i] != NULL)
        {
            deleteALLPaths(&(*startingPoint)->letters[i]);
            free(copy);
        }

    }
    *startingPoint = NULL;
}

// cautam nodul ce contine cheia pe care dorim sa o stergem
// gasim adressa ultimei litere ce se afla la intersectia dintre
// doua cuvinte
void deleteKey(Node **root, char *key)
{
    if(search(*root ,key , 0) != NULL)
    {
        Node *atIntersecion = getIntersection(*root, key);
            deleteALLPaths(&atIntersecion);
    }
    else return;

}
int main()
{
    char *str = malloc(sizeof(char) * 100);
    Node *root = getNode();
    insert(root, "cool");
    insert(root, "cola");
    insert(root, "maria");
    insert(root, "ioana");
    insert(root, "sonia");
    insert(root, "laur");
    insert(root, "mariana");
    insert(root, "martie");
    //printContent(root, str, 0);
   // deleteALLPaths(&root);
    //deleteKey(&root, "cool");
    printContent(root, str, 0);
    printSufixes(root, "mar");
    return 0;
}
