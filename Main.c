#include <stdio.h>
#include <windows.h>

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
	if (direcao == 2)
	{

		//Criar uma nova cabeça para a cobra
		apt = insereElemento(apt, coordElemento.X, coordElemento.Y + 1, 0);

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

//Gera uma cobra inicial
elemento loadCobra(){
	elemento snake = NULL;
	snake = insereElemento(snake, 0, 0, 0);
	snake = insereElemento(snake, 0, 1, 0);
	snake = insereElemento(snake, 0, 2, 0);
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

int main(){

	//Criar uma cobra com 3 elementos iniciais
	elemento snake = loadCobra();

	//Criar elementos de comida espalhados pelo tabuleiro
	elemento comida = loadComida();
	
	while (1){
		//limpar o ecrã
		system("cls");
		//desenhar limites do espaço de jogo
		desenharLimites();
		//desenhar cobra
		desenharElementos(snake, 0);
		//desenhar comida
		desenharElementos(comida, 1);
		//mover cobra
		snake = mover(snake, 2);
	}

	return 1;
}