#pragma once

#include <stdio.h>
#define CHECKPOINT fprintf(stderr, "CHECKPOINT: %s %d\n", __FILE__, __LINE__);

#define PARALLEL_CHECKPOINT fprintf(stderr, "RANK %d, CHECKPOINT: %s %d\n", MessagePasser::Rank(), __FILE__, __LINE__);