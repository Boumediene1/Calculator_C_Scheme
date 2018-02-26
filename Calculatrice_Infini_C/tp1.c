/*
    IFT 2035  TP1 - Gestion mémoire et pointeur.

    Boumediene Boukharouba
    20032279

    Alexandre Deneault
    20044305
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define empiler(n) \
do {                                                                                        \
    Noeud *temp = malloc(sizeof(Noeud));                                                    \
    if (temp == NULL) {                                                                     \
        msgerr = "Il n'y a pas assez d'espace en memoire pour enregistrer les nombres.";    \
        libererNombre(n);                                                                   \
        goto erreur;                                                                        \
    }                                                                                       \
    (*temp).elem.nb = n;                                                                    \
    (*temp).suivant = pile;                                                                 \
    pile = temp;                                                                            \
} while (0)

#define depiler(n)  \
do {                            \
    n = (*pile).elem.nb;        \
    Noeud *temp = pile;         \
    pile = (*temp).suivant;     \
    free(temp);                 \
} while (0)


typedef struct Noeud Noeud;
typedef struct Nombre Nombre;
Nombre* addition(Nombre *n1, Nombre *n2);
Nombre* soustraction(Nombre *n1, Nombre *n2);
Nombre* mult(Nombre *n1, Nombre *n2);

struct Noeud {
    union {
        char valeur;
        Nombre *nb;
    } elem;
    Noeud *suivant;
};

struct Nombre {
    Noeud *chiffres;
    int reference;
    char signe;
};

char c;
char *msgerr = "";
Noeud *pile = NULL;
Nombre *variable[26];
Nombre *varTemp[26];
Noeud *varALiberer;

/*
 *   Libere la memoire occupee par le Nombre pointe par *p.
 */
void libererNombre(Nombre *p) {
    if (p == NULL) {
        printf("Un pointeur NULL ne peut pas être liberer.");
        return;
    }
    // Si le pointeur est plus petit que 1, libere le nombre
    if ((*p).reference < 1) {
        // Libere le Nombre
        Noeud *temp = (*p).chiffres;
        free(p);
        // Libere la liste chainee
        while (temp != NULL) {
            Noeud *temp2 = temp;
            temp = (*temp).suivant;
            free(temp2);
        }
    }
}

/*
 *  Converti un Nombre en chaine de caractere
    Retourne NULL s'il manque de place en mémoire
 */
char* nombreAChaine(Nombre *p) {
    // Détermine le nombre de caractère dans le tableau
    Noeud *temp = (*p).chiffres;
    int cmpt = 1;           // Pour le \0
    if (temp == NULL) {     // Pour le 0
        cmpt++;
    }
    if ((*p).signe != 0) {   //Pour le -
        cmpt++;
    }
    while (temp != NULL) {      //Cherche la longueur du nombre
        cmpt++;
        temp = (*temp).suivant;
    }
    // Demande le tableau
    char *chaine = malloc(sizeof(char)*cmpt);
    //Si il n'y a pas assez de place, retourne le pointeur NULL
    if (chaine == NULL) {
        msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
        return chaine;
    }
    // Copie les caractères dans le tableau
    char *tempch = chaine;
    temp = (*p).chiffres;
    // Si le nombre est négatif
    if ((*p).signe != 0) {
        tempch[0] = '-';
        tempch++;
    }
    // Si le nombre vaut 0
    if (temp == NULL) {
        tempch[0] = '0';
        tempch++;
    }
    // Rempli le tableau
    while (temp != NULL) {
        tempch[0] = (*temp).elem.valeur;
        tempch++;
        temp = (*temp).suivant;
    }
    tempch[0] = '\0';

    //Inverse les chiffres dans le tableau
    tempch--;
    char *tempch2 = chaine;
    if ((*p).signe != 0) {
        tempch2++;
    }
    while (tempch2 < tempch) {
        char t = tempch2[0];
        tempch2[0] = tempch[0];
        tempch[0] = t;
        tempch2++;
        tempch--;
    }
    return chaine;
}





