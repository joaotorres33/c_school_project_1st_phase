#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

//Utilizar estes defines abaixo para fazer verificacoes booleanas
#define bool int
#define TRUE 1
#define FALSE 0

//Declaracao de constantes
#define MAX_EQUIPAMENTOS 200
#define MAX_LABORATORIOS 15
#define MIN_LAB_EQUI 0
#define LABORATORIO_NAO_EXISTE -1
#define EQUIPAMENTO_NAO_EXISTE -1
#define INDEX_N_OCUPADO -1
#define MAX_N_SERIE 20
#define MAX_STRING 100
#define MAX_ID 10

//Declaracao das estruturas
typedef struct{
    int dia, mes, ano;
} s_tipoData;

typedef struct{
    int id;
    char localizacao[MAX_ID];
    int postos_trabalhos;
    int q_equip_instalados;
}s_lab;

typedef struct{
    int id;
    s_tipoData data;
    char descricao[MAX_STRING];
    char n_serie[MAX_N_SERIE];
    int local;
    char estado; //O estado só poderá ser ([D]isponivel, [I]ndisponivel, [A]batadio)
    float custo;
}s_equipamentos;


//Declaracao das funcoes - Menus
void f_menu_inicial(int *quantLabs, int *quantPostosTrabalho, int *quantEquipamentos, float *percEquipamentosAvariados, s_lab v_lab[MAX_LABORATORIOS]);
void f_menu_laboratorio(s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos);
void f_menu_equipamentos();
void f_menu_avarias();
void f_menu_estatisticas();

//Declaracao das funcoes - Gestao dos laboratorios
void f_inserir_laboratorio(s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos);


/*O que está a comentário foi o que eu modifiquei (não tenho a certeza se está correto)
void f_le_ficheiro_binario(s_lab v_lab[MAX_LABORATORIOS], s_equipamentos v_equipamentos[MAX_EQUIPAMENTOS], int *quantLabs, int *quantEquips);
void f_grava_ficheiro_binario(s_lab v_lab[MAX_LABORATORIOS], s_equipamentos v_equipamentos[MAX_EQUIPAMENTOS], int quantLabs, int quantEquips);*/

int lerInteiro (int limMin, int limMax, char msg[MAX_STRING]);
float lerFloat (float limMin, float limMax, char msg[MAX_STRING]);
void lerString(char msg[MAX_STRING], char str[MAX_STRING], int tamanho);
void limparBuffer (void);

int main()
{
    //Inicializacao dos vetores das estruturas
    s_lab v_lab[MAX_LABORATORIOS];
    s_equipamentos v_equipamentos[MAX_EQUIPAMENTOS];

    /*O que está a comentário foi o que eu modifiquei (não tenho a certeza se está correto)
    int quantLabs=0, quantEquips=0;*/

    int quantLabs = 0, quantPostosTrabalho = 0, quantEquipamentos = 0;
    float percEquipamentosAvariados = 0;

    f_menu_inicial(&quantLabs, &quantPostosTrabalho, &quantEquipamentos, &percEquipamentosAvariados, v_lab);
    return 0;
}


//Ficheiros, abrir e ler

void f_le_ficheiro_binario(s_lab v_lab[MAX_LABORATORIOS], s_equipamentos v_equipamentos[MAX_EQUIPAMENTOS], int *quantLabs, int *quantEquips)
{
    FILE *fich;
    int labGuardados, equipGuardados;

    fich = fopen("infoBin.dat", "rb");

    if (fich == NULL)
    {
        printf("\n\nERRO: Falha na abertura do ficheiro.\n\n");
    }
    else
    {
        equipGuardados = fread(quantEquips, sizeof(int), 1, fich);
        if (equipGuardados != 1)
        {
            printf("\n\nERRO: Falha na importacao da quantidade de equipamentos inseridos.\n\n");
        }
        else
        {
            equipGuardados = fread(v_equipamentos, sizeof(s_equipamentos), *quantEquips, fich);
            if (equipGuardados != *quantEquips)
            {
                printf("\n\nERRO: Falha na importacao dos dados inseridos.\n\n");
            }
        }
        labGuardados = fread(quantLabs, sizeof(int), 1, fich);
        if (labGuardados != 1)
        {
            printf("\n\nERRO: Falha na importacao da quantidade de laboratorios inseridos.\n\n");
        }
        else
        {
            labGuardados = fread(v_lab, sizeof(s_lab), *quantLabs, fich);
            if (labGuardados != *quantLabs)
            {
                printf("\n\nERRO: Falha na importacao dos dados inseridos.\n\n");
            }
        }
        fclose(fich);
    }
}


