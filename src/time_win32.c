#ifdef _WIN32

#include "game.h"
#include <windows.h>

#if 0
struct timespec { int64_t tv_sec; int64_t tv_nsec; };
#endif

#undef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0

static struct timespec loop_t0;
static uint64_t current_time_count = 0;
static uint64_t current_time_frame = 0;
static uint64_t current_fps = 0;

static uint64_t fps_target = GAME_FPS;
static uint64_t fps_target_as_us = 1000 * 1000 / GAME_FPS; 

int clock_gettime(int x, struct timespec *spec) {
	(void)x;
	int64_t wintime;
	GetSystemTimeAsFileTime((FILETIME *)&wintime);
	
	wintime -= 116444736000000000LL;
	spec->tv_sec = wintime / 10000000LL;
	spec->tv_nsec = wintime % 10000000LL * 100;
	
	return 0;
}

int64_t game_get_time() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec * 1000000;
}

void usleep(int64_t us) {
	if (us <= 0)
		return ;

	HANDLE timer;
	timer = CreateWaitableTimer(NULL, TRUE, NULL);	
	if (timer == NULL)
		return ;

	LARGE_INTEGER li;
	li.QuadPart = -10LL * us;

	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		CloseHandle(timer);
		return ;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

void game_loop_start() {
	clock_gettime(CLOCK_MONOTONIC, &loop_t0);
}

void game_loop_end() {
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);

	uint64_t frame_time = (now.tv_sec - loop_t0.tv_sec) * 1000 * 1000;
	frame_time += (now.tv_nsec - loop_t0.tv_nsec) / 1000;
	if (frame_time < fps_target_as_us) {
		usleep(fps_target_as_us - frame_time);
	}

	clock_gettime(CLOCK_MONOTONIC, &now);

	frame_time = (now.tv_sec - loop_t0.tv_sec) * 1000 * 1000;
	frame_time += (now.tv_nsec - loop_t0.tv_nsec) / 1000;
	current_time_frame += frame_time;
	current_time_count++;
	if (current_time_count == 30) {
		current_fps = 1000 * 1000 * current_time_count / current_time_frame;
		current_time_count = 0;
		current_time_frame = 0;
	}
}

int game_get_fps() {
	return current_fps;
}

void game_set_fps_target(uint64_t target) {
	if (target == 0) {
		PRINT_ERR("Error: target fps cannot be 0\n");
		return ;
	}
	fps_target = target;
	fps_target_as_us = 1000 * 1000 / target;
}

uint64_t game_get_fps_target() {
	return fps_target;
}

#endif
