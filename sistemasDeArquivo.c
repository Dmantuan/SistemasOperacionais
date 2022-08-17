#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct diretorios{ /*-------------------------------------Adicionar data/time----------------------------------------*/
    char owner[100];
    char nome[14];
    int ID;
    int type;
    int cont;
    struct diretorios *conteudo;/*Conteudo dos diretorios dentro do usuario*/
    struct diretorios *nextDir; 
    int dirPai;
}  DIR;

typedef struct usuario{
    char nome[100];
    char login[100];
    char password[16];
    char endereco[100];
    DIR *nextDir; /*Onde esta tudo o que o usuario contem*/
    struct usuario *nextUser;   /*Define o proximo usuario*/
} USER;

typedef struct road{
    char name[100];
    struct road *next;
} ROAD;

void UPPERCASE(char *comand);
int DELETARUSR(char *login, USER* user);
USER* BuscaUsuario(USER *user, char *login, char *password, ROAD *road);
USER* commands(char *command, USER *user, ROAD* road, USER *userTree);
int CRIARUSR(char *nome, char *login, char *senha,USER *user);
USER* arvoreUsuarios();
void WriteSystemUsers(USER *user);
void HELP();
DIR* arvoreDeArquivosRecursiva(DIR* arqTree, DIR* auxArqTree, FILE *file);
DIR* arvoreArquivos(USER* user, ROAD *road);
DIR* buscaDir(ROAD *road,DIR *dir,DIR *auxDir);
int CDIR(USER* user, char* name, ROAD *road);
void WriteSystemDir(DIR *arqTree, USER *user);
DIR* WriteSytemDirRecursiva(DIR *arqTree, FILE *file);
int CARQ(USER* user, char* name, ROAD *road);
DIR* LISTARATR(USER* user, char* name, ROAD *road);
int RDIR(USER* user, char* name, ROAD *road);
int APAGAR(USER* user, char* name, ROAD *road);
DIR* ApagarRecursivo(DIR *dir);
void LISTAR(USER* user, ROAD *road);
void LISTARTUDO(USER* user, ROAD *road);
DIR* listartudoRecursivo(DIR *dir, int contador);
void MUDAR(USER* user, char *name, ROAD *road);
void MUDAR_ANT(ROAD *road);
DIR* buscaArqTree(DIR *arqTree, int ID, DIR *aux1ArqTree);

int main(){
    char login[100], password[16], comandos[100];
    USER *userTree, *user;
    FILE *file;
    ROAD *road=NULL, *auxRoad;
    road=(ROAD*)malloc(sizeof(ROAD));
    road->next=NULL;
    srand(time(NULL));
    /*Programa de fato esta aqui*/
    while (1){
        userTree=arvoreUsuarios(road);
        while(1){ /*Loop autenticacao do usuario*/
            printf("\t_Bem vindo_\n");
            printf("Login: ");
            scanf("%s", login);
            printf("Senha: ");
            scanf("%s", password);
            user=BuscaUsuario(userTree, login, password,road);
            
            if(user==NULL){
                printf("\n***Usuario nao encontrado***\n");
            }
            else{
                break;
            }
        }
        while(1){ /*Loop funcionalidades*/
            auxRoad=road;
            printf("C:");
            while(auxRoad->next!=NULL){
                printf("\\%s", auxRoad->name);
                auxRoad=auxRoad->next;
            }
            printf("\\%s ",auxRoad->name);
            scanf("%s", comandos);
            UPPERCASE(comandos);
            user=commands(comandos, user, road, userTree);
        }
    }
    free(road);
    return 0;
}

/*Precisa de acertos*/

