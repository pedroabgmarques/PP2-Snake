#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

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
int pontos = 0;
int aComer = 0;

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
			printf("+");
		}
		else{
			printf(".");
		}
		desenharElementos(elemento->seguinte, tipo);
	}
}

void desenharBocaCobra(elemento elemento){
	if (elemento == NULL){
		return 0;
	}
	else{
		coordJanela.X = elemento->coluna + 1;
		coordJanela.Y = elemento->linha + 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
		printf("0");
	}
}

//Remover elemento da lista ligada - recursivamente
elemento removerRecursivo(elemento enderecoInicioLista, int linha, int coluna){
	elemento aux;
	if (enderecoInicioLista != NULL){
		//Lista não está vazia
		if (enderecoInicioLista->linha == linha
			&& enderecoInicioLista->coluna == coluna){
			//encontramos o elemento a eliminar
			aux = enderecoInicioLista->seguinte;
			free(enderecoInicioLista);
			return aux;
		}
		else{
			//não é este o elemento e eliminar, continuar a recursão
			enderecoInicioLista->seguinte = removerRecursivo(enderecoInicioLista->seguinte, linha, coluna);
			return enderecoInicioLista;
		}
	}
	else{
		//Lista vazia
		return enderecoInicioLista;
	}
}

void criarComida(){
	comida = insereElemento(comida, 1 + rand() % (linhas - 1), 1 + rand() % (colunas - 1), 1);
}

//Contagem de elementos recursiva
int quantidade(elemento endereco){
	if (endereco == NULL){
		return 0;
	}
	else{
		//A função invoca-se a ela própria
		return (1 + quantidade(endereco->seguinte));
	}
}

void verificarSeCome(){
	elemento cobra = snake, papa = comida;
	if (cobra == NULL){
		return 0;
	}
	else{
		//O primeiro elemento é a cabeça, vamos ver se está sobreposta a uma comida
		while (papa != NULL){
			if (cobra->coluna == papa->coluna
				&& cobra->linha == papa->linha){
				//Temos aqui uma comida!
				//Alteramos a flag
				aComer = 1;
				//Acrescentamos um elemento à lista da cobra
				snake = insereElemento(snake, papa->linha, papa->coluna, 0);
				//Aumentamos os pontos do jogador
				pontos += 10;
				//Removemos esta comida da lista de comidas
				comida = removerRecursivo(comida, papa->linha, papa->coluna);
				//Inserimos uma nova comida num local aleatorio
				if (quantidade(comida) < 2){
					criarComida();
				}
				break;
			}
			papa = papa->seguinte;
		}
	}
}
// verificar se a cabeça colide com os limites do tabuleiro ou com o proprio corpo
bool verificarColisao()
{

	if (snake == NULL)
	{
		return false;
	}

	else 
	
		// verifica se a cabeça da cobra encontrasse no limite do tabuleiro
		if (snake->linha  == 0 || snake->linha == linhas || snake->coluna == 0 || snake->coluna == colunas)
		{
			
			return true;

		}return false;
		
		
	

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
	printf("%d", pontos);
}
// desenhar fim do jogo
//mensegem de fim do joo
//pontuacao maxima

void desenharFimDoJogo(){
	coordJanela.X = colunas -25;
	coordJanela.Y = 10;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("FIM DO JOGO ANIMAL :)");
	coordJanela.X;;
	coordJanela.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("Pontuacao Maxima : %d\n", pontos);
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
void loadComida(){
	elemento comida = NULL;
	for (int i = 0; i < 6; i++){
		criarComida();
	}
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

void moverCobra(int velocidade){
	if (contadorMovimento > velocidade){
		snake = mover(snake, direccaoMovimento);
		contadorMovimento = 0;
		if (aComer == 1) aComer = 0;
	}
	//atualizar contador de movimento
	contadorMovimento++;
}

void Update(){
	//mover cobra
	moverCobra(10);
	//Atualizar input do teclado
	atualizarInput();
	//Verificar se está a comer
	verificarSeCome();
	
}

void Draw(){
	//limpar o ecrã
	system("cls");
	//desenhar limites do espaço de jogo
	desenharLimites();
	//desenhar comida
	desenharElementos(comida, 1);
	//desenhar cobra
	desenharElementos(snake, 0);
	//Se estiver a comer, desenhar boca
	if (aComer == 1){
		desenharBocaCobra(snake);
	}
	//Desenhar pontuacao
	desenharPontuacao();
}

int main(){

	srand(time(NULL));

	//Criar uma cobra com 3 elementos iniciais
	snake = loadCobra();

	//Criar elementos de comida espalhados pelo tabuleiro
	loadComida();

	while (verificarColisao){
		Update();
		Draw();
	}
	desenharFimDoJogo();

	return 1;
}