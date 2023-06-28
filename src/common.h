#pragma once

#include <assert.h>
#include <cmath>
#include <cstdint>

#ifndef DISABLE_COPY
#define DISABLE_COPY(T)					\
		explicit T(const T&) = delete;	\
		T& operator=(const T&) = delete;
#endif // !DISABLE_COPY

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(T)				\
		explicit T(T&&) = delete;	\
		T& operator=(T&&) = delete;
#endif // !DISABLE_MOVE

#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(T) DISABLE_COPY(T) DISABLE_MOVE(T)
#endif // !DISABLE_COPY_AND_MOVE
