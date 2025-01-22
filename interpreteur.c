#include "src/header.h"

int PC=0; //prochaine instruction a executer
int SP=0;
int nbLignesVariable=0;
int* nbLignes=&nbLignesVariable;

int memoire[TAILLE_MEMOIRE];
int pile[TAILLE_PILE];
short programme[TAILLE_PROGRAMME][2];  //on est en short, ca nous facilite la vie pour les étiquettes, la conversion est faite automatiquemeznt lorsqu'il faut remonter le programme

void operations(int op);

void remplirTableau(char* fichierLM) //on récupère le fichier dans le tableau programme, et on le convertit (hexa->int)
{
    FILE* fichier= fopen(fichierLM,"r");
    if (!fichier){
        printf("erreur, fichier non trouve\n");
        exit(EXIT_FAILURE);
    }
    int opcode, donnee, indice=0;

    while( fscanf(fichier, "%2x %x", &opcode, &donnee)==2){
        programme[indice][0]=opcode;
        programme[indice][1]=donnee;
        indice+=1;
        *nbLignes+=1;
    } //%2x car 2 chiffres ecrit en hexa, et "==2" car la fonction renvoie le nombre d'éléments bien lus
    fclose(fichier);
}

void afficheTableau(short T[TAILLE_PROGRAMME][2]){
    for(int i=0;i<*nbLignes;i++){
        for(int j=0;j<2;j++){
            printf("%d ", T[i][j]);
        }
        printf("\n");
    }
}



void testOverflow(){
    if (SP<=1){
				printf("erreur ligne %d, la pile ne contient pas 2 elements", PC);
				exit(EXIT_FAILURE);
			}
}

void instructions(){
    int commande=programme[PC-1][0];
    int donnee=programme[PC-1][1];
    printf("Ligne=%d, PC=%d, SP=%d, Commande=%d, Donnee=%d\n",PC, PC, SP, commande, donnee);
    switch(commande){  //on exprime toutes les differentes commandes possibles + les differentes operations
        case(0): //pop x
            if(SP<=0){
                printf("Erreur, la pile est vide\n");
                exit(EXIT_FAILURE);
            }
            if(donnee<0 || donnee>TAILLE_MEMOIRE){
                printf("Erreur d'indice\n");
                exit(EXIT_FAILURE);
            }
            memoire[donnee]=pile[SP-1];
            SP--;   
            break;
        case(1): //ipop
            if(SP<=1){
                printf("Erreur, la pile a moins d'1 élément\n");
                exit(EXIT_FAILURE);
            }
            int n_case1=pile[SP-1];
            if(n_case1<0 || n_case1>TAILLE_MEMOIRE){
                printf("erreur, n n'est pas une adresse valide\n");
                exit(EXIT_FAILURE);
            }
            memoire[n_case1]=pile[SP-2];
            SP-=2;
            break;
        case(2): //push x
            if(SP==TAILLE_PILE){
                printf("Plus assez de place pour stocker l'élément\n");
                exit(EXIT_FAILURE);
            }
            if (donnee>=TAILLE_MEMOIRE || donnee<0){
                printf("l'adresse n'est pas valide\n");
                exit(EXIT_FAILURE);
            }
            pile[SP]=memoire[donnee];
            SP+=1;
            break;
        case(3):  //ipush
            if(SP==0){
                printf("Pile vide\n");
                exit(EXIT_FAILURE);
            }
            int n_case3=pile[SP-1];
            if (n_case3 < 0 || n_case3 >= TAILLE_MEMOIRE) {
                printf("Erreur, l'adresse n est invalide\n");
                exit(EXIT_FAILURE);
            }
            pile[SP-1]=memoire[n_case3];
            break;
        case(4): //push# i
            if(SP==TAILLE_PILE){
                printf("Plus assez de place pour stocker l'élément\n");
                exit(EXIT_FAILURE);
            }
            pile[SP]=donnee;
            SP+=1;
            break;
        case(5): //jmp adr
            PC+=donnee;
            break;
        case(6): //jnz adr
            if(SP==0){
                printf("erreur a la ligne %d, la pile est vide\n",PC);
                exit(EXIT_FAILURE);
				}
            if(pile[SP-1]!=0){
				  PC+=donnee;
				}
			 SP--;
            break;
        case(7): //call adr
            if(SP==TAILLE_PILE){
                printf("Plus d'espace de stockage\n");
                exit(EXIT_FAILURE);
            }
            pile[SP]=PC;
            SP++;
            PC+=donnee;
            break;
        case(8): //ret
            if(SP <= 0) {
                    printf("Erreur, pile vide lors du RET\n");
                    exit(EXIT_FAILURE);
            }
            PC=pile[SP-1];
            SP--;
            break;
        case(9): //read x
            if (donnee < 0 || donnee >= TAILLE_MEMOIRE) {
                printf("Erreur d'indice\n");
                exit(EXIT_FAILURE);
            }
            int k_case9;
            printf("Entrez une valeur pour l'adresse %d\n", donnee);
            scanf("%d", &k_case9); //petit entier
            memoire[donnee]=k_case9;
            break;
        case(10): //write x
            if (donnee < 0 || donnee >= TAILLE_MEMOIRE) {
                printf("Erreur d'indice\n");
                exit(EXIT_FAILURE);
            }
            printf("%d\n",memoire[donnee]);
            break;
        case(11): //on appelle operations() qui lui aussi est un switch
            operations(donnee);
            break;
        case(12): //rnd x
            if(SP==TAILLE_PILE){
                printf("Plus assez de place pour stocker l'élément\n");
                exit(EXIT_FAILURE);
            }
            int rd_num = rand() % (donnee);
            pile[SP]=rd_num;
            SP++;
            break;
        case(13): //dup
            if(SP==TAILLE_PILE){
                printf("Plus assez de place pour stocker l'élément\n");
                exit(EXIT_FAILURE);
            }
            pile[SP]=pile[SP-1];
            SP++;
            break;
        case(99): //halt
            printf("fin du programme!");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("aucune commande ne corrsepond a la valeur de la ligne %d", PC);
            exit(EXIT_FAILURE);
            break;
    }
}

