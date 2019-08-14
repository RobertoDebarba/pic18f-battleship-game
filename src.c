struct Parte_Navio
{
     short lin;
     short col;
     short acertou;
};

struct
{
     struct Parte_Navio partes[5];
} Encouracados[2];

struct
{
     struct Parte_Navio partes[6];
} Porta_Aviao;

struct
{
     struct Parte_Navio partes[3];
} Hidroavioes[5];

struct
{
     struct Parte_Navio partes[1];
} Submarinos[4];

struct
{
     struct Parte_Navio partes[2];
} Cruzadores[3];

unsigned char mapa[15][15];
unsigned char mapa_Posicoes[15][15];
int semente = 0;
unsigned char linha_Index[15];
unsigned char numero_Index[10];
int navios_vivos = 15;
int municao = 10;

// Interrupção
void InitTimer0(){
  T0CON         = 0x80;
  TMR0H         = 0x3C;
  TMR0L         = 0xB0;
  GIE_bit         = 1;
  TMR0IE_bit         = 1;
}

void Interrupt(){
  if (TMR0IF_bit){
    TMR0IF_bit = 0;
    TMR0H         = 0x3C;
    TMR0L         = 0xB0;

    //Enter your code here
    semente++;
  }
}


char enter[] = {13,0};

// Utils

int RandU(int min, int max){
    return (rand() % (max - min)) + min;
}

void Escreve(unsigned char *letra){
     UART1_write_Text(letra);
}

void Escreve_Char(unsigned char letra){
     UART1_write(letra);
}

void Emitir_Som(int index){
     int frequencia;
     if(index == 0){// Acertou agua;
          frequencia = 80;
     }else if(index == 1){// Derrubou Navio;
          frequencia = 200;
     }else if(index == 2){// Acertou Navio;
          frequencia = 400;
     }else if(index == 3){// perdeu;
          frequencia = 600;
     }else if(index == 4){// ganhou;
          frequencia = 700;
     }

     //TODO emitir som;
     SOUND_PLAY(frequencia, 200);
}

char* Retornar_Texto(){
     unsigned char output;
     UART1_Read_Text(output, enter, 10);
     return output;
     /*unsigned char output;
     unsigned char *valueString = 'hh';
     while(1){
       while (UART1_Data_Ready() == 1) {
             output = UART1_Read();
             valueString = strchr('', output);
             UART1_write(output);
             UART1_Write_Text(valueString);
       }
     }
     return valueString;
     */
}

int mapaVazio(int vertical, int horizontal){
    if(mapa_Posicoes[vertical][horizontal] == ' '){
        return 1;
    }
    return 0;
}


// Inicializações
void Inicializar_Globais(){
     Sound_Init(&PORTC, 5);
     InitTimer0();
     adcon1 = 0x0E;
     TRISD = 0b00000000;
     TRISB = 0b11111111;

     UART1_INIT(9600);
     Delay_ms(100);
}

void Inicializar_Variavel_Linha_Index(){
     linha_Index[0] = 'A';
     linha_Index[1] = 'B';
     linha_Index[2] = 'C';
     linha_Index[3] = 'D';
     linha_Index[4] = 'E';
     linha_Index[5] = 'F';
     linha_Index[6] = 'G';
     linha_Index[7] = 'H';
     linha_Index[8] = 'I';
     linha_Index[9] = 'J';
     linha_Index[10] = 'K';
     linha_Index[11] = 'L';
     linha_Index[12] = 'M';
     linha_Index[13] = 'N';
     linha_Index[14] = 'O';
}

void Inicializar_Variavel_Numeros_Index(){
     numero_Index[0] = '0';
     numero_Index[1] = '1';
     numero_Index[2] = '2';
     numero_Index[3] = '3';
     numero_Index[4] = '4';
     numero_Index[5] = '5';
     numero_Index[6] = '6';
     numero_Index[7] = '7';
     numero_Index[8] = '8';
     numero_Index[9] = '9';
}

void Inicializar_Parte(struct Parte_Navio parte){
     parte.lin = 255;
     parte.col = 255;
     parte.acertou = 0;
}

