/**
 * @brief SCHC  Compression/Decompression implementation in C
 * @author Quentin Lampin (quentin.lampin@orange.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-08
 * 
 * @copyright Copyright (c) Orange 2023
 * 
 */
#include <inttypes.h>
#include <stdbool.h>

/**
 * @brief buffer padding, either left or right. 
 * 
 * A left-padded buffer has leading zeros for non-byte aligned content.
 * A right-padded buffer has trailing zeros for non-byte aligned content.
 * 
 */
typedef enum _padding {
    LEFT,
    RIGHT
} padding_t;

/**
 * @brief buffer structure
 * 
 * A buffer is the base data model for fields of length bits.
 * 
 */
typedef struct _buffer {
    unsigned char* content;
    uint16_t length;
    padding_t padding;
    uint8_t padding_length;
    uint16_t byte_length;
} buffer_t;

/**
 * @brief direction indicator enum
 * 
 * Indicates if the packet/field direction
 * - UP: from end-device endpoint to gateway endpoint
 * - DOWN: from gateway to end-device endpoint
 * - BIDIRECTIONAL: either way
 * 
 */
typedef enum _direction_indicator {
    UP, 
    DOWN,
    BIDIRECTIONAL
} direction_indicator_t;

/**
 * @brief Matching Operator (MO) enum
 * 
 * Indicates the matching operator of the rule:
 * - EQUAL: matches if the field value equals the target value
 * - IGNORE: matches irrespective of the field value
 * - MSB: matches if Most Significant Bits are the same (up to X bits, X being a parameter)
 * - MATCH_MAPPING: matches if the field value is included in the mapping valuesß
 * 
 */
typedef enum _matching_operator {
    EQUAL,
    IGNORE,
    MSB,
    MATCH_MAPPING
} matching_operator_t;

/**
 * @brief Compression Decompression Action (CDA) enum
 * 
 * Indicates the Compression and Decompression actions to apply if field matches
 * - NOT_SENT: field value is elided in the compressed packet (compatible with MOs: EQUAL, IGNORE)
 * - LSB: add Least Significant Bits to the compressed packet (compatible with MO: MSB)
 * - MAPPING_SENT: add the index of the field value in the mapping to the compressed packet (compatible with MO: MATCH_MAPPING)
 * - VALUE_SENT: add the field value (full extent) to the compressed packet (compatible MOs: IGNORE, EQUAL, MSB).
 */
typedef enum _compression_decompression_action {
    NOT_SENT,
    LSB,
    MAPPING_SENT,
    VALUE_SENT
} compression_decompression_action_t;

/**
 * @brief Mapping structure
 * 
 * contains a list of field values and their indices
 * 
 */
typedef struct _mapping {
    buffer_t** keys; // field values indices
    buffer_t** values; // field values
} mapping_t;

/**
 * @brief Target Value (TV)
 * 
 */
typedef union _target_value{
    buffer_t buffer; ///< value to match against a field
    mapping_t mapping; ///< mapping to matching against a field
} target_value_t;

/**
 * @brief Field Descriptor
 * 
 */
typedef struct _field_descriptor {
    const char* id; ///< name of the field
    buffer_t* buffer; ///< field value
} field_descriptor_t;

/**
 * @brief Header Descriptor
 * 
 */
typedef struct _header_descriptor {
    const 
char* id; ///< name of the protocol
    uint16_t length; ///< length of the header in bits
    uint8_t field_descriptors_count; ///< number of fields descriptors
    field_descriptor_t** field_descriptors; ///< list of pointers to the field descriptors
} header_descriptor_t;

/**
 * @brief Packet Descriptor
 * 
 */
typedef struct _packet_descriptor {
    direction_indicator_t direction; ///< packet direction: {UP, DOWN}
    field_descriptor_t** fields; ///< list of pointers to field descriptors
    buffer_t* payload; ///< pointer to a buffer containing the rest of the packet
    uint16_t length; ///< length of the header
} packet_descriptor_t;

/**
 * @brief 
 * 
 */
typedef struct _rule_field_descriptor {
    const char* id; ///< ID of the field matched by the field descriptor
    uint16_t length; ///< length (in bits) of the field to match
    direction_indicator_t direction; ///< direction of the field/packets to match
    target_value_t target_value; ///< target value of the MO and CDA 
    matching_operator_t matching_operator; ///< matching operator (MO)
    compression_decompression_action_t compression_decompression_action; ///< Compression Decompression Action (CDA)
} rule_field_descriptor_t;

typedef struct _rule_descriptor {
    buffer_t* id; ///< rule ID, to be included in the compressed packet when applied
    rule_field_descriptor_t** field_descriptors; ///< list of pointers to rules' field descriptors
} rule_descriptor_t;

/**
 * @brief SCHC Context
 * 
 * In RFC 8724, there is no formal definition of a SCHC context. 
 * This is a first attempt at defining it.
 * 
 */
typedef struct _context {
    const char* id; ///< ID, not sure if that's necessary
    char* description; ///< SCHC context description
    char* interface_id; ///< network interface ID
    rule_descriptor_t** ruleset; ///< list of pointers to rule descriptors
} context_t;

buffer_t *buffer_init(const unsigned char* content, uint16_t length, padding_t padding);
buffer_t* buffer_extract(buffer_t* buffer, uint16_t bit_start, uint16_t bit_length, padding_t padding);
buffer_t* buffer_shift(buffer_t* buffer, int shift);
buffer_t* buffer_pad(buffer_t* buffer, padding_t padding);
buffer_t* buffer_concat(buffer_t* buffer_1, buffer_t* buffer_2);
bool buffer_equal(buffer_t* buffer_1, buffer_t* buffer_2, int length);
void buffer_deinit(buffer_t* buffer);

field_descriptor_t* field_descriptor_init(const char* id, buffer_t* buffer);
void field_descriptor_deinit(field_descriptor_t* field_descriptor);

header_descriptor_t* header_descriptor_malloc(int field_descriptors_count);
void header_descriptor_free(header_descriptor_t* header_descriptor);
header_descriptor_t* header_descriptor_init(const char* id, int field_descriptors_count, ...);
void header_descriptor_deinit(header_descriptor_t* header_descriptor);
header_descriptor_t* header_descriptor_concat(const char* id, int header_descriptors_count, ...);

