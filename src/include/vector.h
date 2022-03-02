#ifndef VECTOR_H
#define VECTOR_H 1

#include "sjob.h"
#include <stddef.h>

typedef struct VectorInt *VectorIntPtr;

VectorIntPtr VectorInt_new(void);
void VectorInt_delete(VectorIntPtr vector);

size_t VectorInt_size(VectorIntPtr vector);
int VectorInt_get(VectorIntPtr vector, size_t index);
void VectorInt_set(VectorIntPtr vector, size_t index, int element);
void VectorInt_append(VectorIntPtr vector, int element);





typedef struct VectorDouble *VectorDoublePtr;

VectorDoublePtr VectorDouble_new(void);
void VectorDouble_delete(VectorDoublePtr vector);

size_t VectorDouble_size(VectorDoublePtr vector);
double VectorDouble_get(VectorDoublePtr vector, size_t index);
void VectorDouble_set(VectorDoublePtr vector, size_t index, double element);
void VectorDouble_append(VectorDoublePtr vector, double element);





typedef struct VectorSJob *VectorSJobPtr;

VectorSJobPtr VectorSJob_new(void);
void VectorSJob_delete(VectorSJobPtr vector);

size_t VectorSJob_size(VectorSJobPtr vector);
SJobPtr VectorSJob_get(VectorSJobPtr vector, size_t index);
void VectorSJob_set(VectorSJobPtr vector, size_t index, SJobPtr element);
void VectorSJob_append(VectorSJobPtr vector, SJobPtr element);
SJobPtr VectorSJob_remove_last(VectorSJobPtr vector);

#endif /* VECTOR_H */