void Inicializar_Navios_Encouracados(){
     int i;
     int p;
     for(i=0; i < 2; i++){
          for(p=0; p < 5; p++){
                   Inicializar_Parte(Encouracados[i].partes[p]);
          }
     }
}

void Posicionar_Navios_Encouracados(){
     int vertical;
     int horizontal;
     int i;
     int p;
     int procurar = 1;
     for(i=0; i < 2; i++){
          procurar = 1;
          while(procurar){
               procurar = 0;
               vertical = RandU(0, 14);
               horizontal = RandU(0, 9);
               for(p = 0; p < 5; p++){
                    if(!mapaVazio(vertical, horizontal + p)){
                         procurar = 1;
                    }
               }
               if(!procurar){
                    for(p = 0; p < 5; p++){
                         mapa_Posicoes[vertical][horizontal+p] = 'E';
                         Encouracados[i].partes[p].lin = vertical;
                         Encouracados[i].partes[p].col = horizontal+p;
                    }
               }
          }
     }
}

void Inicializar_Navios_Porta_Aviao(){
     int p;
     for(p=0; p < 6; p++){
              Inicializar_Parte(Porta_Aviao.partes[p]);
     }
}

void Posicionar_Navios_Porta_Aviao(){
     int vertical;
     int horizontal;
     int p;
     int procurar = 1;
          while(procurar){
               procurar = 0;
               vertical = RandU(0, 14);
               horizontal = RandU(0, 8);
               for(p = 0; p < 6; p++){
                    if(!mapaVazio(vertical, horizontal + p)){
                         procurar = 1;
                    }
               }
               if(!procurar){
                    for(p = 0; p < 6; p++){
                         mapa_Posicoes[vertical][horizontal+p] = 'P';
                         Porta_Aviao.partes[p].lin = vertical;
                         Porta_Aviao.partes[p].col = horizontal+p;
                    }
               }
          }
}

void Inicializar_Navios_Hidroavioes(){
     int i;
     int p;
     for(i=0; i < 5; i++){
          for(p=0; p < 3; p++){
                   Inicializar_Parte(Hidroavioes[i].partes[p]);
          }
     }
}

void Posicionar_Navios_Hidroavioes(){
     int vertical;
     int horizontal;
     int i;
     int p;
     int procurar;
     for(i=0; i < 5; i++){
          procurar = 1;
          while(procurar){
               procurar = 0;
               vertical = RandU(1, 14);
               horizontal = RandU(0, 12);
               if(!mapaVazio(vertical  , horizontal  ) ||
                  !mapaVazio(vertical-1, horizontal+1) ||
                  !mapaVazio(vertical  , horizontal+2)){
                    procurar = 1;
               }
               if(!procurar){
                    mapa_Posicoes[vertical  ][horizontal  ] = 'H';
                    mapa_Posicoes[vertical-1][horizontal+1] = 'H';
                    mapa_Posicoes[vertical  ][horizontal+2] = 'H';
                    Hidroavioes[i].partes[0].lin = vertical;
                    Hidroavioes[i].partes[0].col = horizontal;
                    Hidroavioes[i].partes[1].lin = vertical-1;
                    Hidroavioes[i].partes[1].col = horizontal+1;
                    Hidroavioes[i].partes[2].lin = vertical;
                    Hidroavioes[i].partes[2].col = horizontal+2;
               }
          }
     }
}

void Inicializar_Navios_Submarinos(){
     int i;
     int p;
     for(i=0; i < 4; i++){
          for(p=0; p < 1; p++){
                   Inicializar_Parte(Submarinos[i].partes[p]);
          }
     }
}

void Posicionar_Navios_Submarinos(){
     int vertical;
     int horizontal;
     int i;
     int p;
     int procurar = 1;
     for(i=0; i < 4; i++){
          procurar = 1;
          while(procurar){
               procurar = 0;
               vertical = RandU(0, 14);
               horizontal = RandU(0, 14);
               if(!mapaVazio(vertical, horizontal)){
                    procurar = 1;
               }
               if(!procurar){
                    mapa_Posicoes[vertical][horizontal] = 'S';
                    Submarinos[i].partes[0].lin = vertical;
                    Submarinos[i].partes[0].col = horizontal;
               }
          }
     }
}

