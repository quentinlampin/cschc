#include <stdarg.h>
#include <stdlib.h>
#include "cschc.h"

header_descriptor_t* header_descriptor_malloc(int field_descriptors_count){
    header_descriptor_t* header_descriptor;

    header_descriptor = (header_descriptor_t*)malloc(sizeof(header_descriptor_t));
    header_descriptor->field_descriptors = (field_descriptor_t**)malloc(field_descriptors_count * sizeof(field_descriptor_t*));
    return header_descriptor;
}

header_descriptor_t* header_descriptor_init(const char* id, int field_descriptors_count, ...){
    va_list field_descriptor_pointer;
    header_descriptor_t* header_descriptor;
    field_descriptor_t* fd_iter;
    int va_index;

    header_descriptor = header_descriptor_malloc(field_descriptors_count);
    header_descriptor->id = id;
    header_descriptor->length = 0;
    header_descriptor->field_descriptors_count = field_descriptors_count;
    
    va_start(field_descriptor_pointer, field_descriptors_count);
    for(va_index=0; va_index < field_descriptors_count; va_index++){
        fd_iter = va_arg(field_descriptor_pointer, field_descriptor_t*);
        header_descriptor->field_descriptors[va_index] = fd_iter;
        header_descriptor->length += fd_iter->buffer->length;
    }
    va_end(field_descriptor_pointer);

    return header_descriptor;
}

void header_descriptor_deinit(header_descriptor_t* header_descriptor){
    free(header_descriptor->field_descriptors);
    free(header_descriptor);
}
