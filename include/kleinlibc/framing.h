#ifndef KLEINLIBC_PACKET_ENCODING_H
#define KLEINLIBC_PACKET_ENCODING_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Apply High-level Data Link Control (HDLC) style byte-stuffing to a byte stream
 *
 * The data is framed with a 0x7E byte at the end and beginning.
 * Any byte in the actual data that has a value of 0x7E must therefore be escaped,
 * so as to not be mistaken for a frame marker. A byte of 0x7E is converted to 0x7D 0x5E.
 * Now the escape character 0x7D must also be escaped, so it becomes 0x7D 0x5D.
 *
 * This means that the length of the byte-stuffed output array can be up to 2 * input_length + 2 bytes,
 * in the worst case where all data bytes must be escaped.
 *
 * @param out
 * @param out_size
 * @param in
 * @param in_size
 * @return size_t
 */
size_t frame_hdlc(uint8_t *out, size_t out_size, const uint8_t *in, size_t in_size);

/* It is impractical for the receiving code to be aware of which framing is being used.
 * Instead, the deframe functionality could be a state machine, that consumes any amount
 * of received bytes and signals (e.g. via callback or pushing to a queue) when a full
 * packet has been decoded.
 * Or just keep this function simple, and have another module receive bytes and look
 * for frame delimiters.
 */
size_t deframe_hdlc(uint8_t *out, size_t out_size, const uint8_t *in, size_t in_size);


#endif