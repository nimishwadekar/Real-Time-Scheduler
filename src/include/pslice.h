#ifndef PSLICE_H
#define PSLICE_H 1

#include <stddef.h>

// Represents a slice of a periodic job, as stored in a schedule.
typedef struct PSlice *PSlicePtr;

PSlicePtr PSlice_new(int task, int job, double slice_time);
void PSlice_delete(PSlicePtr slice);

// The periodic task the slice belongs to.
int PSlice_task(PSlicePtr slice);

// The periodic job instance the slice belongs to.
int PSlice_job(PSlicePtr slice);

// The execution time of the slice.
double PSlice_slice_time(PSlicePtr slice);


// Represents a fixed-size array of PSlices.
typedef struct PSliceArray *PSliceArrayPtr;

PSliceArrayPtr PSliceArray_new(size_t size);
void PSliceArray_delete(PSliceArrayPtr array);

size_t PSliceArray_size(PSliceArrayPtr array);

// The PSlice at index.
PSlicePtr PSliceArray_get(PSliceArrayPtr array, size_t index);

// Sets the element at index to slice.
int PSliceArray_set(PSliceArrayPtr array, size_t index, PSlicePtr slice);

#endif /* PSLICE_H */