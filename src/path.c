#include "game.h"
#include <sys/stat.h>

int game_path_is_dir(char *path) {
	struct stat path_stat;
	
	if (stat(path, &path_stat))
		return 0;
	return S_ISDIR(path_stat.st_mode);
}