void Inicializar_Navios_Cruzadores(){
     int i;
     int p;
     for(i=0; i < 3; i++){
          for(p=0; p < 2; p++){
                   Inicializar_Parte(Cruzadores[i].partes[p]);
          }
     }
}

void Posicionar_Navios_Cruzadores(){
     int vertical;
     int horizontal;
     int i;
     int p;
     int procurar = 1;
     for(i=0; i < 3; i++){
          procurar = 1;
          while(procurar){
               procurar = 0;
               vertical = RandU(0, 14);
               horizontal = RandU(0, 13);
               for(p = 0; p < 2; p++){
                    if(!mapaVazio(vertical, horizontal + p)){
                         procurar = 1;
                    }
               }
               if(!procurar){
                    for(p = 0; p < 2; p++){
                         mapa_Posicoes[vertical][horizontal+p] = 'C';
                         Cruzadores[i].partes[p].lin = vertical;
                         Cruzadores[i].partes[p].col = horizontal+p;
                    }
               }
          }
     }
}

void Inicializar_Posicoes(){
     int i;
     int j;
     for(i = 0; i < 15; i++){
           for(j = 0; j <15; j++){
                 mapa_Posicoes[i][j] = ' ';
           }
     }
}

void Inicializar_Mapa(){
     int i;
     int j;
     for(i = 0; i < 15; i++){
           for(j = 0; j <15; j++){
                 mapa[i][j] = '~';
           }
     }
}

void Inicializar_Navios(){
     Inicializar_Navios_Encouracados();
     Inicializar_Navios_Porta_Aviao();
     Inicializar_Navios_Hidroavioes();
     Inicializar_Navios_Submarinos();
     Inicializar_Navios_Cruzadores();

     Inicializar_Posicoes();

     Posicionar_Navios_Encouracados();
     Posicionar_Navios_Porta_Aviao();
     Posicionar_Navios_Hidroavioes();
     Posicionar_Navios_Submarinos();
     Posicionar_Navios_Cruzadores();
}

/* Encouraçados */
int Retorna_Index_Encouracados(int linha, int coluna){
    int i, p;
    for(i = 0; i < 2; i++){
        for(p = 0; p < 5; p++){
            if(Encouracados[i].partes[p].lin == linha &&
               Encouracados[i].partes[p].col == coluna){
                Encouracados[i].partes[p].Acertou = 1;
                mapa[linha][coluna] = 'E';
                return i;
            }
        }
    }
}

int Encouracados_Derrubado(int index){
    int p;
    for(p = 0; p < 5; p++){
        if(Encouracados[index].partes[p].Acertou == 0){
            return 0;
        }
    }
    return 1;
}

/* Porta_Aviao */
void Retorna_Index_Porta_Aviao(int linha, int coluna){
    int i, p;
	for(p = 0; p < 6; p++){
		if(Porta_Aviao.partes[p].lin == linha &&
		   Porta_Aviao.partes[p].col == coluna){
			Porta_Aviao.partes[p].Acertou = 1;
			mapa[linha][coluna] = 'P';
		}
	}
}

int Porta_Aviao_Derrubado(){
    int p;
    for(p = 0; p < 6; p++){
        if(Porta_Aviao.partes[p].Acertou == 0){
            return 0;
        }
    }
    return 1;
}

/* Hidroavioes */
int Retorna_Index_Hidroavioes(int linha, int coluna){
    int i, p;
    for(i = 0; i < 5; i++){
        for(p = 0; p < 3; p++){
            if(Hidroavioes[i].partes[p].lin == linha &&
               Hidroavioes[i].partes[p].col == coluna){
                Hidroavioes[i].partes[p].Acertou = 1;
                mapa[linha][coluna] = 'H';
                return i;
            }
        }
    }
}

