[![build](https://github.com/quentinlampin/cschc/actions/workflows/cmake.yml/badge.svg)](https://github.com/quentinlampin/cschc/actions/workflows/cmake.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
# CSCHC - Implementation of Static Context Header Compression (SCHC), RFC 8724

## In a nutshell

C implementation of Static Context Header Compression (SCHC), RFC 8724.

## License

MIT License, Copyright (c) 2022-2024 Orange, by Quentin Lampin and Corentin Banier.

## Build

Please refer to [BUILD.md](./BUILD.md).

## Usage

The main [CMakeLists.txt](./CMakeLists.txt) generates `libcschc`, a `main` executable and potentially, depending on the build variants used, the test executables.

### CSCHC Context

CSCHC works with a specific Context format for SCHC, which is represented by a well-defined byte array. The Context structure changes from the RFC definition as we avoid redundancy. As we try to achieve the most efficiency in terms of memory manipulation, this part is very important.

The CSCHC Context structure is explained with four structures: A Context, some Rule Descriptors, some Rule Field Descriptors, and some Target Values. The difference lies in the way we handle them. Indeed, we use an offset logic to associate each element. Each property defined in RFC 8724 is conserved, but instead of storing the same object, we choose to store its offset. One specification is that an offset is defined on 2 bytes. Indeed, one byte is not enough as 255 can be reached easily.

__Important: In CSCHC, the compression is processed using only the Context. No parsing logic is defined. Therefore, we try to compress a packet by running through each Rule Descriptor until we reach a corresponding one or, in the worst case, we reach the no-compression rule. This means that Rule Descriptors have to be defined in the correct order. The no-compression rule has to be the last one. The first Rule Descriptor should be the most constrained one, with the highest compression rate, for example.__

- A CSCHC Context refers to an ID, a number of Rule Descriptors, and the list of their offsets : 
    ```txt
    // Context
    ID, CARD_RULE_DESCRIPTOR, 
    LEFT_OFFSET_RULE_DESCRIPTOR_0, RIGHT_OFFSET_RULE_DESCRIPTOR_0, 
    ..., 
    LEFT_OFFSET_RULE_DESCRIPTOR_N, RIGHT_OFFSET_RULE_DESCRIPTOR_N
    ```

    Example :
    ```txt
    // Context
    0, 3, 0, 6, 0, 10, 0, 40, ...
    ```
    This CSCHC Context has an ID of 0. It contains 3 Rule Descriptors. The first Rule Descriptor position is 6, the second is 10, and the last one is 40.

- We apply the same logic for Rule Descriptor and Rule Field Descriptor :
    ```txt
    // Rule Descriptor
    ID, RULE_NATURE, CARD_RULE_FIELD_DESCRIPTOR,
    LEFT_OFFSET_RULE_FIELD_DESCRIPTOR_0, RIGHT_OFFSET_RULE_FIELD_DESCRIPTOR_0, 
    ..., 
    LEFT_OFFSET_RULE_FIELD_DESCRIPTOR_N, RIGHT_OFFSET_RULE_FIELD_DESCRIPTOR_N
    ```

    Example :
    ```txt
    // Rule Descriptor
    0, 0, 1, 0, 17,
    1, 0, 3, 0, 28, 0, 17, 0, 45,
    2, 1, 0, ...
    ```
    In this example, we can see three Rule Descriptors. The first two have a compression nature (0) and the last one is for no-compression. There is only one Rule Field Descriptor in the first Rule Descriptor but 3 in the second Rule Field Descriptor. You can notice that the offset 17 appears in the first Rule Descriptors. This means they have it in common.

- For the Rule Field Descriptors, we decided to add one more field which refers to the length of an MSB matching operator. Indeed, without adding this information, the system cannot determine it by itself. Additionally, Direction Indicator, Matching Operator, and Compression Decompression Actions have been packed into only one byte as this information in total only requires 7 bits. Then, we used SIDs (not standardized yet) instead of Field-ID.

    Here is the general structure :
    ```txt
    LEFT_SID, RIGHT_SID, LEFT_LEN, RIGHT_LEN, LEFT_POS, RIGHT_POS, 
    DIR_MO_CDA, CARD_TARGET_VALUE, 
    LEFT_OFFSET_TARGET_VALUE_0, RIGHT_OFFSET_TARGET_VALUE_0, 
    ..., 
    LEFT_OFFSET_TARGET_VALUE_N, RIGHT_OFFSET_TARGET_VALUE_N
    ```

    When MSB/LSB is needed :
    ```txt
    LEFT_SID, RIGHT_SID, LEFT_LEN, RIGHT_LEN, LEFT_POS, RIGHT_POS, 
    DIR_MO_CDA, LEFT_MSB_LEN, RIGHT_MSB_LEN, CARD_TARGET_VALUE, 
    LEFT_OFFSET_TARGET_VALUE_0, RIGHT_OFFSET_TARGET_VALUE_0, 
    ..., 
    LEFT_OFFSET_TARGET_VALUE_N, RIGHT_OFFSET_TARGET_VALUE_N
    ```

- Finally, the Target Values. We only concatenate them at the end of the CSCHC Context as you know from the Rule Field Descriptor which association has been done.


PS :
1. When `CARD_...` is 0, no offsets are defined.
2. You can find a complete example in [main.c](./source/main.c) or in test files.

### Memory

One of the goals of CSCHC is to provide SCHC for embedded software, so this program uses the concept of a memory pool. The memory pool is responsible for handling various structures during compression and decompression. Users are also invited to use it, as you can allocate resources from the pool to handle packets. The pool size is determined in [memory.h](./include/utils/memory.h) but can be adjusted using a flag during compilation time.

This `memory_pool_t` implementation is not fragmentation-friendly. Allocation and deallocation must be performed in the correct order to avoid this effect. The internal logic is verified, but the problem could appear if a user wants to allocate or deallocate structures by themselves without checking the order.

More details can be found in the following section.

### Main

The [main.c](./source/main.c) and therefore the `main` executable provide an example that summarizes all the previous items. You can find an example of compression and decompression.

## Tool

To create a CSCHC Context, you can refer to the section `CSCHC Context Builder` of the following project: [cschc_context](https://github.com/cbanier/cschc_context).

_NB: The context used in [main.c](./source/main.c) corresponds to the context defined in [cschc_context](https://github.com/cbanier/cschc_context)._