#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024 // Размер буфера ввода
#define LSH_TOK_BUFSIZE 64  // Размер буфера с указателями на токены
#define LSH_TOK_DELIM " \t\r\n\a" // Разделители для токенизации

void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *line);


int main (int argc, char **argv) { // Поправить аргументы char
	// Инициализация
	printf("Shell: start command line interpretator\n"); // Печатает название программы
	// Главный цикл
	lsh_loop();
	// Завершение работы
	printf("Shell: End program\n"); // Сообщение о завершении программы
	return EXIT_SUCCESS;
}


// Главный цикл
void lsh_loop(void){
	char *line;	 // Указатель на строку ввода
	char **args; // Указатель на указатель на аргументы
	int status = 0; // Статус завершения главного цикла
	
	do {
		printf("> "); // Печатает приглашение ввода команды
		// Получаем указатель на строку ввода
		line = lsh_read_line();
		// Извлекает аргументы
		args = lsh_split_line(line);
		// Определяет завершение главного цикла
		//status = lsh_execute(args);
		
		// Освобождает память
		free(line);
		free(args);
	} while (status);
}


// Возвращает указатель на строку ввода
char *lsh_read_line(void) {
	int bufsize = LSH_RL_BUFSIZE;   // Размер буфера ввода
	int position = 0;               // Позиция символа в строке
	char *buffer = malloc(sizeof(char) * bufsize);  // Выделяем память под буфер ввода
	int c;                          // Текущий символ
	
	// Проверяет, выделилась ли память под буфер ввода
	if (!buffer) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	// Читаем по символу в буфер до конца строки или файла
	while(1) {
		// Читаем символ
		c = getchar();
		
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';  // Заменяем последний символ на 0
			return buffer;
		}
		else {
			buffer[position] = c;
		}
		position++;
		
		// Если буфер переполнен, выделяем еще столько же памяти
		if (position >= bufsize) {
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			
			// Проверяет, выделилась ли память под буфер ввода
			if (!buffer) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);	
			}
		}
	}
}


// Извлекает аргументы
char **lsh_split_line(char *line) {
	int bufsize = LSH_TOK_BUFSIZE;
	int position = 0;      // Позиция символа
	char **tokens = malloc(sizeof(char*) * bufsize);   // Выделяем память под указатель на указатели
	char *token;           // Указатель на текущий токен
	
	// Проверяет, выделилась ли память
	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	// Получаем первый токен из буфера ввода
	token = strtok(line, LSH_TOK_DELIM);
	// Заполняем в цикле указатель на указатели на токены
	while (token != NULL) {
		tokens[position] = token;
		position++;
		
		// Если буфер переполнен, выделяем еще столько же памяти
		if (position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			
			// Проверяет, выделилась ли память
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		// Получаем последующие токены (поэтому NULL)
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	// Последний токен NULL
	tokens[position] = NULL;
	return tokens;
}