void operations(int op){
	switch(op){
		case(0): //egalité
			testOverflow();
			SP--;
			if (pile[SP-1]==pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(1): //inegalité
			testOverflow();
			SP--;
			if (pile[SP-1]!=pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(2):
			testOverflow();
			SP--;
			if (pile[SP-1]>=pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(3):
			testOverflow();
			SP--;
			if (pile[SP-1]<=pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(4):
			testOverflow();
			SP--;
			if (pile[SP-1]>pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(5):
			testOverflow();
			SP--;
			if (pile[SP-1]<pile[SP]){
				pile[SP-1]=1;
			}
			else{
				pile[SP-1]=0;
			}
			break;
		case(6):
			testOverflow();
			SP--;
			pile[SP-1]= pile[SP-1]| pile[SP];
			break;
		case(7):
			testOverflow();
			SP--;
			pile[SP-1]= pile[SP-1]^pile[SP];
			break;
		case(8):
			testOverflow();
			SP--;
			pile[SP-1]= pile[SP-1]&pile[SP];
			break;
		case(9):
			if(SP==0){
                printf("erreur a la ligne %d,la pile est vide\n",PC);
                exit(EXIT_FAILURE);
				}
			pile[SP-1]= ~pile[SP-1];
			break;
		case(10):
			testOverflow();
			SP--;
			pile[SP-1]+=pile[SP];
			break;
		case(11):
			testOverflow();
			SP--;
			pile[SP-1]-=pile[SP];
			break;
		case(12):
			testOverflow();
			SP--;
			pile[SP-1]*=pile[SP];
			break;
		case(13):
			testOverflow();
			if (pile[SP-1]==0){
				printf("erreur, division par 0");
				exit(EXIT_FAILURE);
			}
			SP--;
			pile[SP-1]=pile[SP-1]/pile[SP];
			break;
		case(14):
			testOverflow();
			SP--;
			pile[SP-1]=pile[SP-1]%pile[SP];
			break;
		case(15):
			if (SP==0){
				printf("erreur a la ligne %d,la pile est vide\n",PC);
				exit(EXIT_FAILURE);
			}
			pile[SP-1]= -1*pile[SP-1];
			break;
		default:
            printf("aucune commande ne corrsepond a la valeur de la ligne %d", PC);
            exit(EXIT_FAILURE);
            break;
			
	}
}


int Simulateur(){
    
    remplirTableau("hexa.txt");
    afficheTableau(programme);
    if (*nbLignes>=TAILLE_PROGRAMME){
        printf("Programme trop long, il doit faire au maximum %d lignes\n", TAILLE_PROGRAMME);
        return 0;
    }
    while (PC<*nbLignes){
        PC+=1;
        instructions();
    }
    printf("le programme s'est arrete a l'instruction %d\n",PC);
    return 0;
}