void vider() {
//Liberer la pile
    while (pile != NULL) {
        Noeud *temp = pile;
        pile = (*pile).suivant;
        Nombre *nt = (*temp).elem.nb; //Nombre
        free(temp);
        libererNombre(nt);
    }
    //Vide le buffer
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

/*
    Retourne 0 si les nombres sont égaux
    Retourne 1 si le premier est plus grand
    Retourne -1 si le premier est plus petit
*/
int plusGrandABS(Nombre *n1, Nombre *n2) {
    int res = 0;
    Noeud *c1, *c2;
    c1 = (*n1).chiffres;
    c2 = (*n2).chiffres;
    // Passe à travers les deux nombre caractere par caractere en se souvenant de celui qui était le plus petit
    while (c1 != NULL && c2 != NULL) {
        if ((*c1).elem.valeur > (*c2).elem.valeur) {
            res = 1;
        } else if ((*c1).elem.valeur < (*c2).elem.valeur) {
            res = -1;
        }
        c1 = (*c1).suivant;
        c2 = (*c2).suivant;
    }
    if (c1 != NULL) {
        return 1;
    }
    if (c2 != NULL) {
        return -1;
    }
    return res;
}

/*
    Retourne un Nombre contenant le resultat de l'addition
    Retourne NULL si l'opération n'a pas pu etre effectuee
*/
Nombre* addition(Nombre *n1, Nombre *n2) {
    Nombre *res;
    // Si les deux nombres ont le même signe
    if (((*n1).signe == 0 && (*n2).signe == 0) || ((*n1).signe != 0 && (*n2).signe != 0)) {
        // Demande un espace pour le Nombre
        res = malloc(sizeof(Nombre));
        if (res == NULL) {
            msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
            return NULL;
        }
        //Determine le signe du resultat
        if ((*n1).signe != 0) {
            (*res).signe = 1;
        } else {
            (*res).signe = 0;
        }
        (*res).reference = 0;
        (*res).chiffres = NULL;	//La liste est vide pour l'instant

        Noeud *temp1 = (*n1).chiffres;
        Noeud *temp2 = (*n2).chiffres;
        char retenue = 0;
        Noeud *nouvTemp = NULL;
        //Effectue l'addition caractere par caractere
        while (temp1 != NULL || temp2 != NULL || retenue != 0) {
            char c1;
            char c2;
            // Extrait les caracteres
            if (temp1 == NULL) {
                c1 = 0;
            } else {
                c1 = (*temp1).elem.valeur - '0';
            }
            if (temp2 == NULL) {
                c2 = 0;
            } else {
                c2 = (*temp2).elem.valeur - '0';
            }
            // Demande l'espace pour le nouveau Noeud
            Noeud *nouv = malloc(sizeof(Noeud));
            if (nouv == NULL) {
                msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
                libererNombre(res);
                return NULL;
            }
            // Effectue l'addition
            char cRes = c1 + c2 + retenue;
            (*nouv).elem.valeur = (cRes%10) + '0';
            retenue = cRes / 10;
            // Ajoute le Noeud a la liste
            (*nouv).suivant = NULL;
            if (nouvTemp == NULL) {
                (*res).chiffres = nouv;
            } else {
                (*nouvTemp).suivant = nouv;
            }
            nouvTemp = nouv;
            // Va au caractere suivant
            if (temp1 != NULL) {
                temp1 = (*temp1).suivant;
            }
            if (temp2 != NULL) {
                temp2 = (*temp2).suivant;
            }
        }
    // Si un des deux est negatif
    } else {
        // Si n1 est positif
        if ((*n2).signe != 0) {
            (*n2).signe = 0;
            res = soustraction(n1, n2);
            (*n2).signe = 1;
        // Si n2 est positif
        } else {
            (*n1).signe = 0;
            res = soustraction(n2, n1);
            (*n1).signe = 1;
        }
    }
    return res;
}

/*
    Retourne un Nombre contenant le resultat de la soustraction
    Retourne NULL si l'opération n'a pas pu etre effectuee
*/
Nombre* soustraction(Nombre *n1, Nombre *n2) {
    Nombre *res;
    // Si les deux nombres ont le même signe
    if (((*n1).signe == 0 && (*n2).signe == 0) || ((*n1).signe != 0 && (*n2).signe != 0)) {
        // Demande un espace pour le Nombre
        res = malloc(sizeof(Nombre));
        if (res == NULL) {
            msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
            return NULL;
        }
        (*res).reference = 0;
        (*res).chiffres = NULL;
        int plusGrand = plusGrandABS(n1, n2);
        // Si n1 == n2
        if (plusGrand == 0) {
            (*res).signe = 0;
            return res;
        }
        // Si n1 > n2, le signe ne change pas
        if (plusGrand == 1) {
            (*res).signe = (*n1).signe;
        // Sinon on inverse n1 et n2 en changeant le signe
        } else {
            Nombre *t = n1;
            n1 = n2;
            n2 = t;
            if ((*n1).signe == 0) {
                (*res).signe = 1;
            } else {
                (*res).signe = 0;
            }
        }
        // Maintenant, on a le signe du resultat avec n1 > n2
        Noeud *temp1 = (*n1).chiffres;
        Noeud *temp2 = (*n2).chiffres;
        char diff = 0;
        Noeud *nouvTemp = NULL;
        Noeud *pasZero = NULL;
        // Effectue la soustraction caractere par caractere
        while (temp1 != NULL) {
            // Extrait les valeurs
            char c1;
            char c2;
            c1 = (*temp1).elem.valeur - '0';
            if (temp2 == NULL) {
                c2 = 0;
            } else {
                c2 = (*temp2).elem.valeur - '0';
            }
            // Demande l'espace pour un nouveau noeud
            Noeud *nouv = malloc(sizeof(Noeud));
            if (nouv == NULL) {
                msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
                libererNombre(res);
                return NULL;
            }
            // Calcule la nouvelle valeur
            char cRes = c1 - c2 - diff;
            diff = 0;
            // Touvours possible puisque n1 > n2
            if (cRes < 0) {
                cRes += 10;
                diff = 1;
            }
            (*nouv).elem.valeur = cRes + '0';
            (*nouv).suivant = NULL;
            // Se souvient du plus grand noeud qui n'est pas 0
            if (cRes != 0) {
                pasZero = nouv;
            }
            // nouvTemp est un pointeur sur le chiffre de poid le plus eleve
            // Ajoute le nouveau Noeud dans la liste
            if (nouvTemp == NULL) {
                (*res).chiffres = nouv;
            } else {
                (*nouvTemp).suivant = nouv;
            }
            nouvTemp = nouv;
            // Passe au caractere suivant
            temp1 = (*temp1).suivant;
            if (temp2 != NULL) {
                temp2 = (*temp2).suivant;
            }
        }
        //Enleve les 0 des chiffres significatifs
        Noeud *zero = (*pasZero).suivant;
        (*pasZero).suivant = NULL;
        while (zero != NULL) {
            Noeud *temp = zero;
            zero = (*zero).suivant;
            free(temp);
        }

    // Si un des deux est negatif
    } else {
        if ((*n2).signe == 0) {
            (*n2).signe = 1;
            res = addition(n1, n2);
            (*n2).signe = 0;
        } else {
            (*n2).signe = 0;
            res = addition(n1, n2);
            (*n2).signe = 1;
        }
    }
    return res;
}

/*
    Retourne un Nombre contenant le resultat de la multiplication
    Retourne NULL si l'opération n'a pas pu etre effectuee
*/
Nombre* mult(Nombre *n1, Nombre *n2) {
    //Initialise un Nombre a 0
    Nombre *res = malloc(sizeof(Nombre));
    if (res == NULL) {
        msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
        return NULL;
    }
    (*res).reference = 0;
    (*res).chiffres = NULL;
    // Le signe est positif le temps des calculs
    (*res).signe = 0;
    // Si le premier nombre est plus petit, on les inverse
    if (plusGrandABS(n1, n2) == -1) {
        Nombre *t = n1;
        n1 = n2;
        n2 = t;
    }
    //On ignore le signe de n1 et n2 le temps des calculs
    char signeN1 = (*n1).signe;
    char signeN2 = (*n2).signe;
    (*n1).signe = 0;
    // n1 > n2
    Noeud *temp1 = (*n1).chiffres;
    Noeud *temp2 = (*n2).chiffres;
    Noeud *zeroFin = NULL;
    Nombre *t = NULL;
    // Pour tous les caracteres de n2
    while (temp2 != NULL) {
        // Extrait la valeur de du caractere
        char c2 = (*temp2).elem.valeur - '0';
        // Additionne c2 fois n1 a res
        while (c2 > 0) {
            t = addition(res, n1);
            libererNombre(res);
            res = t;
            if (res == NULL) {
                goto fin;
            }
            c2--;
        }
        // Multiplie n1 par 10
        Noeud *nouv = malloc(sizeof(Noeud));
        if (nouv == NULL) {
            msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
            libererNombre(res);
            res = NULL;
            goto fin;
        }
        // Se souvient du premier zero ajoute
        if (zeroFin == NULL) {
            zeroFin = nouv;
        }
        (*nouv).elem.valeur = '0';
        (*nouv).suivant = (*n1).chiffres;
        (*n1).chiffres = nouv;

        temp2 = (*temp2).suivant;
    }
    //Si les deux nombre ont le meme signe, le resultat est positif, sinon le resultat est negatif
    if ((signeN1 == 0 && signeN2 == 0) || (signeN1 != 0 && signeN2 != 0)) {
        (*res).signe = 0;
    } else {
        (*res).signe = 1;
    }

fin:
    // On retabli le signe de n1 et n2
    (*n1).signe = signeN1;
    (*n2).signe = signeN2;
    // Supprime les zeros qui ont ete ajoutes
    if (zeroFin != NULL) {
        Noeud *temp = (*n1).chiffres;
        (*n1).chiffres = (*zeroFin).suivant;
        (*zeroFin).suivant = NULL;
        while (temp != NULL) {
            Noeud *t = temp;
            temp = (*temp).suivant;
            free(t);
        }
    }
    return res;
}


/*
    Retourne un Nombre representant le compteur de reference du nombre passe en parametre
    Retourne NULL si l'opération n'a pas pu etre effectuee
*/
Nombre* cmptref(Nombre *n1) {
    // Demande un espace pour le Nombre
    Nombre *res = malloc(sizeof(Nombre));
    if (res == NULL) {
        msgerr = "Il n'y a pas assez d'espace en memoire pour pour faire les operations.";
        return NULL;
    }
    (*res).signe = 0;
    (*res).reference = 0;
    (*res).chiffres = NULL;

    int cmpt = (*n1).reference;
    Noeud *nouvTemp = NULL;
    // Ajoute le compteur de reference au Nombre caractere par caractere
    while (cmpt > 0) {
        // Demande l'espace pour le nouveau Noeud
        Noeud *nouv = malloc(sizeof(Noeud));
        if (nouv == NULL) {
            msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
            libererNombre(res);
            return NULL;
        }
        //Determine la valeur du caractere
        (*nouv).elem.valeur = (cmpt%10) + '0';
        cmpt /= 10;
        // Ajoute le Noeud a la liste
        (*nouv).suivant = NULL;
        if (nouvTemp == NULL) {
            (*res).chiffres = nouv;
        } else {
            (*nouvTemp).suivant = nouv;
        }
        nouvTemp = nouv;
    }
    return res;
}





int main(int argc, char **argv) {
    // Initialise le tableau de variable
    int i;
    for (i = 0; i < 26; i++) {
        variable[i] = NULL;
        varTemp[i] = NULL;
    }
    varALiberer = NULL;
    // Débute la lecture des lignes
    do {
        printf("> ");

        c = getchar();
        if (c == EOF) {
            continue;
        }


boucle:
        while (c != '\n' && c != '\0') {

            //On ignore les espaces
            while (c == ' ') {
                c = getchar();
                goto boucle;
            }



            //On lit un nombre
            if (c >= '0' && c <= '9') {
                //Demande un espace mémoire pour la tete et vérifie que l'espace a bien été attribué
                Nombre *tete = malloc(sizeof(Nombre));
                if (tete == NULL) {
                    msgerr = "Il n'y a pas assez d'espace en memoire pour enregistrer les nombres.";
                    goto erreur;
                }
                (*tete).signe = 0;		//Les nombres lus sont toujour positif
                (*tete).reference = 0;
                (*tete).chiffres = NULL;	//La liste est vide pour l'instant

                empiler(tete);

                //Enlève les zéro des chiffre de poids plus élevés
                while (c == '0') {
                    c = getchar();
                };
                // Ajoute les caractere suivant jusqu'a l'espace ou la fin de ligne
                Noeud *temp = NULL;
                while (c != ' ' && c != '\n') {
                    // Demande l'espace pour le Noeud et l'ajoute au debut de la liste
                    temp = malloc(sizeof(Noeud));
                    if (temp == NULL) {
                        msgerr = "Il n'y a pas assez d'espace en memoire pour enregistrer les nombres.";
                        goto erreur;
                    }
                    (*temp).elem.valeur = c;
                    (*temp).suivant = (*(*pile).elem.nb).chiffres; //Tete.chiffres
                    (*(*pile).elem.nb).chiffres = temp;

                    //Charactere suivant
                    c = getchar();
                    // Si c n'est pas un nombre ou un espace, il y a une erreur
                    if ( ! ((c >= '0' && c <= '9') || (c == ' ' || c == '\n')) ){
                        msgerr = "Il y a un caractere invalide";
                        goto erreur;
                    }
                }
                // Va au prochain mot
                goto boucle;
            }



            //On lit un opérateur
            if (c == '+' || c == '-' || c == '*' || c == '?') {
                char op = c;
                c = getchar();
                if (c != ' ' && c != '\n') {
                    msgerr = "Operateur invalide.";
                    goto erreur;
                }
                switch (op) {
                    case '+' :  {
                                //Sors deux nombres de la pile
                                Nombre *n1, *n2, *n3;
                                if (pile != NULL) {
                                    depiler(n2);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    goto erreur;
                                }
                                if (pile != NULL) {
                                    depiler(n1);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    libererNombre(n2);
                                    goto erreur;
                                }
                                //Effectue l'addition
                                n3 = addition(n1, n2);
                                libererNombre(n1);
                                libererNombre(n2);
                                if (n3 == NULL) {
                                    goto erreur;
                                }
                                // Empile le resultat
                                empiler(n3);
                                } break;


                    case '-' :  {
                                //Sors deux nombres de la pile
                                Nombre *n1, *n2, *n3;
                                if (pile != NULL) {
                                    depiler(n2);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    goto erreur;
                                }
                                if (pile != NULL) {
                                    depiler(n1);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    libererNombre(n2);
                                    goto erreur;
                                }
                                // Effectue la soustraction
                                n3 = soustraction(n1, n2);
                                libererNombre(n1);
                                libererNombre(n2);
                                if (n3 == NULL) {
                                    goto erreur;
                                }
                                // Empile le resultat
                                empiler(n3);
                                } break;


                    case '*' :  {
                                //Sors deux nombres de la pile
                                Nombre *n1, *n2, *n3;
                                if (pile != NULL) {
                                    depiler(n2);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    goto erreur;
                                }
                                if (pile != NULL) {
                                    depiler(n1);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    libererNombre(n2);
                                    goto erreur;
                                }
                                // Effectue la multiplication
                                n3 = mult(n1, n2);
                                libererNombre(n1);
                                libererNombre(n2);
                                if (n3 == NULL) {
                                    goto erreur;
                                }
                                // Empile le resultat
                                empiler(n3);
                                } break;


                    case '?' :  {
                                //Sors un nombre de la pile
                                Nombre *n1, *n2;
                                if (pile != NULL) {
                                    depiler(n1);
                                } else {
                                    msgerr = "Il manque d'operande.";
                                    goto erreur;
                                }
                                // Converti le compteur de reference
                                n2 = cmptref(n1);
                                libererNombre(n1);
                                if (n2 == NULL) {
                                    goto erreur;
                                }
                                // Empile le resultat
                                empiler(n2);
                                }
                }

                goto boucle;
            }



            //On lit une variable
            if (c >= 'a' && c <= 'z') {
                // Trouve la position de la variable dans le tableau
                char pos = c - 'a';
                // Verifie le nom de la variable
                c = getchar();
                if (c != ' ' && c != '\n') {
                    msgerr = "Nom de variable incorrect.";
                    goto erreur;
                }
                // Extraction du pointeur de la variable
                Nombre *p;
                if (varTemp[pos] == NULL) {
                    if (variable[pos] == NULL) {
                        // Utilisation d'une variable non initialisee
                        msgerr = "Une variable ne peut pas etre utilisee avant d'avoir ete initialisee";
                        goto erreur;
                    } else {
                        p = variable[pos];
                    }
                } else {
                    p = varTemp[pos];
                }
                // Empile le pointeur
                empiler(p);

                goto boucle;
            }



            //On assigne une variable
            if (c == '=') {
                // Cherche le nom de la variable
                c = getchar();
                char pos;
                if (c >= 'a' && c <= 'z') {
                    // Trouve la position de la variable dans le tableau
                    pos = c - 'a';
                    // Verifie le nom de la variable
                    c = getchar();
                    if (c != ' ' && c != '\n') {
                        msgerr = "Nom de variable incorrect.";
                        goto erreur;
                    }
                } else {
                    // Si il y a autre chose qu'une lettre apres le =
                    msgerr = "Nom de variable incorrect.";
                    goto erreur;
                }
                // Si il y a deja une variable temporaire
                if (varTemp[pos] != NULL) {
                    //Ajoute l'ancienne modification dans la pile a liberer a la fin de la ligne
                    Noeud *temp = malloc(sizeof(Noeud));
                    if (temp == NULL) {
                        msgerr = "Il n'y a pas assez d'espace en memoire pour faire les operations.";
                        goto erreur;
                    }
                    (*temp).elem.nb = varTemp[pos];
                    varTemp[pos] = NULL;
                    (*temp).suivant = varALiberer;
                    varALiberer = temp;
                }
                // assigne le nombre sur le dessus de la pile a la variable
                if (pile != NULL) {
                    varTemp[pos] = (*pile).elem.nb;
                    (*varTemp[pos]).reference++;
                // Si il n'y a pas de nombre a assigner sur la pile
                } else {
                    msgerr = "Il n'y a pas de valeur a assigner";
                    goto erreur;
                }


                goto boucle;
            }



            //On lit un caractère invalide
            msgerr = "Lecture d'un caractere invalide.";
            goto erreur;
        }



        // Fin de la ligne
        // Afficher la reponse
        Nombre *res;
        if (pile != NULL) {
            depiler(res);
        } else {
            msgerr = "Il manque d'operande.";
            goto erreur;
        }
        // Si il reste plus qu'un pointeur sur la pile, il y a une erreur
        if (pile != NULL) {
            libererNombre(res);
            msgerr = "Il manque d'operateur.";
            goto erreur;
        }
        // Convertie le Nombre en chaine de caractere
        char *chaine = nombreAChaine(res);
        if (chaine == NULL) {
            libererNombre(res);
            goto erreur;
        }
        printf("%s\n",chaine);
        free(chaine);
        libererNombre(res);
        // Enregistre les modifications de variable
        for (i = 0; i < 26; i++) {
            if (varTemp[i] != NULL) {
                if (variable[i] != NULL) {
                    (*variable[i]).reference--;
                    libererNombre(variable[i]);
                }
                variable[i] = varTemp[i];
                varTemp[i] = NULL;
            }
        }
        //Vide la pile de variable a liberer
        while (varALiberer != NULL) {
            Nombre *temp = (*varALiberer).elem.nb;
            Noeud *noeudTemp = varALiberer;
            varALiberer = (*noeudTemp).suivant;
            free(noeudTemp);
            (*temp).reference--;
            libererNombre(temp);
        }

        continue;



        erreur:
        printf("%s\n", msgerr);
        // Vide la pile et le buffer
        vider();
        //Annuler les modifications de variable
        for (i = 0; i < 26; i++) {
            if (varTemp[i] != NULL) {
                (*varTemp[i]).reference--;
                libererNombre(varTemp[i]);
                varTemp[i] = NULL;
            }
        }
        //Vide la pile de variable a liberer
        while (varALiberer != NULL) {
            Nombre *temp = (*varALiberer).elem.nb;
            Noeud *noeudTemp = varALiberer;
            varALiberer = (*noeudTemp).suivant;
            free(noeudTemp);
            (*temp).reference--;
            libererNombre(temp);
        }


    } while (c != EOF);

    // Avant de terminer le programme
    //Vide la pile et le buffer
    vider();
    //Libere les variables
    for (i = 0; i < 26; i++) {
        if (variable[i] != NULL) {
            (*variable[i]).reference--;
            libererNombre(variable[i]);
            variable[i] = NULL;
        }
        if (varTemp[i] != NULL) {
            (*varTemp[i]).reference--;
            libererNombre(varTemp[i]);
            varTemp[i] = NULL;
        }
    }
    while (varALiberer != NULL) {
            Nombre *temp = (*varALiberer).elem.nb;
            Noeud *noeudTemp = varALiberer;
            varALiberer = (*noeudTemp).suivant;
            free(noeudTemp);
            (*temp).reference--;
            libererNombre(temp);
        }
    return 0;
}
