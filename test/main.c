#include "minirt_base.h"
#include "minirt_progress_serializer.h"

#include <stdbool.h>
#include <stdlib.h>

#define WIDTH 16
#define HEIGHT 9

static void assert(bool b) {
	if (!b) exit(EXIT_FAILURE);
}

int main() {
	minirt_progress *original = malloc(
		sizeof(minirt_progress) + HEIGHT * WIDTH * sizeof(minirt_pixel));
	if (!original) return EXIT_FAILURE;
	original->total_width = WIDTH;
	original->total_height = HEIGHT;
	original->total_depth = rand();
	original->complete_width = rand() % WIDTH;
	original->complete_height = rand() % HEIGHT;
	original->complete_depth = rand();
	for (size_t i = 0; i < original->total_height; i++) {
		for (size_t j = 0; j < original->total_width; j++) {
			size_t index = i * original->total_width + j;
			original->pixels[index].r = rand() * 1.0f / RAND_MAX;
			original->pixels[index].g = rand() * 1.0f / RAND_MAX;
			original->pixels[index].b = rand() * 1.0f / RAND_MAX;
		}
	}

	minirt_progress_serializer_buffer *buffer;
	if (minirt_progress_serializer_serialize(original, &buffer))
		return EXIT_FAILURE;

	minirt_progress *deserialized;
	if (minirt_progress_serializer_deserialize(buffer, &deserialized))
		return EXIT_FAILURE;

	assert(original->total_width == deserialized->total_width);
	assert(original->total_height == deserialized->total_height);
	assert(original->total_depth == deserialized->total_depth);
	assert(original->complete_width == deserialized->complete_width);
	assert(original->complete_height == deserialized->complete_height);
	assert(original->complete_depth == deserialized->complete_depth);
	for (size_t i = 0; i < original->total_height; i++) {
		for (size_t j = 0; j < original->total_width; j++) {
			size_t index = i * original->total_width + j;
			assert(original->pixels[index].r == deserialized->pixels[index].r);
			assert(original->pixels[index].g == deserialized->pixels[index].g);
			assert(original->pixels[index].b == deserialized->pixels[index].b);
		}
	}
}