void f_grava_ficheiro_binario(s_lab v_lab[MAX_LABORATORIOS], s_equipamentos v_equipamentos[MAX_EQUIPAMENTOS], int quantLabs, int quantEquips)
{
    FILE *fich;
    int labGuardados, equipGuardados;

    fich = fopen("infoBin.dat", "wb");

    if (fich == NULL)
    {
        printf("\n\nERRO: Falha na abertura do ficheiro.\n\n");
    }
    else
    {
        equipGuardados = fwrite(&quantEquips, sizeof(int), MAX_EQUIPAMENTOS, fich);
        if (equipGuardados != MAX_EQUIPAMENTOS)
        {
            printf("\n\nERRO: Falha na gravacao da quantidade de equipamentos inseridos.\n\n");
        }
        else
        {
            equipGuardados = fwrite(v_equipamentos, sizeof(s_equipamentos), quantEquips, fich);
            if (equipGuardados != quantEquips)
            {
                printf("\n\nERRO: Falha na gravacao dos dados inseridos.\n\n");
            }
        }
        labGuardados = fwrite(&quantLabs, sizeof(int), MAX_LABORATORIOS, fich);
        if (labGuardados != MAX_LABORATORIOS)
        {
            printf("\n\nERRO: Falha na gravacao da quantidade de laboratorios inseridos.\n\n");
        }
        else
        {
            labGuardados = fwrite(v_lab, sizeof(s_lab), quantLabs, fich);
            if (labGuardados != quantLabs)
            {
                printf("\n\nERRO: Falha na gravacao dos dados inseridos.\n\n");
            }
        }
        fclose(fich);
    }
}

//Inicio funcoes dos menus

