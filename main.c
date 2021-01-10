#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct funcionario
{
    int codigo;
    int cargo;//1->empregado 2-> gerente 3-> patrao
    int telefone;
    int filhos;
    char nome[20];
    char estado[20];
    char tempo_entrada[20];
    char tempo_saida[20];
    float salario;
    float subsidio;
    char data_nascimento[20];
    int eliminado; //0-> esta no sistema, 1-> esta eliminado
}funcionario;



//recebe dois argumentos,  o nome do ficheiro e um apontador pata o size_t, onde o numero de funcionarios é guardado

funcionario *ler_funcionarios(const char *filename, size_t *len){

    //verificar parametros
    if(filename==NULL || len==NULL)
        return NULL;
    
    //abrir o ficheiro em modo de leitura
    FILE *fp = fopen(filename, "r");
    //Verificar o ficheiro
    if(fp==NULL){
        fprintf(stderr,"Could not open %s: %s\n",filename,strerror(errno));
        return NULL;
    }

    //inicializar 2 arrays de funcionarios 
    funcionario *arrFuncs = NULL, *tmp;
    //contagem dos funcionarios começa a zero
    *len = 0;

    //definiçao de linha assumindo que nenhuma sera maior que 1023 caracteres
    char line[1024];

    while(fgets(line, sizeof line, fp)){
        //aloca espaço no tmp para receber mais um funcionario
        tmp = realloc(arrFuncs, (*len + 1) * sizeof *arrFuncs);
        if(tmp == NULL){
            fprintf(stderr, "could not parse the whole file %s\n", filename);

            // retornar todos os funcionarios lidos ate ao momento
            if(*len == 0){
                free(arrFuncs);
                arrFuncs = NULL;
            }
            return arrFuncs;
        }

        arrFuncs = tmp;

        if(sscanf(line,"%d,%d,%d,%d,%19[^,],%19[^,],%19[^,],%19[^,],%f,%f,%19[^,],%d", &(arrFuncs[*len].codigo), &(arrFuncs[*len].cargo), &(arrFuncs[*len].telefone), &(arrFuncs[*len].filhos), (arrFuncs[*len].nome), (arrFuncs[*len].estado), &(arrFuncs[*len].tempo_entrada), &(arrFuncs[*len].tempo_saida),&(arrFuncs[*len].salario),&(arrFuncs[*len].subsidio),&(arrFuncs[*len].data_nascimento),&(arrFuncs[*len].eliminado))!=12)
        {
            //se o formato da linha for invalido passa a frente e decrementa na contagem de lihas
            fprintf(stderr, "Invalid line format (skipping line): %s\n", line);
            (*len)--;
            continue;
        }
        //se estiver tudo bem incrementa
        (*len)++;
    }

    fclose(fp);
    //se o aarray estiver vazio ou todas as linhas tem o formato errado
    if(*len == 0){
        free(arrFuncs);
        arrFuncs = NULL;
    }

    return arrFuncs;
}


//recebe a lista dos funcionarios, o tamanho dessa lista o o numero do funcionario que queremos encontrar
//retorna o indice onde essse funcionario esta gurdardo
int procura_funcionario_por_numero(funcionario *funcionarios,size_t len, int num){
    if(funcionarios == NULL)
        return -1;

     for(size_t i = 0; i < len; ++i)
        if(num == funcionarios[i].codigo )
            return i;
    // not found
    return -1;
}

//Escreve a lista dos funcionarios editada de volta no ficheiro
void imprime_funcionarios(funcionario *funcionarios, size_t len, FILE *fp){
     //verificar parametros
    if(funcionarios==NULL || fp==NULL)
        return;
    for(size_t i = 0; i<len; ++i){
        fprintf(fp,"%d,%d,%d,%d,%s,%s,%s,%s,%f,%f,%s,%d\n", (funcionarios[i].codigo), (funcionarios[i].cargo), (funcionarios[i].telefone), (funcionarios[i].filhos), (funcionarios[i].nome), (funcionarios[i].estado), (funcionarios[i].tempo_entrada), (funcionarios[i].tempo_saida), (funcionarios[i].salario), (funcionarios[i].subsidio),(funcionarios[i].data_nascimento), (funcionarios[i].eliminado));
    }
}



