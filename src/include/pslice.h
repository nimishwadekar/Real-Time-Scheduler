#ifndef PSLICE_H
#define PSLICE_H 1

#include <stddef.h>

typedef struct PSlice *PSlicePtr;

PSlicePtr PSlice_new(int task, int job, double slice_time);
void PSlice_delete(PSlicePtr slice);

int PSlice_task(PSlicePtr slice);
int PSlice_job(PSlicePtr slice);
double PSlice_slice_time(PSlicePtr slice);


typedef struct PSliceArray *PSliceArrayPtr;

PSliceArrayPtr PSliceArray_new(size_t size);
void PSliceArray_delete(PSliceArrayPtr array);

size_t PSliceArray_size(PSliceArrayPtr array);
PSlicePtr PSliceArray_get(PSliceArrayPtr array, size_t index);
int PSliceArray_set(PSliceArrayPtr array, size_t index, PSlicePtr slice);

#endif /* PSLICE_H */