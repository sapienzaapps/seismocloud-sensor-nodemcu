#ifndef __CMA_H
#define __CMA_H

// Cumulative Moving Average and standard deviation formulas:
// https://en.wikipedia.org/wiki/Moving_average
// CMA_new = CMA_current + (new_value - CMA_current) / (values_count+1)
// CMA_StdDev_new = (new_value - CMA_current) * (new_value - CMA_new)

// Simple CMA
typedef struct {
  double value;
  uint32_t count;
} CMA;

#define CMA_ADD(avg, new_value)                        \
avg.value += (new_value - avg.value) / (avg.count + 1);  \
avg.count++

#define CMA_RESET(avg)    avg.value = 0; avg.count = 0;

#define CMA_COPY(dst, src) dst.value = src.value; dst.count = src.count;

// CMA with Standard deviation
typedef struct {
  double value;
  double stddev;
  uint32_t count;
} CMA_StdDev;

#define CMA_STDDEV_ADD(avg, new_value)                  \
avg.stddev += (new_value - avg.value) * (new_value - (avg.value + (new_value - avg.value) / (avg.count + 1)));  \
avg.value += (new_value - avg.value) / (avg.count + 1);   \
avg.count++

#define CMA_STDDEV_GET(avg)      sqrt(avg.stddev / (avg.count - 1))

#define CMA_STDDEV_RESET(avg)    avg.value = 0; avg.stddev = 0; avg.count = 0;

#define CMA_STDDEV_COPY(dst, src) dst.value = src.value; dst.stddev = src.stddev; dst.count = src.count;

#endif
