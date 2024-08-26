#include "core/compression.h"
#include "core/context.h"
#include "core/decompression.h"
#include "utils/log.h"
#include "utils/memory.h"

#include <stdio.h>
#include <string.h>

/* ********************************************************************** */

void compression_usage(const uint8_t *context, const size_t context_byte_len) {
  /**
   * @brief This Packet should match the Rule Descriptor 0.
   */
  const uint8_t packet[] = {
      0x6f, 0xff, 0xf8, 0x5f, 0x00, 0x38, 0x11, 0x40, 0x20, 0x01, 0x0d, 0xb8,
      0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
      0x20, 0x01, 0x0d, 0xb8, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x20, 0xd1, 0x00, 0x16, 0x33, 0x00, 0x38, 0x1b, 0xe9,
      0x48, 0x02, 0x84, 0x82, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
      0xb2, 0x56, 0x34, 0x33, 0x62, 0x3d, 0x55, 0x0d, 0x02, 0x0a, 0x0b, 0x0c,
      0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
      0xd2, 0x14, 0xab, 0xef, 0xff, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64};
  const size_t packet_size = sizeof(packet);

  printf("=========== COMPRESSION EXAMPLE ===========\n");
  printf("Initial Pkt is : ");
  print_hex_packet(packet, packet_size, 0);

  /**
   * @brief Allocate the SCHC Packet.
   *
   * @details As the compression can mismatch all the rules, the packet can be
   * finally "compressed" by the no-compression rule. In this case, the final
   * size of the SCHC packet will be the size of the initial packet + 1 due to
   * the Rule ID used to compress the packet.
   */
  uint8_t *cschc_packet = (uint8_t *) pool_alloc(packet_size + 1);

  /**
   * @brief Perform CSCHC Compression.
   *
   * @details The Direction Indicator is set to UP. Indeed :
   * - IPv6 Source Address corresponds to 2001:0db8:000a::0003
   * - IPv6 Destination Address corresponds to 2001:0db8:000a::0020
   * - UDP Source Port corresponds to 53504
   * - UDP Destination Port corresponds to 5683
   * This description corresponds to the UP definition for these fields.
   *
   * Additionally, other fields correspond to the Rule Descriptor 0.
   */
  size_t cschc_packet_size =
      compress(cschc_packet, packet_size + 1, DI_UP, packet, packet_size,
               context, context_byte_len);
  printf("DI_UP compression :\n");
  printf("  CSCHC Pkt is : ");
  print_hex_packet(cschc_packet, cschc_packet_size, 0);

  /**
   * @brief Perform CSCHC Compression
   *
   * @details Now, if we try to compress the last packet with DOWN Direction
   * Indicator, the no-compression rule will be reached.
   */
  cschc_packet_size = compress(cschc_packet, packet_size + 1, DI_DW, packet,
                               packet_size, context, context_byte_len);
  printf("DI_DW compression :\n");
  printf("  CSCHC Pkt is : ");
  print_hex_packet(cschc_packet, cschc_packet_size, 0);

  printf("===========================================\n\n");

  /**
   * @brief Do not forget to deallocate cschc_packet from the pool.
   */
  pool_dealloc(cschc_packet, packet_size + 1);
}

/* ********************************************************************** */

void decompression_usage(const uint8_t *context,
                         const size_t   context_byte_len) {
  const uint8_t cschc_packet[] = {
      0x40, 0x98, 0x00, 0x3f, 0xa0, 0x00, 0x81, 0x01, 0x82, 0x02, 0x83,
      0x03, 0x84, 0x78, 0x82, 0xb1, 0xa1, 0xef, 0x01, 0x41, 0x61, 0x81,
      0xa1, 0xc1, 0xe2, 0x02, 0x22, 0x42, 0x62, 0x82, 0xa2, 0xc2, 0xe3,
      0x1e, 0x21, 0x57, 0xde, 0xe0, 0xc2, 0xf2, 0xd8, 0xde, 0xc2, 0xc8};
  const size_t cschc_packet_size = sizeof(cschc_packet);

  printf("========== DECOMPRESSION EXAMPLE ==========\n");
  printf("  CSCHC Pkt is : ");
  print_hex_packet(cschc_packet, cschc_packet_size, 0);

  /**
   * @brief Allocate the Packet.
   */
  uint8_t *packet = (uint8_t *) pool_alloc(100);

  /**
   * @brief Perform CSCHC Decompression.
   *
   * @details 5 Rule Descriptors are defined in the Context, meaning that 3 bits
   * are necessary to encode the Rule ID. So the first 3 bits of the compressed
   * packet are 010 = 2. Therefore, the Rule Descriptor to use is 2.
   */
  size_t packet_size = decompress(packet, 100, DI_UP, cschc_packet,
                                  cschc_packet_size, context, context_byte_len);
  printf(" Decomp Pkt is : ");
  print_hex_packet(packet, packet_size, 0);

  printf("===========================================\n\n");

  /**
   * @brief Do not forget to deallocate packet from the pool.
   */
  pool_dealloc(packet, 100);
}

