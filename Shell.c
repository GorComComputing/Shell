// Командный интерпретатор Shell
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024 // Размер буфера ввода
#define LSH_TOK_BUFSIZE 64  // Размер буфера с указателями на токены
#define LSH_TOK_DELIM " \t\r\n\a" // Разделители для токенизации

// Массив имен встроенных команд
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

// Массив указателей на функции встроенных команд
int (*builtin_func[]) (char **) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *line);
int lsh_launch(char **args);
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);


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


// Запуск процессов
int lsh_launch(char **args) {
	
}


//
int lsh_num_builtins() {
		return sizeof(builtin_str) / sizeof(char *);
}


// Команда смены каталога
int lsh_cd(char **args) {
	// Проверяем, есть ли второй аргумент
	if (args[1] == NULL) {
		// Сообщение об ошибке
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	}
	else {
		// Вызываем функцию смены каталога
		//if (chdir(args[1]) != 0) {
		//	perror("lsh");
		//}
	}
	return 1;
}


// Команда help
int lsh_help(char **args) {
	int i;
	printf("Gor.Com Shell\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");
	
	// Выводим массив имен встроенных команд
	for (i = 0; i < lsh_num_builtins(); i++) {
		printf(" %s\n", builtin_str[i]);
	}
	printf("Use the man command for information on other programs.\n");
	return 1;
}


// Команда выход
int lsh_exit(char **args) {
	return 0;
}


// Выполнение встроенных команд или запуск процессов
int lsh_execute(char **args) {
	int i;
	
	// Проверяем, не путая ли строка введена
	if (args[0] == NULL) {
		return 1;
	}
	
	for (i = 0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	
	return lsh_launch(args);
}

