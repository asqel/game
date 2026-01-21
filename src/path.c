#include "game.h"
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>

int game_path_is_dir(char *path) {
	struct stat path_stat;

	if (stat(path, &path_stat))
		return 0;
	return S_ISDIR(path_stat.st_mode);
}

int game_path_is_file(char *path) {
	struct stat path_stat;

	if (stat(path, &path_stat))
		return 0;
	return S_ISREG(path_stat.st_mode);
}

void game_list_files(char *path, void (*callback)(const char *filepath)) {
	DIR *dir = opendir(path);
	struct dirent *entry;

	if (!dir)
		return;

	entry = readdir(dir);
	while (entry) {
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
			entry = readdir(dir);
			continue;
		}
		char *new_path = malloc(strlen(path) + strlen(entry->d_name) + 2);
		if (!new_path) {
			PRINT_ERR("Error: Memory allocation failed (game_list_files)\n");
			closedir(dir);
			return;
		}
		sprintf(new_path, "%s/%s", path, entry->d_name);
		if (game_path_is_dir(new_path))
			game_list_files(new_path, callback);
		else
			callback(new_path);
		entry = readdir(dir);
		free(new_path);
	}
	closedir(dir);
}

void normalize_path(char *path) {
	size_t i = 0;
	while (path[i]) {
		if (path[i] != '/') {
			i++;
			continue;
		}
		size_t start = i;
		while (path[start] == '/')
			start++;
		i++;
		memmove(&path[i], &path[start], strlen(&path[start]) + 1);
	}
}