/* ********************************************************************** */

int main(void) {
  /**
   * @brief Construct a new memory pool.
   *
   * @details The size of the pool is defined by POOL_SIZE in
   * include/utils/memory.h. You only need to initialize the pool at the
   * beginning of your program. As the pool is extern, you can access it
   * throughout all libcschc.
   */
  init_memory_pool();

  /**
   * @brief Create a CSCHC Context.
   *
   * @details 5 Rule Descriptors and 47 Rule Field Descriptors.
   * Rule Descriptor 0 : Maximal compression expected for a packet here.
   *            - equal/not-sent :
   *                ~ IPv6 : Version, Next Header, Hop Limit, Source Address,
   *                         Destination Address
   *                ~ UDP  : Application Port, Device Port
   *                ~ CoAP : Version, Type, Token Length, Code, Option Delta,
   *                         Option Length, Option Length Extended,
   *                         Option Delta Extended, Payload Marker
   *
   *            - match-mapping/mapping-sent :
   *                ~ IPv6 : Traffic Class, Flow Label
   *                ~ CoAP : Message ID,
   *
   *            - ignore/compute :
   *                ~ IPv6 : Payload Length
   *                ~ UDP  : Length, Checksum
   *
   *            - ignore/value_sent :
   *                ~ CoAP : Token, Option Value
   *
   * Rule Descriptor 1 :
   *      Reduce compression by changing :
   *            - IPv6 Traffic Class match-mapping/mapping-sent by MSB/LSB
   *            - CoAP MID match-mapping/mapping-sent by MSB/LSB
   *
   * Rule Descriptor 2 :
   *      Reduce compression by changing :
   *            - IPv6 Traffic Class MSB/LSB by ignore/value-sent
   *            - CoAP MID MSB/LSB by ignore/value-sent
   *
   * Rule Descriptor 3 :
   *      This Rule compress only IPv6 Version, Addresses and UDP Ports.
   *
   * Rule Descriptor 4 : No-compression rule.
   *
   * @details You can allocate the Context in the pool. But you have to allocate
   * it first. In this main example case, as the Context is not random and as we
   * need to fill it, we need to define it on the stack anyway. So this is
   * useless to reserve it from the pool. To allocate the Context : uint8_t
   * *context = (uint8_t*)pool_alloc(SIZE_CONTEXT);
   *
   * @details See https://github.com/cbanier/cschc_context.git. The following
   * context is equivalent to
   * https://github.com/cbanier/cschc_context/blob/master/data/example.json
   */
  const uint8_t context[] = {
      // Context
      0, 5, 0, 12, 0, 89, 0, 166, 0, 243, 1, 64,

      // Rule Descriptors
      0, 0, 37, 1, 67, 1, 77, 1, 93, 1, 109, 1, 117, 1, 127, 1, 137, 1, 147, 1,
      157, 1, 167, 1, 177, 1, 187, 1, 197, 1, 207, 1, 217, 1, 225, 1, 233, 1,
      243, 1, 253, 2, 7, 2, 17, 2, 37, 2, 45, 2, 55, 2, 65, 2, 73, 2, 83, 2, 93,
      2, 107, 2, 117, 2, 127, 2, 65, 2, 137, 2, 55, 2, 147, 2, 65, 2,
      157,  // Rule Descriptor n° 0
      1, 0, 37, 1, 67, 2, 167, 1, 93, 1, 109, 1, 117, 1, 127, 1, 137, 1, 147, 1,
      157, 1, 167, 1, 177, 1, 187, 1, 197, 1, 207, 1, 217, 1, 225, 1, 233, 1,
      243, 1, 253, 2, 7, 2, 179, 2, 37, 2, 45, 2, 55, 2, 65, 2, 73, 2, 83, 2,
      93, 2, 107, 2, 117, 2, 127, 2, 65, 2, 137, 2, 55, 2, 147, 2, 65, 2,
      157,  // Rule Descriptor n° 1
      2, 0, 37, 1, 67, 2, 191, 1, 93, 1, 109, 1, 117, 1, 127, 1, 137, 1, 147, 1,
      157, 1, 167, 1, 177, 1, 187, 1, 197, 1, 207, 1, 217, 1, 225, 1, 233, 1,
      243, 1, 253, 2, 7, 2, 199, 2, 37, 2, 45, 2, 55, 2, 65, 2, 73, 2, 83, 2,
      93, 2, 107, 2, 117, 2, 127, 2, 65, 2, 137, 2, 55, 2, 147, 2, 65, 2,
      157,  // Rule Descriptor n° 2
      3, 0, 37, 1, 67, 2, 191, 2, 207, 1, 109, 1, 117, 1, 127, 1, 137, 1, 147,
      1, 157, 1, 167, 1, 177, 1, 187, 1, 197, 1, 207, 1, 217, 1, 225, 2, 215, 2,
      223, 2, 231, 2, 239, 2, 199, 2, 37, 2, 247, 2, 255, 2, 65, 2, 247, 2, 255,
      2, 65, 2, 247, 2, 255, 3, 7, 2, 65, 2, 247, 2, 255, 3, 15, 2, 65, 2,
      157,      // Rule Descriptor n° 3
      4, 1, 0,  // Rule Descriptor n° 4

      // Rule Field Descriptors
      0x13, 0xcc, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x17,  // Rule Field Descriptor n° 0
      0x13, 0xc9, 0x0, 0x8, 0x0, 0x1, 0x5a, 0x4, 0x3, 0x18, 0x3, 0x19, 0x3,
      0x1a, 0x3, 0x1b,  // Rule Field Descriptor n° 1
      0x13, 0xc5, 0x0, 0x14, 0x0, 0x1, 0x5a, 0x4, 0x3, 0x1c, 0x3, 0x1f, 0x3,
      0x22, 0x3, 0x25,  // Rule Field Descriptor n° 2
      0x13, 0xc8, 0x0, 0x10, 0x0, 0x1, 0x4c, 0x0,  // Rule Field Descriptor n° 3
      0x13, 0xc7, 0x0, 0x8, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x28,  // Rule Field Descriptor n° 4
      0x13, 0xc6, 0x0, 0x8, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x29,  // Rule Field Descriptor n° 5
      0x13, 0xc1, 0x0, 0x80, 0x0, 0x1, 0x0, 0x1, 0x3,
      0x2a,  // Rule Field Descriptor n° 6
      0x13, 0xc1, 0x0, 0x80, 0x0, 0x1, 0x20, 0x1, 0x3,
      0x3a,  // Rule Field Descriptor n° 7
      0x13, 0xc4, 0x0, 0x80, 0x0, 0x1, 0x0, 0x1, 0x3,
      0x3a,  // Rule Field Descriptor n° 8
      0x13, 0xc4, 0x0, 0x80, 0x0, 0x1, 0x20, 0x1, 0x3,
      0x2a,  // Rule Field Descriptor n° 9
      0x13, 0xce, 0x0, 0x10, 0x0, 0x1, 0x0, 0x1, 0x3,
      0x4a,  // Rule Field Descriptor n° 10
      0x13, 0xce, 0x0, 0x10, 0x0, 0x1, 0x20, 0x1, 0x3,
      0x4c,  // Rule Field Descriptor n° 11
      0x13, 0xd1, 0x0, 0x10, 0x0, 0x1, 0x0, 0x1, 0x3,
      0x4c,  // Rule Field Descriptor n° 12
      0x13, 0xd1, 0x0, 0x10, 0x0, 0x1, 0x20, 0x1, 0x3,
      0x4a,  // Rule Field Descriptor n° 13
      0x13, 0xd2, 0x0, 0x10, 0x0, 0x1, 0x4c,
      0x0,  // Rule Field Descriptor n° 14
      0x13, 0xd0, 0x0, 0x10, 0x0, 0x1, 0x4c,
      0x0,  // Rule Field Descriptor n° 15
      0x13, 0xbf, 0x0, 0x2, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x4e,  // Rule Field Descriptor n° 16
      0x13, 0xbe, 0x0, 0x2, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x4f,  // Rule Field Descriptor n° 17
      0x13, 0xbc, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x50,  // Rule Field Descriptor n° 18
      0x13, 0x9f, 0x0, 0x8, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x51,  // Rule Field Descriptor n° 19
      0x13, 0xa2, 0x0, 0x10, 0x0, 0x1, 0x5a, 0x6, 0x3, 0x52, 0x3, 0x54, 0x3,
      0x56, 0x3, 0x58, 0x3, 0x5a, 0x3, 0x5c,  // Rule Field Descriptor n° 20
      0x13, 0xbd, 0x0, 0x0, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 21
      0x14, 0x10, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x5e,  // Rule Field Descriptor n° 22
      0x14, 0x12, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x5f,  // Rule Field Descriptor n° 23
      0x14, 0x14, 0x0, 0x0, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 24
      0x14, 0x10, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x60,  // Rule Field Descriptor n° 25
      0x14, 0x12, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x60,  // Rule Field Descriptor n° 26
      0x14, 0x14, 0x0, 0x0, 0x0, 0x1, 0x5a, 0x3, 0x3, 0x61, 0x3, 0x64, 0x3,
      0x67,  // Rule Field Descriptor n° 27
      0x14, 0x10, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x6a,  // Rule Field Descriptor n° 28
      0x14, 0x12, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x6b,  // Rule Field Descriptor n° 29
      0x14, 0x13, 0x0, 0x0, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x51,  // Rule Field Descriptor n° 30
      0x14, 0x10, 0x0, 0x4, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x6b,  // Rule Field Descriptor n° 31
      0x14, 0x11, 0x0, 0x0, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x6c,  // Rule Field Descriptor n° 32
      0x14, 0x15, 0x0, 0x8, 0x0, 0x1, 0x40, 0x1, 0x3,
      0x18,  // Rule Field Descriptor n° 33
      0x13, 0xc9, 0x0, 0x8, 0x0, 0x1, 0x51, 0x0, 0x4, 0x1, 0x3,
      0x6d,  // Rule Field Descriptor n° 34
      0x13, 0xa2, 0x0, 0x10, 0x0, 0x1, 0x51, 0x0, 0xa, 0x1, 0x3,
      0x6e,  // Rule Field Descriptor n° 35
      0x13, 0xc9, 0x0, 0x8, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 36
      0x13, 0xa2, 0x0, 0x10, 0x0, 0x1, 0x4b,
      0x0,  // Rule Field Descriptor n° 37
      0x13, 0xc5, 0x0, 0x14, 0x0, 0x1, 0x4b,
      0x0,  // Rule Field Descriptor n° 38
      0x13, 0xbf, 0x0, 0x2, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 39
      0x13, 0xbe, 0x0, 0x2, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 40
      0x13, 0xbc, 0x0, 0x4, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 41
      0x13, 0x9f, 0x0, 0x8, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 42
      0x14, 0x10, 0x0, 0x4, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 43
      0x14, 0x12, 0x0, 0x4, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 44
      0x14, 0x13, 0x0, 0x0, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 45
      0x14, 0x11, 0x0, 0x0, 0x0, 0x1, 0x4b, 0x0,  // Rule Field Descriptor n° 46

      // Target Values
      0x6, 0xff, 0xfe, 0xf1, 0xf7, 0x0, 0xef, 0x2d, 0xf, 0xfe, 0x2d, 0x7, 0x77,
      0x77, 0xf, 0xf8, 0x5f, 0x11, 0x40, 0x20, 0x1, 0xd, 0xb8, 0x0, 0xa, 0x0,
      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x20, 0x1, 0xd, 0xb8, 0x0,
      0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0xd1, 0x0, 0x16,
      0x33, 0x1, 0x0, 0x8, 0x2, 0x84, 0x81, 0x84, 0x82, 0x84, 0x83, 0x84, 0x84,
      0x84, 0x85, 0x84, 0x86, 0xb, 0x2, 0x3, 0x62, 0x3d, 0x55, 0xab, 0xcd, 0xef,
      0x77, 0x0, 0xff, 0x0, 0xd, 0x14, 0xf, 0x2, 0x12};

  const size_t context_byte_len = sizeof(context);

  // CSCHC Compression Usage
  compression_usage(context, context_byte_len);

  // CSCHC Decompression Usage
  decompression_usage(context, context_byte_len);

  // At the end of the program, you need to destroy the pool as a single malloc
  // is performed during the initialization.
  destroy_memory_pool();

  return 0;
}