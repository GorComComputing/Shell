// Командный интерпретатор Shell
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024 // Размер буфера ввода
#define LSH_TOK_BUFSIZE 64  // Размер буфера с указателями на токены
#define LSH_TOK_DELIM " \t\r\n\a" // Разделители для токенизации

void loop(void);
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int num_builtins();
int launch(char **args);
int cmd_cd(char **args);
int cmd_help(char **args);
int cmd_exit(char **args);

// Массив имен встроенных команд
char *cmd_str[] = {
	"cd",           // Поменять каталог
	"help",			// Справка
	"exit"			// Выход
};

// Массив указателей на функции встроенных команд
int (*cmd_func[]) (char **) = {
	cmd_cd,
	cmd_help,
	cmd_exit
};


int main (int argc, char **argv) { // Поправить аргументы char
	// Инициализация
	printf("Bash started. This is command line interpretator\n"); // Печатает название программы
	// Главный цикл
	loop();
	// Завершение работы
	printf("Bash ended\n"); // Сообщение о завершении программы
	return EXIT_SUCCESS;
}


// Главный цикл
void loop(void){
	char *line;	 // Указатель на строку ввода
	char **args; // Указатель на указатель на аргументы
	int status = 0; // Статус завершения главного цикла
	
	do {
		printf("$ "); // Печатает приглашение ввода команды
		// Получаем указатель на строку ввода
		line = read_line();
		// Извлекает аргументы
		args = split_line(line);
		// Определяет завершение главного цикла
		status = execute(args);
		
		// Освобождает память
		free(line);
		free(args);
	} while (status);
}


// Возвращает указатель на строку ввода
char *read_line(void) {
	int bufsize = LSH_RL_BUFSIZE;   // Размер буфера ввода
	int position = 0;               // Позиция символа в строке
	char *buffer = malloc(sizeof(char) * bufsize);  // Выделяем память под буфер ввода
	int c;                          // Текущий символ
	
	// Проверяет, выделилась ли память под буфер ввода
	if (!buffer) {
		fprintf(stderr, "Bash: allocation error\n");
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
				fprintf(stderr, "Bash: allocation error\n");
				exit(EXIT_FAILURE);	
			}
		}
	}
}


// Извлекает аргументы
char **split_line(char *line) {
	int bufsize = LSH_TOK_BUFSIZE;
	int position = 0;      // Позиция символа
	char **tokens = malloc(sizeof(char*) * bufsize);   // Выделяем память под указатель на указатели
	char *token;           // Указатель на текущий токен
	
	// Проверяет, выделилась ли память
	if (!tokens) {
		fprintf(stderr, "Bash: allocation error\n");
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
				fprintf(stderr, "Bash: allocation error\n");
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


// Запуск процессов
int launch(char **args) {
	// Здесь должен быть запуск процессов
	printf("Launched: %s with param. = %s \n", args[0], args[1]); // Отладочный вывод
	return 1;
}


//
int num_builtins() {
		return sizeof(cmd_str) / sizeof(char *);
}


// Выполнение встроенных команд или запуск процессов
int execute(char **args) {
	int i;
	
	// Проверяем, не путая ли строка введена
	if (args[0] == NULL) {
		return 1;
	}
	
	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(args[0], cmd_str[i]) == 0) {
			return (*cmd_func[i])(args);
		}
	}
	
	return launch(args);
}


// Команда смены каталога
int cmd_cd(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
		fprintf(stderr, "Bash: expected argument to \"cd\"\n");
	}
	else {
		// Вызываем функцию смены каталога
		//if (chdir(args[1]) != 0) {
		//	perror("Bash");
		//}
	}
	return 1;
}


// Команда help
int cmd_help(char **args) {
	int i;
	printf("Bash version 0.0.1\n");
	printf("Copyright (c) 2022 Evgeny Goryachev Gor.Com\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");
	
	// Выводим массив имен встроенных команд
	for (i = 0; i < num_builtins(); i++) {
		printf(" %s\n", cmd_str[i]);
	}
	return 1;
}


// Команда выход
int cmd_exit(char **args) {
	return 0;
}