//Adiciona um novo funcionario ao final do ficheiro
void adiciona_funcionario(funcionario *funcionarios, size_t len){

        FILE *fadd = fopen("./funcionarios.csv", "a");
        if(fadd==NULL){
            fprintf(stderr, "Nao foi possivel abrir o ficheiro para escrita: %s\n",strerror(errno));
        } 

        funcionario funci;
        //verificação se ja ha algum funcionarion com o mesmo codigo
        int i = 0;
        do{
            printf("Introduza o codigo do funcionario: ");
            scanf("%d",&(funci.codigo));  
            if(procura_funcionario_por_numero(funcionarios,len,funci.codigo)!=-1){
                printf("Codigo ja utilizado, introduza outro!!!\n");
            }else{i=1;}
        }while(i==0);
        
        do{
            printf("Introduza o cargo do funcionario(1-> empregado, 2-> gerente, 3-> patrao): ");
            scanf("%d",&(funci.cargo));
            if(funci.cargo<1 || funci.cargo>3){
                printf("Valor invalido!!\n");
            }
        }while(funci.cargo<1 || funci.cargo>3);
        
        printf("Introduza o numero de telefone do funcionario: ");
        scanf("%d",&(funci.telefone));
        printf("Introduza o numero de filhos do funcionario: ");
        scanf("%d",&(funci.filhos));
        printf("Introduza o nome do funcionario: ");
        scanf("%s",&(funci.nome));
        printf("Introduza o estado civil do funcionario: ");
        scanf("%s",&(funci.estado));
        printf("Introduza o tempo de entrada na empresa do funcionario: ");
        scanf("%s",&(funci.tempo_entrada));
        printf("Introduza o tempo de saida na empresa do funcionario: ");
        scanf("%s",&(funci.tempo_saida));
        printf("Introduza o salario do funcionario: ");
        scanf("%f",&(funci.salario));
        printf("Introduza o subsidio de alimentacao do funcionario: ");
        scanf("%f",&(funci.subsidio));
        printf("Introduza a data de nascimento do funcionario: ");
        scanf("%s",&(funci.data_nascimento));
        funci.eliminado=0;


    if(fadd==NULL)
        return;
    fprintf(fadd,"%d,%d,%d,%d,%s,%s,%s,%s,%f,%f,%s,%d\n", (funci.codigo), (funci.cargo), (funci.telefone), (funci.filhos), (funci.nome), (funci.estado), (funci.tempo_entrada), (funci.tempo_saida), (funci.salario), (funci.subsidio),(funci.data_nascimento), (funci.eliminado));


    fclose(fadd);
    printf("Funcionario adicionado com sucesso!\n");
}