void f_menu_inicial(int *quantLabs, int *quantPostosTrabalho, int *quantEquipamentos, float *percEquipamentosAvariados, s_lab v_lab[MAX_LABORATORIOS]){
    char opcao;

    do{
        printf("\t| Quantidade Laboratorios     - %d        |\n", *quantLabs);
        printf("\t| Quantidade Postos Trabalho  - %d        |\n", *quantPostosTrabalho);
        printf("\t| Quantidade Equipamentos     - %d        |\n", *quantEquipamentos);
        printf("\t| %% Equipamentos Avariados   - %.2f      |\n", *percEquipamentosAvariados);
        printf("\t-------------------------------------------\n");
        printf("\t| L - Gestao Laboratorio                  |\n");
        printf("\t| E - Gestao Equipamentos                 |\n");
        printf("\t| A - Gestao Avarias                      |\n");
        printf("\t| O - Listar Equipamentos Por Laboratorios|\n");
        printf("\t| S - Estatisticas                        |\n");
        printf("\t| V - Ver Logs                            |\n");
        printf("\t| F - Sair                                |\n");
        printf("\t-------------------------------------------\n");
        printf("\t| Selecione opcao:                        |\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);
        switch(opcao){
        case 'L':
            f_menu_laboratorio(v_lab, quantLabs, quantEquipamentos);
            break;
        case 'E':
            f_menu_equipamentos();
            break;
        case 'A':
            f_menu_avarias();
            break;
        case 'O':
            break;
        case 'S':
            f_menu_estatisticas();
            break;
        case 'V':
            break;
        case 'F':
            break;
        default:
            printf("Opcao invalida. Insira uma opcao valida\n");
        }
    }while(opcao != 'F');
}

void f_menu_laboratorio(s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos){
    char opcao;
    do{
        printf("\t-------------------------------------------\n");
        printf("\t| I - Inserir Laboratorio                 |\n");
        printf("\t| L - Listar Laboratorios                 |\n");
        printf("\t| C - Consultar Laboratorio               |\n");
        printf("\t| A - Alterar Laboratorio                 |\n");
        printf("\t| V - Voltar                              |\n");
        printf("\t-------------------------------------------\n");
        printf("\t| Selecione opcao:                        |\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);

        switch(opcao){
        case 'I':
            f_inserir_laboratorio(v_lab, quantLabs, quantEquipamentos);
            break;
        case 'L':
            break;
        case 'C':
            f_consultar_laboratorio (s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos);
            break;
        case 'A':
            break;
        case 'V':
            break;
        default:
            printf("Opcao invalida. Insira uma opcao valida\n");
        }
    }while(opcao != 'V');



}

void f_menu_equipamentos(){
    char opcao;
    do{
        printf("\t-------------------------------------------\n");
        printf("\t| L - Listar Equipamentos                 |\n");
        printf("\t| C - Consultar Equipamentos              |\n");
        printf("\t| I - Inventariacao                       |\n");
        printf("\t| N - Instalacao Equipamentos             |\n");
        printf("\t| A - Avaria Equipamento                  |\n");
        printf("\t| R - Reparacao Equipamento               |\n");
        printf("\t| B - Abate Equipamento                   |\n");
        printf("\t| V - Voltar                              |\n");
        printf("\t-------------------------------------------\n");
        printf("\t| Selecione opcao:                        |\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);

        switch(opcao){
        case 'L':
            break;
        case 'C':
            break;
        case 'I':
            break;
        case 'N':
            break;
        case 'A':
            break;
        case 'R':
            break;
        case 'B':
            break;
        case 'V':
            break;
        default:
            printf("Opcao invalida. Insira uma opcao valida\n");
        }
    }while(opcao != 'V');
}

void f_menu_avarias(){
    char opcao;
    do{
        printf("\t-------------------------------------------\n");
        printf("\t| L - Listar Avarias                      |\n");
        printf("\t| C - Consultar Avarias                   |\n");
        printf("\t| E - Eliminar Avaria                     |\n");
        printf("\t| V - Voltar                              |\n");
        printf("\t-------------------------------------------\n");
        printf("\t| Selecione opcao:                        |\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);

        switch(opcao){
        case 'L':
            break;
        case 'C':
            break;
        case 'E':
            break;
        case 'V':
            break;
        default:
            printf("Opcao invalida. Insira uma opcao valida\n");
        }
    }while(opcao != 'V');
}

void f_menu_estatisticas(){
    char opcao;
    do{
        printf("\t---------------------------------------------------\n");
        printf("\t| C - Custo Medio Cada Equipamento                |\n");
        printf("\t| P - %% Equipamentos Abatidos                    |\n");
        printf("\t| Q - Quantidade Equipamentos Avariados por data   |\n");
        printf("\t| D - Dados das reparacoes com maior custo        |\n");
        printf("\t| M - Menor %% De Equipamentos Disponiveis Por Lab|\n");
        printf("\t| V - Voltar                                      |\n");
        printf("\t---------------------------------------------------\n");
        printf("\t| Selecione opcao:                        |\n");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);

        switch(opcao){
        case 'C':
            break;
        case 'P':
            break;
        case 'Q':
            break;
        case 'D':
            break;
        case 'M':
            break;
        case 'V':
            break;
        default:
            printf("Opcao invalida. Insira uma opcao valida\n");
        }
    }while(opcao != 'V');

}

//FIM DOS MENUS INICIAIS

//2 INICIO GESTAO DOS LABORATORIOS

//2.1 Inserir Laboratorios

void f_inserir_laboratorio(s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos){
    int i, laboratorio_id = 0, confirmacao_dados[3]; //confirmacao de dados sera usado para guardar os dados do laboratorio para confirmar mais tarde
    //True = Erro | False = Everything cool
    bool ver = FALSE;
    char localizacao_lab[MAX_STRING], opcao;

    //Iniciar o confirmacao_dados = 0
    for(i=0; i < 4; i++){
        confirmacao_dados[i] = 0;
    }


    do{
         if(*quantLabs < MAX_LABORATORIOS){
             //***********************
             //                      *
             //   ID LABORATORIO     *
             //                      *
             //***********************
             do{
                confirmacao_dados[0] = lerInteiro(MIN_LAB_EQUI, MAX_LABORATORIOS, "Insira o ID do laboratorio:\n");
                for(i=0;i < MAX_LABORATORIOS;i++){
                    //Verificar se o laboratorio ja foi adicionado
                    //Se nao foi adicionado o laboratio entao é adicionado ao index disponivel
                    if(confirmacao_dados[0] == v_lab[i].id){
                        printf("Ha um laboratorio com o mesmo ID!\n");
                        //Se existir um id ja existente ver (verificacao) passa a TRUE
                        //para ativar e dizer que há ID igual
                        ver = TRUE;
                        break;
                    }else{
                        ver = FALSE;
                    }
                }
             }while(ver == TRUE);

             //**************************
             //                         *
             //     LOCAL LABORATORIO   *
             //                         *
             //**************************
             do{
                lerString("Insira o local do laboratorio:\n", localizacao_lab, MAX_STRING);
                for(i=0; i<MAX_LABORATORIOS; i++){
                    if(strcmp(localizacao_lab, v_lab[i].localizacao) == 0){
                        do{
                            //Verifica se o mesmo local do laboratorio ja foi inserido antes
                            //Se nao for inserido ver = FALSE se ja foi inserido entao ver = TRUE
                            printf("Ja existe um laboratorio com esta localizacao\n");
                            printf("Tem certeza que desja continuar? (S)im | (N)ao\n");
                            scanf(" %c", &opcao);
                            opcao = toupper(opcao);
                            switch(opcao){
                                case 'S':
                                    ver = FALSE;
                                    break;
                                case 'N':
                                    limparBuffer();
                                    lerString("Insira o local do laboratorio:\n", localizacao_lab, MAX_STRING);
                                    opcao = 'S';
                                    break;
                                default:
                                    printf("Opcao invalida. Insira uma opcao valida por favor!\n");
                            }
                        }while(opcao == 'N');
                    }

                }
             }while(ver == TRUE);


             //**************************
             //                         *
             //     POSTOS TRABALHOS    *
             //                         *
             //**************************
             confirmacao_dados[1] = lerInteiro(MIN_LAB_EQUI, MAX_EQUIPAMENTOS, "Insira o numero de postos de trabalho no laboratorio\n");
             /* Quais as verificacoes que tem de ser feitas aqui? */


             //**************************
             //                         *
             // EQUIPAMENTOS INSTALADOS *
             //                         *
             //**************************
             confirmacao_dados[2] = lerInteiro(MIN_LAB_EQUI, MAX_EQUIPAMENTOS, "Insira a quantidade de equipamentos instalados: \n"));
            /* Quais as verificacoes que tem de ser feitas aqui? */



             //**************************
             //                         *
             //        CONFIRMACAO      *
             //                         *
             //**************************
             printf("***Confirmacao dos dados***\n");
             printf("ID Laboratorio: %d\n", confirmacao_dados[0]);
             printf("Localizacao Laboratorio: %s\n", localizacao_lab);
             printf("Postos de trabalho: %d\n", confirmacao_dados[1]);
             printf("Equipamentos Instalados: %d\n", confirmacao_dados[2]);
             printf("Deseja confirmar? (S)im (N)ao\n");
             scanf(" %c", &opcao);
             opcao = toupper(opcao);
             switch(opcao){
                case 'S':
                    ver = FALSE;
                    break;
                case 'N':
                    ver = TRUE;
                    break;
                default:
                    printf("Insira uma opcao correcta. (S)im ou (N)ao\n");
             }


        }else{
            printf("Nao ha espaco para novos laboratorio!\n");
            printf("Codigo de erro: AL:%d", __LINE__);

        }


    }while(ver == TRUE);

    //Se adicionar ao vector da estrutura correr bem entao ...
     if(v_lab[*quantLabs].id = confirmacao_dados[0]) {
        if(strcpy(v_lab[*quantLabs].localizacao, localizacao_lab)){
            if(v_lab[*quantLabs].postos_trabalhos = confirmacao_dados[1]){
                if(v_lab[*quantLabs].q_equip_instalados = confirmacao_dados[2]){
                    printf("****Dados adicionado com sucesso!****\n");
                    //Incrementa o numero de quantidade de laboratorios ja adicionados
                    *quantLabs = *quantLabs + 1;
                }else{
                    printf("Erro ao adicionar equipamentos instalados!\n");
                    printf("Codigo de erro: AL:%d", __LINE__);
                }
            }else{
                printf("Erro ao adicionar postos de trabalho!\n");
                printf("Codigo de erro: AL:%d", __LINE__);
            }
        }else{
            printf("Erro ao adicionar localizacao!\n");
            printf("Codigo de erro: AL:%d", __LINE__);
        }
     }else{
        printf("Erro ao adicionar ID!\n");
        printf("Codigo de erro: AL:%d", __LINE__);
     }



}

void f_consultar_laboratorio (s_lab v_lab[MAX_LABORATORIOS], int *quantLabs, int *quantEquipamentos){


}


//FUNÇÕES AUXILIARES

void lerString(char msg[MAX_STRING], char str[MAX_STRING], int tamanho)
{
    int tamTexto;

    do
    {
        printf("%s", msg);
        fgets(str, tamanho, stdin);

        tamTexto = strlen(str);

        if (tamTexto <= 1)
        {
            printf("\n\nERRO: tem de inserir um valor.");
        }
        else
        {
            if (str[tamTexto-1] == '\n')
            {
                str[tamTexto-1] = '\0';
            }
            else
            {
                limparBuffer();
            }
        }
    } while (tamTexto <= 1);
}


int lerInteiro (int limMin, int limMax, char msg[MAX_STRING])
{
    int num, res;


    do
    {
        printf("%s", msg);
        res = scanf("%d", &num);
        limparBuffer();

        if (res != 1)
        {
            printf("\n\nERRO: o valor inserido tem que ser um numero inteiro.");
        }
        else
        {
            if (num < limMin || num > limMax)
            {
                printf("\n\nERRO: o numero tem de pertencer ao intervalo [%d, %d].", limMin, limMax);
            }
        }

    } while (num < limMin || num > limMax || res != 1);

    return num;
}


float lerFloat (float limMin, float limMax, char msg[MAX_STRING])
{
    float num;
    int res;


    do
    {
        printf("%s", msg);
        res = scanf("%f", &num);
        limparBuffer();

        if (res != 1)
        {
            printf("\n\nERRO: o valor inserido tem que ser um numero inteiro.");
        }
        else
        {
            if (num < limMin || num > limMax)
            {
                printf("\n\nERRO: o numero tem de pertencer ao intervalo [%.2f, %.2f].", limMin, limMax);
            }
        }

    } while (num < limMin || num > limMax || res != 1);

    return num;
}


void limparBuffer (void)
{
    char lixo;

    do
    {
        lixo = getchar();

    } while (lixo != '\n' && lixo != EOF);
}


//Falta fazer os ficheiros e nao se esquecer de passar os vetores por parametros

//Fazer a validação do scanf. Se scanf = 1 então erro