int Hidroavioes_Derrubado(int index){
    int p;
    for(p = 0; p < 3; p++){
        if(Hidroavioes[index].partes[p].Acertou == 0){
            return 0;
        }
    }
    return 1;
}

/* Submarinos */
int Retorna_Index_Submarinos(int linha, int coluna){
    int i, p;
    for(i = 0; i < 4; i++){
        for(p = 0; p < 1; p++){
            if(Submarinos[i].partes[p].lin == linha &&
               Submarinos[i].partes[p].col == coluna){
                Submarinos[i].partes[p].Acertou = 1;
                mapa[linha][coluna] = 'S';
                return i;
            }
        }
    }
}

int Submarinos_Derrubado(int index){
    int p;
    for(p = 0; p < 1; p++){
        if(Submarinos[index].partes[p].Acertou == 0){
            return 0;
        }
    }
    return 1;
}

/* Cruzadores */
int Retorna_Index_Cruzadores(int linha, int coluna){
    int i, p;
    for(i = 0; i < 3; i++){
        for(p = 0; p < 2; p++){
            if(Cruzadores[i].partes[p].lin == linha &&
               Cruzadores[i].partes[p].col == coluna){
                Cruzadores[i].partes[p].Acertou = 1;
                mapa[linha][coluna] = 'C';
                return i;
            }
        }
    }
}

int Cruzadores_Derrubado(int index){
    int p;
    for(p = 0; p < 2; p++){
        if(Cruzadores[index].partes[p].Acertou == 0){
            return 0;
        }
    }
    return 1;
}

void Desenhar_Mapa(){
     int i;
     int j;
     Escreve(" x  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 \r");
     for(i = 0; i < 15; i++){
           Escreve_Char(' ');
           Escreve_Char(linha_Index[i]);
           Escreve_Char(' ');
           for(j = 0; j <15; j++){
                 Escreve_Char(' ');
                 Escreve_Char(mapa[i][j]);
                 Escreve_Char(' ');
           }
           Escreve_Char('\r');
     }
}

void Desenhar_Mapa_Aberto(){
     int i;
     int j;
     Escreve(" x  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 \r");
     for(i = 0; i < 15; i++){
           Escreve_Char(' ');
           Escreve_Char(linha_Index[i]);
           Escreve_Char(' ');
           for(j = 0; j <15; j++){
                 Escreve_Char(' ');
                 Escreve_Char(mapa_Posicoes[i][j]);
                 Escreve_Char(' ');
           }
           Escreve_Char('\r');
     }
     Delay_ms(2000);
}

int posicaoValida(unsigned char *tecla){
    int i, linhaValida, colunaValida;
    if(strlen(tecla) == 2 || strlen(tecla) == 3){
        linhaValida = 0;
        for(i = 0; i < 15; i++){
            if(tecla[0] == linha_Index[i]){
                linhaValida = 1;
            }
        }

        colunaValida = 0;
        for(i = 0; i < 10; i++){
            if(tecla[1] == numero_Index[i]){
                colunaValida = 1;
            }
        }
        if(strlen(tecla) == 3){
            colunaValida = 0;
            for(i = 0; i < 10; i++){
                if(tecla[2] == numero_Index[i]){
                    colunaValida = 1;
                }
            }
        }

        if(colunaValida && linhaValida){
            return 1;
        }
    }
    return 0;
}

int Extrair_Index_Linha(unsigned char *tecla){
    int i;
    for(i = 0; i < 15; i++){
        if(tecla[0] == linha_Index[i]){
            return i;
        }
    }
    return -1;
}

int Extrair_Index_Coluna(unsigned char *tecla){
    int i;
    int retorno = -1;
    for(i = 0; i < 10; i++){
        if(tecla[1] == numero_Index[i]){
            retorno = i;
        }
    }
    if(strlen(tecla) == 3){
        retorno = retorno *10;
        for(i = 0; i < 10; i++){
            if(tecla[2] == numero_Index[i]){
                retorno = retorno + i;
            }
        }
    }

    return retorno-1;
}

