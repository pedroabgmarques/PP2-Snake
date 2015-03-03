#include <stdio.h>
#include <windows.h>
#include <conio.h>

COORD 
	//coordenadas em que são criados elementos da cobra
	coordElemento, 
	//coordenadas utilizadas para desenhar os elementos
	coordJanela;

#define linhas 20
#define colunas 50

typedef struct registo{
	int linha;
	int coluna;
	struct registo *seguinte;
}* elemento;

//Variáveis globais
int contadorMovimento = 0;
int direccaoMovimento = 2;
elemento snake, comida;

//Desenha os limites do espaço de jogo
void desenharLimites(){
	for (int i = 0; i <= linhas; i++){
		for (int j = 0; j <= colunas; j++){
			if (i == 0 || i == linhas || j == 0 || j == colunas){
				coordJanela.X = j;
				coordJanela.Y = i;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
				printf("+");
			}
		}
	}
}

//Insere um elemento na lista ligada
elemento insereElemento(elemento seguinte, int linha, int coluna, int tipoElemento){
	//Aloca o espaço necessário na memória
	elemento elemento = malloc(sizeof(struct registo));
	//Dar os valores ao aluno
	elemento->linha = linha;
	elemento->coluna = coluna;
	if (tipoElemento == 0){
		//Elementos da cobra
		coordElemento.X = linha;
		coordElemento.Y = coluna;
	}
	elemento->seguinte = seguinte;
	//devolve o endereço de memória do espaço alocado
	return elemento;
}

elemento mover(elemento apt, int direcao)
{
	elemento aux = apt, ant = apt, actual = apt;
	switch (direcao)
	{
		//Criar uma nova cabeça para a cobra
	case 2:
		apt = insereElemento(apt, coordElemento.X + 1, coordElemento.Y, 0);
		break;
	case 6:
		apt = insereElemento(apt, coordElemento.X, coordElemento.Y + 1, 0);
		break;
	case 8:
		apt = insereElemento(apt, coordElemento.X - 1, coordElemento.Y, 0);
		break;
	case 4:
		apt = insereElemento(apt, coordElemento.X, coordElemento.Y - 1, 0);
		break;
	default:
		break;
	}

	//Guardar a lista numa variável temp que podemos alterar
	actual = apt;

	//Remover a cauda da cobra
	while (actual != NULL){
		//Enquanto não chegarmos ao fim da lista..
		if (actual->seguinte != NULL){
			//Tem seguinte, guardar este elemento no anterior e passar para o proximo
			ant = actual;
			actual = actual->seguinte;
		}
		else{
			//Não tem seguinte, é a cauda!
			//Retirar o seguinte do penultimo elemento
			ant->seguinte = NULL;
			//Apagar o ultimo, o penultimo passa a ser o ultimo
			free(actual);
			//devolver a lista
			return apt;
		}
	}

	return(apt);
}

//Desenhar os elementos da cobra e a comida
void desenharElementos(elemento elemento, int tipo){
	if (elemento == NULL){
		return 0;
	}
	else{
		coordJanela.X = elemento->coluna+1;
		coordJanela.Y = elemento->linha+1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);

		if (tipo == 0){
			printf("O");
		}
		else{
			printf("X");
		}
		desenharElementos(elemento->seguinte, tipo);
	}
}

//Desenha o numero de pontos que o jogador tem
void desenharPontuacao(){
	coordJanela.X = colunas + 12;
	coordJanela.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("PONTOS");
	coordJanela.X += 2;
	coordJanela.Y = 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("0");
}

//Gera uma cobra inicial
elemento loadCobra(){
	elemento snake = NULL;
	snake = insereElemento(snake, 10, 12, 0);
	snake = insereElemento(snake, 10, 11, 0);
	snake = insereElemento(snake, 10, 10, 0);
	return snake;
}

//Gera os elementos de comida iniciais
elemento loadComida(){
	elemento comida = NULL;
	comida = insereElemento(comida, 10, 5, 1);
	comida = insereElemento(comida, 12, 1, 1);
	comida = insereElemento(comida, 17, 8, 1);
	comida = insereElemento(comida, 2, 10, 1);
	comida = insereElemento(comida, 6, 14, 1);
	return comida;
}

//Lida com input do teclado
void atualizarInput(){
	if (_kbhit())
	{
		int key = _getch();

		switch (key)
		{
		case '2':
			direccaoMovimento = 2;
			break;
		case '6':
			direccaoMovimento = 6;
			break;
		case '8':
			direccaoMovimento = 8;
			break;
		case '4':
			direccaoMovimento = 4;
			break;
		default:
			break;
		}
	}
}

void Update(){
	//mover cobra
	if (contadorMovimento > 5){
		snake = mover(snake, direccaoMovimento);
		contadorMovimento = 0;
	}
	//atualizar contador de movimento
	contadorMovimento++;
	//Atualizar input do teclado
	atualizarInput();
}

void Draw(){
	//limpar o ecrã
	system("cls");
	//desenhar limites do espaço de jogo
	desenharLimites();
	//desenhar cobra
	desenharElementos(snake, 0);
	//desenhar comida
	desenharElementos(comida, 1);
	//Desenhar pontuacao
	desenharPontuacao();
}



int main(){

	//Criar uma cobra com 3 elementos iniciais
	snake = loadCobra();

	//Criar elementos de comida espalhados pelo tabuleiro
	comida = loadComida();

	while (1){
		Update();
		Draw();
	}

	return 1;
}