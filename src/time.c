#include "game.h"
#include <time.h>
#include <unistd.h>

static struct timespec loop_t0;
static uint64_t current_time_count = 0;
static uint64_t current_time_frame = 0;
static uint64_t current_fps = 0;

static uint64_t fps_target = GAME_FPS;
static uint64_t fps_target_as_us = 1000 * 1000 / GAME_FPS; 

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
