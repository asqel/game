#include "game.h"

char *read_file(char *path) {
	FILE *f = fopen(path, "r");
	if (!f)
		return NULL;

	if (fseek(f, 0, SEEK_END)) {
		fclose(f);
		return NULL;
	}
	long int len = ftell(f);
	if (len < 0) {
		fclose(f);
		return NULL;
	}
	fseek(f, 0, SEEK_SET);
	char *res = malloc(len + 1);
	if (!res) {
		fclose(f);
		return NULL;
	}
	res[len] = '\0';
	fread(res, 1, len, f);
	fclose(f);
	return res;
}