USER* commands(char *command, USER *user, ROAD *road, USER *userTree){
    if(strcmp(command,"HELP")==0){
        HELP();
        return user;
    }
    else{
        if(strcmp(command,"CRIARUSR")==0){
            char nome[100], login[100], password[16];
            scanf("%s %s %s", nome, login, password);
            if(strcmp(user->login, "admin")==0){
                int controle=0;
                controle=CRIARUSR(nome, login, password, userTree);
                if(controle==1){                   
                    printf("\nUsuario criado com sucesso!!\n\n");
                    return user;
                }
                else{
                    printf("\n***Nao foi possivel criar o usuario***\n\n");
                    return user;
                }
            }
            else{
                printf("Voce nao tem acesso a este comando");
                return user;
            }
        }
        else{
            if(strcmp(command, "DELETARUSR")==0){
                char login[100];
                scanf("%s", login);
                if(strcmp(user->login, "admin")==0){
                    int controle;
                    controle=DELETARUSR(login, user);
                    if(controle==1){
                        printf("\nUsuario deletado com sucesso!\n\n");
                        return user;
                    }
                    else{
                        return user;
                    }
                }
                else{
                    printf("Voce nao tem acesso a este comando\n");
                    return user;
                }
            }
            else{
                if(strcmp(command, "ALTERARUSR")==0){
                    USER *auxUser;
                    auxUser=userTree;
                    char login[100], password[16];
                    scanf("%s %s", login, password);
                    auxUser=BuscaUsuario(auxUser, login, password, road);
                    if(auxUser!=NULL){
                        printf("\nUsuario alterado com sucesso!\n\n");
                        return auxUser;
                    }
                    else{
                        printf("\n*** Nao foi possivel alterar o usuario***\n");
                        return user;
                    }
               }
                else{
                    if(strcmp(command, "CDIR")==0){
                        char dirNome[14];
                        int controlador;
                        scanf("%s", &dirNome);
                        controlador= CDIR(user, dirNome, road);
                        if(controlador==0){
                            printf("\n***ERRO AO CRIAR O DIRETORIO\n");
                        }
                        else{
                            printf("\n Diretorio criado com sucesso!\n");
                        }
                        return user;
                    }
                    else{
                        if(strcmp(command, "CARQ")==0){
                            char dirNome[14];
                            int controlador;
                            scanf("%s", &dirNome);
                            controlador= CARQ(user, dirNome, road);
                            if(controlador==0){
                                printf("\n***ERRO AO CRIAR O ARQUIVO\n");
                            }
                            else{
                                printf("\n ARQUIVO criado com sucesso!\n");
                            }
                            return user;
                        }
                        else{
                            if(strcmp(command, "LISTARATR")==0){
                                char dirName[14];
                                DIR *dirReponse;
                                scanf("%s", dirName);
                                dirReponse=LISTARATR(user,dirName,road);
                                if(dirReponse==NULL){
                                    printf("\n*** Problema ao localizar o DIR/ARQ ***\n");
                                }
                                else{
                                    printf("\n\n#------------------\n");
                                    if(dirReponse->type==1){
                                        printf("|Nome:%s\n|ID: %d\n|Tipo:DIR\n|Proprietario: %s\n", dirReponse->nome, dirReponse->ID, dirReponse->owner);
                                    }
                                    else{
                                        printf("|Nome:%s\n|ID: %d\n|Tipo:DIR\n|Proprietario: %s\n", dirReponse->nome, dirReponse->ID, dirReponse->owner);
                                    }
                                    printf("#------------------\n\n");
                                }
                                return user;
                            }
                            else{
                                if(strcmp(command, "RDIR")==0){
                                    char nameDir[14];
                                    int controle;
                                    scanf("%s", nameDir);
                                    controle=RDIR(user,nameDir,road);
                                    if(controle == 1){
                                        printf("\n\nDIR:%s Removido com sucesso\n\n",nameDir);
                                    }
                                    else{
                                        printf("\n\n*** Falha ao remover o DIR: %s ***\n\n", nameDir);
                                    }
                                    return user;
                                }
                                else{
                                    if(strcmp(command, "APAGAR")==0){
                                        char dirName[14];
                                        int controle;
                                        scanf("%s", dirName);
                                        controle = APAGAR(user,dirName,road);
                                        if(controle==1){
                                            printf("\n\nDIR:%s Removido com sucesso\n\n",dirName);
                                        }   
                                        else{
                                            printf("\n\n*** Falha ao remover o DIR: %s ***\n\n", dirName);
                                        }               
                                        return user;                      
                                    }
                                    else{
                                        if(strcmp(command, "CLEAR")==0||strcmp(command,"CLS")==0){
                                            system("clear");
                                            return user;
                                        }
                                        else{
                                            if(strcmp(command, "LISTAR")==0){
                                                LISTAR(user,road);
                                                return user;
                                            }
                                            else{
                                                if(strcmp(command, "ATUAL")==0){
                                                    ROAD* auxRoad;
                                                    auxRoad=road;
                                                    while(auxRoad->next != NULL){
                                                        auxRoad=auxRoad->next;           /*Final do road*/
                                                    }
                                                    printf("%s\n", auxRoad->name);
                                                    return user;
                                                }
                                                else{
                                                    if(strcmp(command,"LISTARTUDO")==0){
                                                        LISTARTUDO(user,road);
                                                        return user;
                                                    }
                                                    else{
                                                        if(strcmp(command, "MUDAR")==0||strcmp(command, "CD")==0){
                                                            char dirName[14];
                                                            scanf("%s",dirName);
                                                            if(strcmp(dirName,"..")==0){
                                                                MUDAR_ANT(road);
                                                            }
                                                            else{
                                                                MUDAR(user, dirName,road);
                                                            }
                                                            return user;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return user;
}

void HELP(){
    printf("\n|-------------------------------|\n");
    printf("CRIARUSR <nome_usuario> <login> <senha> - cria um novo usuario.\n");
    printf("ALTERARUSR <login_usuario_dest> <senha_usuario_dest> - fara o login do novo usuario\n");
    printf("DELETARUSR <login> - Apagara o usuario. Automaticamente apagara o seu diretorio.\n");
    printf("SAIR - logout do sistema.\n");
    printf("CDIR <nome_do_diretório> - cria um novo diretório\n");
    printf("CARQ <nome_do_arquivo> - cria um novo arquivo\n");
    printf("LISTARATR <nome_do_arq_ou_dir> - lista os atributos de um determinado arquivo ou diretório\n");
    printf("RDIR <nome_do_dir> - apaga um diretório vazio\n");
    printf("APAGAR <nome> - apaga um arquivo ou um diretório com arquivos (e faz isso recursivamente)\n");
    printf("LISTAR - lista o conteúdo do diretório atual\n");
    printf("CLEAR ou CLS, limpa o console\n");
    printf("LISTAR - lista o conteúdo do diretório atual.");
    printf("LISTARTUDO - lista o conteúdo do diretório e se houver, também listará o conteúdo dos subdiretórios");
    printf("MUDAR/CD <end_destino> - altera o estado atual de uma pasta para outra qualquer sendo subdiretorio dela");
    printf("MUDAR/CD .. - altera o estado atual de uma pasta para a anterior");
    printf("|-------------------------------|\n\n");

    /*
    printf("LISTARINV – lista o conteúdo do diretório em ordem decrescente.");
    printf("ATUAL – mostra o nome do diretório atual");
    printf("COPIAR <origem> <destino> – copia um arquivo/diretório para um outro lugar informado");
    printf("RENOMEAR <nome_atual> <nome_final> – renomeia um arquivo ou diretório");
    printf("MOVER <origem> <destino> – move um arquivo/diretório para um outro lugar informado");
    printf("BUSCAR <nome_arquivo> <dir_de_busca> – busca um arquivo informado na hierarquia de diretório");
    */
}

/*Prontos para uso*/

int DELETARUSR(char *login, USER* user){
    int index=1;
    char nome[100], confimacao[5];
    USER *auxUser, *antUser;
    auxUser=user;
    while(auxUser!=NULL){
        if(strcmp(auxUser->login, login)==0){
            while(1){
                printf("Deseja excluir o usuario %s?  sim/s\tnao/n\n", auxUser->login);
                scanf("%s", confimacao);
                UPPERCASE(confimacao);
                if(strcmp(confimacao,"SIM")==0||strcmp(confimacao,"S")==0){
                    antUser->nextUser=auxUser->nextUser;
                    remove(auxUser->endereco);
                    free(auxUser);
                    WriteSystemUsers(user);
                    return 1;
                }
                else{
                    if(strcmp(confimacao,"NAO")||strcmp(confimacao, "N")){
                        printf("\n***Operacao cancelada***\n\n");
                        return 0;
                    }
                    else{
                        printf("\n****Resposta invalida****\n");
                    }
                }
            }
            
        }
        antUser=auxUser;
        auxUser=auxUser->nextUser;
    }
    printf("\n***Operacao cancelada, usuario invalido.***\n\n");
    return 0;

}

void WriteSystemUsers(USER *user){
    USER *auxUser;
    FILE *file;
    file= fopen("SystemUsers.txt", "w");

    auxUser=user;
    while(auxUser->nextUser!=NULL){
        fprintf(file,"%s %s %s %s\n", auxUser->endereco, auxUser->nome, auxUser->login, auxUser->password);
        auxUser=auxUser->nextUser;
    }
    fprintf(file,"%s %s %s %s", auxUser->endereco, auxUser->nome, auxUser->login, auxUser->password);
    fclose(file);
}

void UPPERCASE(char *command){
    for(int i=0; i<strlen(command);i++){   /*Convertendo os comando para maiusculo*/
        command[i]=toupper(command[i]);
    }
}

int CRIARUSR(char *nome, char *login, char *password,USER *user){
    USER *newUser, *auxList;
    FILE *file;
    nome[0]=toupper(nome[0]);
    newUser=(USER*)malloc(sizeof(USER));
    if(newUser==NULL){
        return 0;
    }
    else{
        newUser->nextDir=NULL;
        newUser->nextUser=NULL;
        auxList=user;
        while(auxList->nextUser!=NULL){
            if(strcmp(auxList->login,login)==0){
                return 0;
            }
            auxList=auxList->nextUser;
        }
        if(strcmp(auxList->login,login)==0){
            return 0;
        }
        auxList->nextUser=newUser;
        strcpy(newUser->nome,nome);
        strcpy(newUser->login,login);
        strcpy(newUser->password,password);
        
        /*Criando o arquivo do usr*/
        strcat(login, ".txt");
        strcpy(newUser->endereco, login);
        WriteSystemUsers(user);
        file=fopen(newUser->endereco,"w");
        fprintf(file,"%d %s %s 1 0 -1", rand()%50, user->nome, newUser->nome);
        fclose(file);

        return 1;
    }   
}

USER* BuscaUsuario(USER *user, char *login, char *password, ROAD* road){
    while(user != NULL){
        if(strcmp(login,user->login)==0 &&strcmp(password,user->password)==0){
            strcpy(road->name, user->nome);
            road->next=NULL;
            return user;
        }
        user= user->nextUser;
    }
    return NULL;
}

USER* arvoreUsuarios(){
    FILE* file;
    USER *user, *newUser, *auxLista;
    
    user= (USER*)malloc(sizeof(USER));
    user->nextUser=NULL;
    user->nextDir=NULL;

   /*Ve se o arquivo System.txt ja existe se nao o cria*/
    file = fopen("SystemUsers.txt", "r");
    if (file == NULL){
        /*admin default*/
        strcpy(user->nome, "root");
        strcpy(user->login, "admin");
        strcpy(user->password, "admin");
        strcpy(user->endereco, "root");

        file = fopen("SystemUsers.txt", "w"); /*Adicionando o admin no arquivo*/
        fprintf(file,"%s %s %s %s", user->endereco, user->nome, user->login, user->password);
        fclose(file);
        return user;
    }

    /*De fato fazendo uma lista encadeada sendo o primeiro o root, pq o root e sempre o primeiro do arquivo*/
    fscanf(file, "%s %s %s %s", user->endereco, user->nome, user->login, user->password);  /*Le uma vez e pula a linha no arquivo*/


    auxLista=user;
    while(!feof(file)){ 
        /*Iniciando o proximo usuario da lista*/
        newUser=(USER*)malloc(sizeof(USER));
        
        fscanf(file, "%s %s %s %s", newUser->endereco, newUser->nome, newUser->login, newUser->password);
        newUser->nextDir=NULL;
        newUser->nextUser=NULL;

        auxLista->nextUser=newUser;
        auxLista=auxLista->nextUser;
    }
    return user;
}

DIR* arvoreArquivos(USER* user, ROAD* road){
    FILE* file;
    DIR *arqTree, *auxArqTree;
    arqTree=(DIR*)malloc(sizeof(DIR));
    file=fopen(user->endereco,"r");
    if(file==NULL){
        printf("problema ao abrir o arquivo");
    }

    /*Inicializando o arqTree, ou seja o diretorio referente ao arquivo de urs*/
    fscanf(file,"%d %s %s %d %d %d", &arqTree->ID, arqTree->owner, arqTree->nome, &arqTree->type, &arqTree->cont ,&arqTree->dirPai);
    arqTree->nextDir=NULL;
    arqTree->conteudo=NULL;
    auxArqTree=arqTree;
    arqTree=arvoreDeArquivosRecursiva(auxArqTree, auxArqTree, file);
    fclose(file);
    return arqTree;
}

DIR* arvoreDeArquivosRecursiva(DIR* arqTree, DIR* auxArqTree, FILE *file){
    if(!feof(file)){
        DIR *newDir;
        newDir=(DIR*)malloc(sizeof(DIR));
        fscanf(file,"%d %s %s %d %d %d", &newDir->ID, newDir->owner, newDir->nome, &newDir->type, &newDir->cont, &newDir->dirPai);
        newDir->nextDir=NULL;
        newDir->conteudo=NULL;

        if(auxArqTree->ID == newDir->dirPai){
            auxArqTree->conteudo=newDir;
        }
        else{
            DIR* aux1ArqTree;
            auxArqTree=buscaArqTree(arqTree,newDir->dirPai,aux1ArqTree);
            auxArqTree=auxArqTree->conteudo;
            while(auxArqTree->nextDir!=NULL){
                auxArqTree=auxArqTree->nextDir;
            }
            auxArqTree->nextDir=newDir;
        }
        arvoreDeArquivosRecursiva(arqTree,newDir,file);
    }
    return auxArqTree; 
}

DIR* buscaArqTree(DIR *arqTree, int ID, DIR *aux1ArqTree){
    if(arqTree!=NULL){
        aux1ArqTree=buscaArqTree(arqTree->conteudo, ID,aux1ArqTree);
        aux1ArqTree=buscaArqTree(arqTree->nextDir, ID,aux1ArqTree);
        if(arqTree->ID==ID){
            return(arqTree);
        }
    }
    return(aux1ArqTree);
}

DIR* buscaDir(ROAD *road,DIR *dir, DIR *auxDir){
    if(dir!=NULL){
        if(strcmp(road->name,dir->nome)==0||strcmp(auxDir->nome,road->name)==0){
            return dir;
        }
        else{
            auxDir=buscaDir(road,dir->conteudo,auxDir);
            auxDir=buscaDir(road,dir->nextDir,auxDir);
        }
    }
    return auxDir;
}

int CDIR(USER* user, char* name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    if(dirAtual==NULL){
        return 0;
    }
    else{
        newDir = (DIR*)malloc(sizeof(DIR));
        strcpy(newDir->nome, name);
        if(dirAtual->dirPai==-1){
            strcpy(newDir->owner, dirAtual->nome);            
        }
        else{
            strcpy(newDir->owner, dirAtual->owner);
        }
        
        newDir->ID=50+rand();
        newDir->dirPai=dirAtual->ID;
        newDir->type=1;
        newDir->cont=0;
        newDir->nextDir=NULL;
        newDir->conteudo=NULL;

        if(dirAtual->cont==1){
            dirAtual=dirAtual->conteudo;
            while(dirAtual->nextDir!=NULL){
                dirAtual=dirAtual->nextDir;
            }
            dirAtual->nextDir=newDir;
        }
        else{
            dirAtual->cont=1;
            dirAtual->conteudo=newDir;
        }


        WriteSystemDir(arqTree,user);
        return 1;
    }
}

int CARQ(USER* user, char* name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    if(dirAtual==NULL){
        return 0;
    }
    else{
        newDir = (DIR*)malloc(sizeof(DIR));
        strcpy(newDir->nome, dirAtual->owner);
        strcpy(newDir->nome, name);
        if(dirAtual->dirPai==-1){
            strcpy(newDir->owner, dirAtual->nome);            
        }
        else{
            strcpy(newDir->owner, dirAtual->owner);
        }
        
        newDir->ID=50+rand();
        newDir->dirPai=dirAtual->ID;
        newDir->type=0;
        newDir->cont=0;
        newDir->nextDir=NULL;
        newDir->conteudo=NULL;

        if(dirAtual->cont==1){
            dirAtual=dirAtual->conteudo;
            while(dirAtual->nextDir!=NULL){
                dirAtual=dirAtual->nextDir;
            }
            dirAtual->nextDir=newDir;
        }
        else{
            dirAtual->cont=1;
            dirAtual->conteudo=newDir;
        }
        WriteSystemDir(arqTree,user);
        return 1;
    }
}

DIR* LISTARATR(USER* user, char* name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    dirAtual=dirAtual->conteudo;
    while(dirAtual!=NULL){
        if(strcmp(dirAtual->nome,name)==0){
            return dirAtual;
        }
        dirAtual=dirAtual->nextDir;
    }
    return NULL;
}

void LISTAR(USER* user, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    
    printf("%s ",dirAtual->nome);
    dirAtual=dirAtual->conteudo;
    printf("\n\nID\tProprietario\tNome\tTipo\n");
    while(dirAtual!=NULL){
        printf("%d\t%s\t\t%s\t",dirAtual->ID,dirAtual->owner,dirAtual->nome);
        if(dirAtual->type==1){
            printf("[DIR]\n\n");
        }
        else{
            printf("[ARQ]\n\n");
        }
        dirAtual=dirAtual->nextDir;
    }
}

void LISTARTUDO(USER* user, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    dirAtual=dirAtual->conteudo;

    listartudoRecursivo(dirAtual,0);

    printf("\n");
}

DIR* listartudoRecursivo(DIR *dir, int contador){
    if(dir!=NULL){
        int contAux=contador;
        for(contAux;contAux>0;contAux--){
            printf("\t"); 
        }
        if(contador!=0){
            printf("\\___ ");
        }
        if(dir->type==1){
            printf("%s [DIR]\n",dir->nome);
        }
        else{
            printf("%s [ARQ]\n", dir->nome);
        }

        
        listartudoRecursivo(dir->conteudo, contador+1);
        listartudoRecursivo(dir->nextDir, contador);
    }
}

int RDIR(USER* user, char* name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    if(dirAtual==NULL){
        return 0;
    }
    else{
        DIR *auxDIR;
        auxDIR=dirAtual;
        dirAtual=dirAtual->conteudo;
        while(dirAtual!=NULL){
            if(strcmp(dirAtual->nome,name)==0){
                if(dirAtual->cont==0 && dirAtual->type==1){
                    if(dirAtual->dirPai==auxDIR->ID){
                        auxDIR->conteudo=dirAtual->nextDir;
                        if(dirAtual->nextDir==NULL){
                            auxDIR->cont=0;
                        }
                    }
                    else{
                    auxDIR->nextDir=dirAtual->nextDir;
                    }
                
                    WriteSystemDir(arqTree,user);
                    return 1;
                }
                return 0;
            }
            auxDIR=dirAtual;
            dirAtual=dirAtual->nextDir;
        }
        return 0;
    }   
}

int APAGAR(USER* user, char* name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);
    if(dirAtual==NULL){
        return 0;
    }
    else{
        DIR *auxDIR;
        auxDIR=dirAtual;
        dirAtual=dirAtual->conteudo;
        while(dirAtual!=NULL){
            if(strcmp(dirAtual->nome,name)==0){
                if(dirAtual->cont==0){
                    if(dirAtual->dirPai==auxDIR->ID){
                        auxDIR->conteudo=dirAtual->nextDir;
                        if(dirAtual->nextDir==NULL){
                            auxDIR->cont=0;
                        }
                    }
                    else{
                    auxDIR->nextDir=dirAtual->nextDir;
                    }

                    free(dirAtual);
                    WriteSystemDir(arqTree,user);
                    return 1;
                }
                else{
                    dirAtual->conteudo=ApagarRecursivo(dirAtual->conteudo);
                    
                    if(dirAtual->dirPai==auxDIR->ID){
                        auxDIR->conteudo=dirAtual->nextDir;
                        if(dirAtual->nextDir==NULL){
                            auxDIR->cont=0;
                        }
                    }
                    else{
                    auxDIR->nextDir=dirAtual->nextDir;
                    }

                    free(dirAtual);
                    WriteSystemDir(arqTree,user);
                    return 1;
                    
                }
                return 0;
            }
            auxDIR=dirAtual;
            dirAtual=dirAtual->nextDir;
        }
        return 0;
    }
    
}

DIR* ApagarRecursivo(DIR *dir){
    if(dir!=NULL){
        ApagarRecursivo(dir->conteudo);
        ApagarRecursivo(dir->nextDir);
        free(dir);  
    }
    return NULL;
}

void MUDAR(USER* user, char *name, ROAD *road){
    DIR *arqTree, *dirAtual, *newDir;
    ROAD *auxRoad;
    auxRoad=road;
    arqTree=arvoreArquivos(user,road);   /*Monta a arvore de arquivos dentro do usr.txt*/
    while(auxRoad->next != NULL){
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    dirAtual=buscaDir(auxRoad,arqTree,arqTree);  /*Verifica se ja existe um dir com esse nome*/
    printf("%s ",dirAtual->nome);

    if(dirAtual->cont=1){
        dirAtual=dirAtual->conteudo;
        while(dirAtual!=NULL){

            if(strcmp(dirAtual->nome, name)==0 && dirAtual->type==1){
                ROAD *newRoad;
                newRoad = (ROAD*)malloc(sizeof(ROAD));
                strcpy(newRoad->name, dirAtual->nome);
                newRoad->next=NULL;
                auxRoad->next=newRoad;
                break;
            }

            dirAtual=dirAtual->nextDir;
        }
        if(dirAtual==NULL){
            printf("\n***Diretorio informado nao existe***\n\n");
        }
    }
    else{
        printf("\n*** O diretorio atual esta vazio ***\n\n");
    }
}

void MUDAR_ANT(ROAD *road){
    int controle=0;
    ROAD *auxRoad, *antAuxRoad;
    auxRoad=road;
    antAuxRoad;

    while(auxRoad->next != NULL){
        controle++;
        antAuxRoad=auxRoad;
        auxRoad=auxRoad->next;           /*Final do road*/
    }
    if(controle!=0){
        free(auxRoad);
        antAuxRoad->next=NULL;
    }
    else{
        printf("\n*** Nao e possivel mudar para um DIR anterior ao do usuario ***\n\n");
    }

}

void WriteSystemDir(DIR *arqTree, USER *user){
    FILE *file;
    file=fopen(user->endereco, "w");

    WriteSytemDirRecursiva(arqTree, file);
    fclose(file);
}

DIR* WriteSytemDirRecursiva(DIR *arqTree, FILE *file){
    if(arqTree!=NULL){
        if(arqTree->dirPai!=-1){
            fprintf(file,"\n");
        }
        fprintf(file,"%d %s %s %d %d %d", arqTree->ID, arqTree->owner, arqTree->nome, arqTree->type, arqTree->cont, arqTree->dirPai);
        if(arqTree->type==1){
            if(arqTree->cont==1){
                arqTree->conteudo=WriteSytemDirRecursiva(arqTree->conteudo,file);
                arqTree->nextDir=WriteSytemDirRecursiva(arqTree->nextDir,file);
            }
            else{
                arqTree->nextDir=WriteSytemDirRecursiva(arqTree->nextDir,file);
            }
        }
        else{
            arqTree->nextDir=WriteSytemDirRecursiva(arqTree->nextDir,file);
        }
    }
    else{
        return NULL;
    }
}
