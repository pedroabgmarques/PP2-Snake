#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

COORD 
// ahsjgdfha
	//coordenadas em que são criados elementos da cobra
	coordElemento, 
	//coordenadas utilizadas para desenhar os elementos
	coordJanela;

#define linhas 40
#define colunas 40

typedef struct registo{
	int linha;
	int coluna;
	struct registo *seguinte;
}* elemento;

//Variáveis globais
int contadorMovimento = 0;
int direccaoMovimento = 2;
int direccaoMovimentoAnterior = 2;
elemento snake, comida;
int pontos = 0;
int aComer = 0;
bool endgame = false;
bool sairJogo = false;
ALLEGRO_FONT *font;
ALLEGRO_KEYBOARD_STATE state;

//Criar um display para o Allegro
ALLEGRO_DISPLAY *display = NULL;

//Cores
ALLEGRO_COLOR RED, BLACK, ORANGE;

//Desenha os limites do espaço de jogo
void desenharLimites(){
	for (int i = 0; i <= linhas; i++){
		for (int j = 0; j <= colunas; j++){
			if (i == 0 || i == linhas || j == 0 || j == colunas){
				/*coordJanela.X = j;
				coordJanela.Y = i;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
				printf("+");*/
				al_draw_rectangle(j * 10, i * 10, j * 10 + 10, i * 10 + 10, BLACK, 2);
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

//Insere um elemento na cauda lista ligada
elemento insereElementoCauda(elemento snake, int linha, int coluna, int tipoElemento){
	
	//Aloca o espaço necessário na memória
	elemento elemento = malloc(sizeof(struct registo));

	elemento->seguinte = NULL;

	//encontrar a cauda da cobra
	while (snake->seguinte != NULL){
		snake = snake->seguinte;
	}
	
	elemento->linha = snake->linha;
	elemento->coluna = snake->coluna;

	snake->seguinte = elemento;

	return snake;
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
		/*coordJanela.X = elemento->coluna+1;
		coordJanela.Y = elemento->linha+1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);*/

		if (tipo == 0){
			/*printf("+");*/
			//Cor sólida
			al_draw_filled_rectangle(elemento->coluna * 10, elemento->linha * 10, elemento->coluna * 10 + 10, elemento->linha * 10 + 10, RED);
			//Borda
			al_draw_rectangle(elemento->coluna * 10 + 2, elemento->linha * 10 + 2, elemento->coluna * 10 + 8, elemento->linha * 10 + 8, BLACK, 2);
		}
		else{
			/*printf("O");*/
			al_draw_filled_circle(elemento->coluna * 10 + 5, elemento->linha * 10 + 5, 6, ORANGE);
		}
		desenharElementos(elemento->seguinte, tipo);
	}
}

void desenharBocaCobra(elemento elemento){
	if (elemento == NULL){
		return 0;
	}
	else{
		/*coordJanela.X = elemento->coluna + 1;
		coordJanela.Y = elemento->linha + 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
		printf("*");*/
		al_draw_filled_rectangle(elemento->coluna * 10, elemento->linha * 10, elemento->coluna * 10 + 10, elemento->linha * 10 + 10, BLACK);
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
	comida = insereElemento(comida, (1+rand() % (linhas-2)), (1+rand() % (colunas-2)), 1);
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
				cobra = insereElementoCauda(snake, papa->linha, papa->coluna, 0);
				//Aumentamos os pontos do jogador
				pontos += 6;
				//Removemos esta comida da lista de comidas
				comida = removerRecursivo(comida, papa->linha, papa->coluna);
				//Inserimos uma nova comida num local aleatorio
				if (quantidade(comida) < 5){
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
	{
		// verifica se a cabeça da cobra encontrasse no limite do tabuleiro
		if (snake->linha == 0 || snake->linha == linhas || snake->coluna == 0 || snake->coluna == colunas )
		{
			return true;
		}	
		
		if (snake->seguinte != NULL){
			elemento cobra = snake->seguinte;
			while (cobra != NULL){
				if (cobra->linha == snake->linha && cobra->coluna == snake->coluna){
					return true;
				}
				cobra = cobra->seguinte;
			}
		}
		

		return false;
	}
}

//Desenha o numero de pontos que o jogador tem
void desenharPontuacao(){
	/*coordJanela.X = colunas + 12;
	coordJanela.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("PONTOS");
	coordJanela.X += 2;
	coordJanela.Y = 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("%d", pontos);*/

	char cated_string[128];
	sprintf(cated_string, "%s%d", "Score: ", pontos);

	al_draw_text(font, al_map_rgb(255, 255, 255), 525, 10, ALLEGRO_ALIGN_CENTRE, cated_string);
}

//ALLEGRO HELPER - press any key
void al_readkey() { 
	ALLEGRO_EVENT_QUEUE *queue = NULL; 
	queue = al_create_event_queue(); 
	al_register_event_source(queue, al_get_keyboard_event_source()); 
	al_wait_for_event(queue, NULL); 
	al_destroy_event_queue(queue); 
}

//desenhar fim do jogo
//mensegem de fim do joo
//pontuacao maxima
void desenharFimDoJogo(){
	/*coordJanela.X = colunas -25;
	coordJanela.Y = 10;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("GAME OVER!");
	coordJanela.X;;
	coordJanela.Y += 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordJanela);
	printf("Pontuacao Maxima : %d\n", pontos);*/

	//TODO:
	//ALLEGRO game over e esperar por teclado antes de passar engame para true

	al_draw_text(font, al_map_rgb(255, 255, 255), 640/2, 480/3, ALLEGRO_ALIGN_CENTRE, "Game Over");

	al_flip_display();

	al_readkey();

	endgame = true;
	
	
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

	al_get_keyboard_state(&state);

	if (al_key_down(&state, ALLEGRO_KEY_DOWN)){
		// Andar para baixo
		if (direccaoMovimentoAnterior != 8)
		{
			direccaoMovimento = 2;
		}
	}

	if (al_key_down(&state, ALLEGRO_KEY_RIGHT)){
		// Andar para a direita
		if (direccaoMovimentoAnterior != 4)
		{
			direccaoMovimento = 6;
		}
	}

	if (al_key_down(&state, ALLEGRO_KEY_UP)){
		// Andar para cima
		if (direccaoMovimentoAnterior != 2)
		{
			direccaoMovimento = 8;
		}
	}

	if (al_key_down(&state, ALLEGRO_KEY_LEFT)){
		// Andar para a esquerda
		if (direccaoMovimentoAnterior != 6)
		{
			direccaoMovimento = 4;
		}
	}
}

void moverCobra(int velocidade){
	if (contadorMovimento > velocidade){
		snake = mover(snake, direccaoMovimento);
		contadorMovimento = 0;
		if (aComer == 1) aComer = 0;
		direccaoMovimentoAnterior = direccaoMovimento;
	}
	//atualizar contador de movimento
	contadorMovimento++;
}

void Update(){
	//mover cobra
	moverCobra(50);
	//Atualizar input do teclado
	atualizarInput();
	//Verificar se está a comer
	verificarSeCome();
	
}
// recomeçar o jogo
// variaveis são iniciadas
void Restart()
{

	snake = NULL;
	comida = NULL;
	pontos = 0;
	snake = loadCobra();
	loadComida();
	direccaoMovimento = 2;
	endgame = false;

}

// metodo para reiniciar o jogo
void newGame()
{

	al_clear_to_color(al_map_rgb(63, 20, 42));

	al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, ALLEGRO_ALIGN_LEFT, "Play Again?");
	al_draw_text(font, al_map_rgb(255, 255, 255), 0, 32, ALLEGRO_ALIGN_LEFT, "Y for yes or N for no.");

	al_flip_display();

	al_get_keyboard_state(&state);

	if (al_key_down(&state, ALLEGRO_KEY_Y)){
		Restart();
	}
	if (al_key_down(&state, ALLEGRO_KEY_N)){
		sairJogo = true;
		Restart();
	}
}

//Desenhar o jogo
void Draw(){
	//limpar o ecrã
	//system("cls");
	//Limpar o backbuffer para a cor de fundo
	al_clear_to_color(al_map_rgb(63, 20, 42));

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

	//Desenhar o backbuffer no ecrã
	al_flip_display();
}

//Destroi os objetos criados
void shutDown(){
	if (display){
		al_destroy_display(display);
	}
	if (font){
		al_destroy_font(font);
	}
}



int main(int argc, char **argv){

	//Desligar a consola
	FreeConsole();

	//Inicializar o allegro
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	//Inicializar o display
	display = al_create_display(640, 480);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	//Inicializar o addon de primitivas
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		return -1;
	}
	//Titulo da janela
	al_set_window_title(display, "Ultimate Snake");
	//Inicializar o keyboard
	al_install_keyboard();
	//initialize the font addon
	al_init_font_addon(); 
	//initialize the ttf (True Type Font) addon
	al_init_ttf_addon();
	//Inicializar uma fonte
	font = al_load_ttf_font("pirulen.ttf", 28, 0);
	if (!font){
		fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
		return -1;
	}

	//Inicializar cores
	RED = al_map_rgb(255, 0, 0);
	BLACK = al_map_rgb(0, 0, 0);
	ORANGE = al_map_rgb(255, 165, 0);

	srand(time(NULL));

	//Criar uma cobra com 3 elementos iniciais
	snake = loadCobra();

	//Criar elementos de comida espalhados pelo tabuleiro
	loadComida();
	
	while (!sairJogo)
	 {
		 if(endgame == false)
		 {
			 
			 if (!verificarColisao())
			 {
				 Update();
				 Draw();
			 }
			 else
			 {
				 desenharFimDoJogo();
			 }
		 }
		 else
		 {
			 newGame();
		 }
	 }
	
	
	 shutDown();
	 return 1;
}