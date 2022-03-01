#ifndef VECTOR_H
#define VECTOR_H 1

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

#endif /* VECTOR_H */