void Atirar_Posicao(int linha, int coluna){
     int index_navio;
     if(mapa[linha][coluna] == '~'){
          if(mapa_Posicoes[linha][coluna] == ' '){
               municao--;
               mapa[linha][coluna] = '.';
               Emitir_Som(0);
          }else if(mapa_Posicoes[linha][coluna] == 'E'){
               municao++;
               index_navio = Retorna_Index_Encouracados(linha, coluna);
               if(Encouracados_Derrubado(index_navio)){
                    municao = municao +3;
                    navios_vivos--;
                    Emitir_Som(1);
               }else{
                    Emitir_Som(2);
               }
          }else if(mapa_Posicoes[linha][coluna] == 'P'){
               municao++;
               Retorna_Index_Porta_Aviao(linha, coluna);
               if(Porta_Aviao_Derrubado()){
                    municao = municao +3;
                    navios_vivos--;
                    Emitir_Som(1);
               }else{
                    Emitir_Som(2);
               }
          }else if(mapa_Posicoes[linha][coluna] == 'H'){
               municao++;
               index_navio = Retorna_Index_Hidroavioes(linha, coluna);
               if(Hidroavioes_Derrubado(index_navio)){
                    municao = municao +3;
                    navios_vivos--;
                    Emitir_Som(1);
               }else{
                    Emitir_Som(2);
               }
          }else if(mapa_Posicoes[linha][coluna] == 'S'){
               municao++;
               index_navio = Retorna_Index_Submarinos(linha, coluna);
               if(Submarinos_Derrubado(index_navio)){
                    municao = municao +3;
                    navios_vivos--;
                    Emitir_Som(1);
               }else{
                    Emitir_Som(2);
               }
          }else if(mapa_Posicoes[linha][coluna] == 'C'){
               municao++;
               index_navio = Retorna_Index_Cruzadores(linha, coluna);
               if(Cruzadores_Derrubado(index_navio)){
                    municao = municao +3;
                    navios_vivos--;
                    Emitir_Som(1);
               }else{
                    Emitir_Som(2);
               }
          }
     }
}

void Desenhar_Hud(){
     char texto[2];

     Escreve(" Munição: ");
     IntToStr(municao, texto);
     Escreve(texto);
     Escreve("\r");
     Escreve(" Navios Navegando: ");
     IntToStr(navios_vivos, texto);
     Escreve(texto);
     Escreve("\r");
     Escreve(" Navios Afundados: ");
     IntToStr(15-navios_vivos, texto);
     Escreve(texto);
     Escreve("\r");
     Escreve("\r");
}

void Limpar_Tela(){
     Escreve_Char(12);
}

void Executando(){
     int linha, coluna;
     short jogando;
     unsigned char *texto;
     jogando = 1;
     while (jogando){
          Limpar_Tela();
          Desenhar_Hud();
          Desenhar_Mapa();
          texto = Retornar_Texto();
          Escreve(texto);
          Escreve("\r");
          if(texto[0] == '*'){
               Limpar_Tela();
               Desenhar_Mapa_Aberto();
          }else if(posicaoValida(texto)){
               linha = Extrair_Index_Linha(texto);
               coluna = Extrair_Index_Coluna(texto);
               if(linha > -1 && coluna > -1){
                    Atirar_Posicao(linha, coluna);

                    if(navios_vivos == 0){
                         Emitir_Som(3);
                         Limpar_Tela();
                         Escreve(" Ganhou!!!");
                         while(1);
                    }
                    if( municao == 0){
                         Emitir_Som(4);
                         Limpar_Tela();
                         Escreve(" Perdeu!! :( ");
                         while(1);
                    }
               }
          }
     }
}

void Aguardar_Inicio(){
     unsigned char tecla;
     Escreve("Precione uma tecla para iniciar! \r");
     while(1){
       if (UART1_Data_Ready() == 1) {
         break;
       }
     }

     SRAND(semente);
}

void main() {
     Inicializar_Globais();
     Aguardar_Inicio();
     Inicializar_Variavel_Linha_Index();
     Inicializar_Variavel_Numeros_Index();

     Inicializar_Navios();
     Inicializar_Mapa();

     Executando();
}