//Imprime as informaçoes do funcionario pretendido
void consulta_funcionario(funcionario *funcionarios,size_t len){
    int funcNum;
    printf("Introduza o numero do funcionario do qual pretende obter as informacoes: ");
    scanf("%d",&funcNum);
    int funcIndex = procura_funcionario_por_numero(funcionarios, len, funcNum);
    if(funcIndex == -1){
        printf("Nao existe nenhum funcinario com esse numero!");
        return;
    }else{
        funcionario func = funcionarios[funcIndex];
        printf("Informação do funcionario: \n");
        printf("Codigo:                     %d;\n",func.codigo);
        printf("Nome:                       %s;\n",func.nome);
        if(func.cargo == 1){
            printf("Cargo:                      Empregado;\n");
        }else if(func.cargo == 2){
            printf("Cargo:                      Gerente;\n");
        }else if(func.cargo == 3){
            printf("Cargo:                      Patrao;\n");
        }
        printf("Estado Civil:               %s;\n",func.estado);
        printf("Numero de Filhos:           %d;\n",func.filhos);
        printf("Nr telefone:                %d;\n",func.telefone);
        printf("Data de entrada na empresa: %s;\n",func.tempo_entrada);
        printf("Data de saida na empresa:   %s;\n",func.tempo_saida);
        printf("Salario:                    %f;\n",func.salario);
        printf("subsidio de alimentacao :   %f;\n",func.subsidio);
        printf("Data de nascimento:         %s;\n",func.data_nascimento);
        if(func.eliminado == 1){
            printf("Estado do funcionario:      Eliminado;\n"); 
        }else{
            printf("Estado do funcionario:      No Sistema;\n"); 
        }
        


    }


}
void edita_funcionario(funcionario *funcionarios,size_t len){
    int funcNum;
    printf("Introduza o numero do funcionario que pretende editar: ");
    scanf("%d",&funcNum);
    int funcIndex = procura_funcionario_por_numero(funcionarios, len, funcNum);
    if(funcIndex == -1){
        printf("Nao existe nenhum funcinario com esse numero!");
        return;
    }else{
        funcionario funci;
        do{
            printf("Introduza o cargo do funcionario(1-> empregado, 2-> gerente, 3-> patrao): ");
            scanf("%d",&(funci.cargo));
            if(funci.cargo<1 || funci.cargo>3){
                printf("Valor invalido!!\n");
            }
        }while(funci.cargo<1 || funci.cargo>3);
        printf("Introduza o novo numero de telefone do funcionario: ");
        scanf("%d",&(funci.telefone));
        printf("Introduza o novo numero de filhos do funcionario: ");
        scanf("%d",&(funci.filhos));
        printf("Introduza o novo nome do funcionario: ");
        scanf("%s",&(funci.nome));
        printf("Introduza o novo estado civil do funcionario: ");
        scanf("%s",&(funci.estado));
        printf("Introduza o novo tempo de entrada na empresa do funcionario: ");
        scanf("%s",&(funci.tempo_entrada));
        printf("Introduza o novo tempo de saida na empresa do funcionario: ");
        scanf("%s",&(funci.tempo_saida));
        printf("Introduza o novo salario do funcionario: ");
        scanf("%f",&(funci.salario));
        printf("Introduza o novo subsidio de alimentacao do funcionario: ");
        scanf("%f",&(funci.subsidio));

        funcionarios[funcIndex].cargo = funci.cargo;
        funcionarios[funcIndex].telefone = funci.telefone;    
        funcionarios[funcIndex].filhos = funci.filhos;
        strcpy(funcionarios[funcIndex].nome, funci.nome);
        strcpy(funcionarios[funcIndex].estado, funci.estado);
        strcpy(funcionarios[funcIndex].tempo_entrada, funci.tempo_entrada);
        strcpy(funcionarios[funcIndex].tempo_saida, funci.tempo_saida);
        funcionarios[funcIndex].salario = funci.salario;
        funcionarios[funcIndex].subsidio = funci.subsidio;

        FILE *fp = fopen("./funcionarios.csv", "w");
        if(fp==NULL){
            fprintf(stderr, "Nao foi possivel abrir o ficheiro para escrita: %s\n",
                    strerror(errno));

            free(funcionarios);
            return;
        } 

        
        imprime_funcionarios(funcionarios, len, fp);
        fclose(fp);
        free(funcionarios);

    }
}




void elimina_funcionario(funcionario *funcionarios,size_t len){
int funcNum;
    printf("Introduza o numero do funcionario que pretende eliminar: ");
    scanf("%d",&funcNum);
    int funcIndex = procura_funcionario_por_numero(funcionarios, len, funcNum);
    if(funcIndex == -1){
        printf("Nao existe nenhum funcinario com esse numero!");
        return;
    }else{
        funcionarios[funcIndex].eliminado = 1;

        FILE *fp = fopen("./funcionarios.csv", "w");
        if(fp==NULL){
            fprintf(stderr, "Nao foi possivel abrir o ficheiro para escrita: %s\n",
                    strerror(errno));

            free(funcionarios);
            return;
        } 

        
        imprime_funcionarios(funcionarios, len, fp);
        fclose(fp);
        free(funcionarios);
    }
}






int main(void){

    size_t funcionarios_len;
    funcionario *funcionarios = ler_funcionarios("./funcionarios.csv", &funcionarios_len);

    if(funcionarios == NULL){
            return 1;
        }

    //PRINTS PARA A INTERFACE
    printf("Qual a operação a executar?\n");
    printf("1-> Adicionar funcionario; \n2-> Consultar informação de funcionario\n3-> Editar Funcionario\n4-> Eliminar Funcionario\n");
    int opcao;
    scanf("%d",&opcao);

    if(opcao == 1){
        adiciona_funcionario(funcionarios, funcionarios_len);
    }
    else if(opcao == 2){
        consulta_funcionario(funcionarios, funcionarios_len);
    }
    else if(opcao == 3){
        edita_funcionario(funcionarios, funcionarios_len);
    }else if(opcao == 4){
        elimina_funcionario(funcionarios, funcionarios_len);
    }
    else{
        printf("Opçao invalida");
        //return 0;
    }


    return 0;
}