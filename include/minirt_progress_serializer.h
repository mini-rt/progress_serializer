#ifndef MINIRT_PROGRESS_SERIALIZER_H
#define MINIRT_PROGRESS_SERIALIZER_H

#include "minirt_base.h"

typedef struct {
	size_t length;
	char content[];
} minirt_progress_serializer_buffer;

#define MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_SUCCESS 0
#define MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_FAILURE_MALLOC 1
#define MINIRT_PROGRESS_SERIALIZER_SERIALIZE_RESULT_FAILURE_TOO_LARGE 2

int minirt_progress_serializer_serialize(
	minirt_progress *progress,
	minirt_progress_serializer_buffer **out);

#define MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_SUCCESS 0
#define MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_MALLOC 1
#define MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_INVALID 2
#define MINIRT_PROGRESS_SERIALIZER_DESERIALIZE_RESULT_FAILURE_INCOMPATIBLE 3

int minirt_progress_serializer_deserialize(
	minirt_progress_serializer_buffer *buffer,
	minirt_progress **out);

#endif
