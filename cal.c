/**********************************************
*** Programa .......: Cal.c                  ***
*** Autor ..........: Wesley Costa           ***
*** Prontuário......: 1510797                ***
*** Data ...........: Junho de 2016          ***
************************************************/
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include "stdgen.h"
#include "calendar.h"
#define  tString 128
#define  tColuna 7
#define  tLinha 8

typedef struct _sMes
{
    char Matriz[8][tColuna + 2][tString];
} sMes;

char *cabecalho = "D  S  T  Q  Q  S  S  ";
int i, j;

char *ToString(int num)
{
    static char str[tString];
    static char numStr[tString];
    strcpy(str, "");

    if(num < 10)
        strcpy(str , " ");

    sprintf(numStr, "%d", num);
    strcat(str, numStr);

    return str;
}

char *NewString(char str[], int num)
{
    static char strNew[tString];
    int i = 0;
    strcpy(strNew, "");

    for(i = 0; i < num; i++)
        strcat(strNew, str);

    return strNew;
}

 int GetAnoAtual()
 {
     time_t rawtime;
     struct tm * timeinfo;

     time (&rawtime);
     timeinfo = localtime(&rawtime);

     return timeinfo->tm_year + 1900;
 }

 int GetMesAtual()
 {
     time_t rawtime;
     struct tm * timeinfo;

     time (&rawtime);
     timeinfo = localtime (&rawtime);

     return timeinfo->tm_mon + 1;
 }

char *GetDate(int dia, int mes, int ano)
{
    static char data[tString];
    sprintf(data, "%02.d/%02.d/%04.d", dia, mes, ano);

    return data;
}

char *GetMes(int mes)
{
    switch (mes)
     {
        case  1: return "Janeiro";
        case  2: return "Fevereiro";
        case  3: return "Março";
        case  4: return "Abril";
        case  5: return "Maio";
        case  6: return "Junho";
        case  7: return "Julho";
        case  8: return "Agosto";
        case  9: return "Setembro";
        case 10: return "Outubro";
        case 11: return "Novembro";
        case 12: return "Dezembro";
    }

    return NULL;
}

void SetCabecalho(sMes *smes, int mes)
{
  char *nomeMes = GetMes(mes);
  int tamanho = strlen(cabecalho) - strlen(nomeMes);

  strcpy(smes->Matriz[0][0], nomeMes);
  strcpy(smes->Matriz[1][0], cabecalho);
  strcpy(smes->Matriz[0][strlen(nomeMes) -1], NewString(" ", tamanho));
}

void LimparMes(sMes *smes)
{
    for(i =0; i <= tColuna; i++)
        for(j =0; j <=  tLinha; j++)
          strcpy(smes->Matriz[i][j], "");
}

void FormataPrimeiraLinha(sMes *smes, int diaSemana)
{
  for(i = 0; i <= diaSemana; i++)
    strcpy(smes->Matriz[2][i], NewString(" " , 3));
}

void FormataUltimaLinha(sMes *smes, int linha, int diaSemana)
{
    for(j = linha; j < tLinha; j++)
       for(i = diaSemana  % 7 ; i < tColuna; i++)
       {
            strcpy(smes->Matriz[j][i], NewString(" ", 3));
            diaSemana++;
       }
}

void PreencheMes(sMes *smes, int mes, int ano)
{
  int linha = 2;
  int dia = 1;

  int diaSemana = dayweek(GetDate(dia,mes, ano));

  SetCabecalho(smes, mes);
  FormataPrimeiraLinha(smes, diaSemana);

   for(dia = 1; dia <= lastday(GetDate(dia,mes, ano)); dia++)
    {
        sprintf(smes->Matriz[linha][diaSemana % 7],"%s%s",  ToString(dia), " ");

        if(diaSemana % 7 == 6)
            linha++;

        diaSemana++;
    }

    FormataUltimaLinha(smes, linha, diaSemana);
}

void ImprimeAnoNoCentro()
{
  printf("%s%d\n", NewString(" ", (80 - 4) / 2), ano);
}

void ImprimeAno(int ano)
{
    sMes meses[12];
    int mes = 0;

    for(mes = 0; mes < 12; mes++)
    {
        LimparMes(&meses[mes]);
        PreencheMes(&meses[mes], mes + 1, ano);
    }

    ImprimeAnoNoCentro();

    mes = 0;
    while(mes < 12)
    {
        for(i =0; i <= tColuna; i++)
        {
            printf("%s", NewString(" ", 6));

            for(j =0; j <=  tLinha; j++)
                printf("%s", meses[mes].Matriz[i][j]);

            printf("%s", NewString(" ", 3));
            for(j =0; j <=  tLinha; j++)
                printf("%s", meses[mes + 1].Matriz[i][j]);

            printf("%s", NewString(" ", 3));
            for(j =0; j <=  tLinha; j++)
                printf("%s", meses[mes + 2].Matriz[i][j]);

            printf("\n");
        }
        mes += 3;
    }
}

void ImprimeMes(int mes, int ano)
{
    sMes smes;

    LimparMes(&smes);
    PreencheMes(&smes, mes, ano);
    ImprimeAnoNoCentro(ano);

    for(i =0; i <= tColuna; i++)
    {
        printf("%s", NewString(" ", (80 - 22) / 2));

        for(j =0; j <=  tLinha; j++)
            printf("%s", smes.Matriz[i][j]);

        printf("\n");
    }
 }

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    if(argc > 3)
        printf("\n Entrada invalida... \n");
    else
    {
        if(argc == 1)
            ImprimeMes(GetMesAtual(), GetAnoAtual());
        else if(argc == 3)
        {
            int _mes = atoi(argv[1]);
            int _ano = atoi(argv[2]);

            if(_mes < 1 || _mes > 12 || _ano < 0 || _ano > 9999)
                printf("\n Entrada invalida... \n");
            else
                ImprimeMes(_mes, _ano);
        }
        else
        {
            int _ano = atoi(argv[1]);

            if( _ano > 9999 || _ano < 0)
                printf("\n Entrada invalida... \n");
            else
                ImprimeAno(_ano);
        }
     }

   return 0;
}
