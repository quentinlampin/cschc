#include <stdlib.h>


#include "cschc.h"

field_descriptor_t* field_descriptor_malloc(){
    field_descriptor_t* field_descriptor;

    field_descriptor = (field_descriptor_t*)malloc(sizeof(field_descriptor_t));
    return field_descriptor;
}

field_descriptor_t* field_descriptor_init(const char* id, buffer_t* buffer){
    field_descriptor_t* field_descriptor;

    field_descriptor = field_descriptor_malloc();
    field_descriptor->id = id;
    field_descriptor->buffer = buffer;

    return field_descriptor;
}

void field_descriptor_deinit(field_descriptor_t* field_descriptor){
    buffer_deinit(field_descriptor->buffer);
    free(field_descriptor);
}