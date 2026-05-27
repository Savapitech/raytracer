// Compile the stb_image implementation exactly once.
// This file is only included in the build when HEADLESS=ON.
#ifdef HEADLESS_BUILD

// Suppress warnings from the third-party header
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunused-result"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma GCC diagnostic pop

#endif // HEADLESS_BUILD
