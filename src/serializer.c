#include "minirt_base.h"
#include "minirt_progress_serializer.h"

#include <stdlib.h>

static bool is_le(void) {
	unsigned int x = 1;
	return !!*((char *)&x);
}

static void write_le_u(uint32_t u, void *buffer) {
	if (is_le()) {
		*((uint32_t *)buffer) = u;
		return;
	}
	for (size_t i = 0; i < sizeof(uint32_t); i++) {
		((char *)buffer)[i] = ((char *)&u)[sizeof(uint32_t) - 1 - i];
	}
}

static void write_le_f(minirt_f f, void *buffer) {
	if (is_le()) {
		*((minirt_f *)buffer) = f;
		return;
	}
	for (size_t i = 0; i < sizeof(minirt_f); i++) {
		((char *)buffer)[i] = ((char *)&f)[sizeof(minirt_f) - 1 - i];
	}
}

static uint32_t read_le_u(void *buffer) {
	if (is_le()) {
		return *((uint32_t *)buffer);
	}
	uint32_t result;
	for (size_t i = 0; i < sizeof(uint32_t); i++) {
		((char *)&result)[i] = ((char *)buffer)[sizeof(uint32_t) - 1 - i];
	}
	return result;
}

static minirt_f read_le_f(void *buffer) {
	if (is_le()) {
		return *((minirt_f *)buffer);
	}
	minirt_f result;
	for (size_t i = 0; i < sizeof(minirt_f); i++) {
		((char *)&result)[i] = ((char *)buffer)[sizeof(minirt_f) - 1 - i];
	}
	return result;
}

int minirt_progress_serializer_serialize(
	minirt_progress *progress,
	minirt_progress_serializer_buffer **out) {
	if (progress->total_depth > UINT32_MAX
		|| progress->total_height
			> (UINT32_MAX - 28) / 3 / progress->total_width)
		return MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_FAILURE_TOO_LARGE;
	size_t buffer_size = 28
		+ progress->total_width * progress->total_height * 3 * sizeof(minirt_f);
	minirt_progress_serializer_buffer *result
		= malloc(sizeof(minirt_progress_serializer_buffer) + buffer_size);
	if (!result)
		return MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_FAILURE_MALLOC;
	write_le_u(sizeof(minirt_f), result->content);
	write_le_u(progress->total_depth, result->content + 4);
	write_le_u(progress->total_height, result->content + 8);
	write_le_u(progress->total_width, result->content + 12);
	write_le_u(progress->complete_depth, result->content + 16);
	write_le_u(progress->complete_height, result->content + 20);
	write_le_u(progress->complete_width, result->content + 24);
	for (size_t i = 0; i < progress->total_height; i++) {
		for (size_t j = 0; j < progress->total_width; j++) {
			size_t index = i * progress->total_width + j;
			write_le_f(
				progress->pixels[index].r,
				result->content + 28 + (index * 3) * sizeof(minirt_f));
			write_le_f(
				progress->pixels[index].g,
				result->content + 28 + (index * 3 + 1) * sizeof(minirt_f));
			write_le_f(
				progress->pixels[index].b,
				result->content + 28 + (index * 3 + 2) * sizeof(minirt_f));
		}
	}
	result->length = buffer_size;
	*out = result;
	return MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_SUCCESS;
}

int minirt_progress_serializer_deserialize(
	minirt_progress_serializer_buffer *buffer,
	minirt_progress **out) {
	if (buffer->length < 28)
		return MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_INVALID;
	if (read_le_u(buffer->content) != sizeof(minirt_f))
		return MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_INCOMPATIBLE;
	uint32_t total_depth = read_le_u(buffer->content + 4);
	uint32_t total_height = read_le_u(buffer->content + 8);
	uint32_t total_width = read_le_u(buffer->content + 12);
	uint32_t complete_depth = read_le_u(buffer->content + 16);
	uint32_t complete_height = read_le_u(buffer->content + 20);
	uint32_t complete_width = read_le_u(buffer->content + 24);
	if (buffer->length
		!= 28 + total_height * total_width * 3 * sizeof(minirt_f))
		return MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_INVALID;
	minirt_progress *result = malloc(
		sizeof(minirt_progress)
		+ total_height * total_width * sizeof(minirt_pixel));
	if (!result)
		return MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_MALLOC;
	result->total_depth = total_depth;
	result->total_height = total_height;
	result->total_width = total_width;
	result->complete_depth = complete_depth;
	result->complete_height = complete_height;
	result->complete_width = complete_width;
	for (size_t i = 0; i < total_height; i++) {
		for (size_t j = 0; j < total_width; j++) {
			size_t index = i * total_width + j;
			result->pixels[index].r = read_le_f(
				buffer->content + 28 + (index * 3) * sizeof(minirt_f));
			result->pixels[index].g = read_le_f(
				buffer->content + 28 + (index * 3 + 1) * sizeof(minirt_f));
			result->pixels[index].b = read_le_f(
				buffer->content + 28 + (index * 3 + 2) * sizeof(minirt_f));
		}
	}
	*out = result;
	return MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_SUCCESS;
}
