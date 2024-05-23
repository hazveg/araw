#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// string helpers
int string_length(char* string);
// system stuff
char* get_file_content(char* path);
int is_file(char* possible_path);
int is_pipe();
// actual functionality
void output(char character);
int output_string_with_newlines(char* string);
int output_pipe_with_newlines();
int argument_data(char* content);



int main(int argc, char* argv[]) {
	if (argc >= 2) {
		return argument_data(argv[1]);
	}

	return output_pipe_with_newlines();
}



int is_pipe() {
	struct stat stdin_stat;
	fstat(fileno(stdin), &stdin_stat);
	return S_ISFIFO(stdin_stat.st_mode);
}

int output_pipe_with_newlines() {
	char character;
	int i = 0;

	if (!is_pipe()) {
		printf("lnl: Nothing provided\n");
		return -1;
	}

	while ((character = getchar()) != EOF) {
		output(character);
		i++;
	}

	if (i == 0) {
		printf("lnl: Nothing provided to pipe\n");
		return -1;
	} else {
		return 0;
	}
}

void output(char character) {
	if (character != '\n') {
		printf("%c", character);
		return;
	}

	printf("\x1b[31m" "\\n" "\x1b[0m" "\n");
}

int argument_data(char* content) {
	int is_mallocd = 0;

	if (is_file(content)) {
		content = get_file_content(content);
		// probably still not a great solution, but better.
		switch ((int) content[0]) {
			case 1:
				printf("lnl: Failed to open file pointer\n");
				return -1;
			case 2:
				printf("lnl: Failed to allocate memory for file content\n");
				return -1;
		}

		is_mallocd = 1;
	}
	
	output_string_with_newlines(content);

	if (is_mallocd)
		free(content);

	return 0;
}

int is_file(char* possible_path) {
	struct stat path_status;

	if (stat(possible_path, &path_status) != 0) {
		return 0;
	}

	return 1;
}

int string_length(char* string) {
	int i = 0;
	char character;

	while ((character = string[i]) != '\0') {
		i++;
	}

	return i;
}

int output_string_with_newlines(char* string) {
	int str_length = string_length(string);

	for (int i = 0; i < str_length; i++) {
		output(string[i]);
	}

	return 0;
}

char* get_file_content(char* path) {
	FILE* file_pointer = fopen(path, "r");
	char character;
	int file_length = 0;

	if (file_pointer == NULL)
		return (char*) 1;

	while ((character = fgetc(file_pointer)) != EOF) {
		file_length++;
	}

	char* content = malloc((file_length + 1) * sizeof(char));
	if (content == NULL) {
		printf("Failed to allocate memory for file content");
		return (char*) 2;
	}

	rewind(file_pointer);
	int i = 0;
	while (1) {
		if ((character = fgetc(file_pointer)) == EOF) {
			content[i] = '\0';
			break;
		}

		content[i] = character;
		i++;
	}

	fclose(file_pointer);
	return content;
}
