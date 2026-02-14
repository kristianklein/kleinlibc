#include "kleinlibc/framing.h"

#define HDLC_FLAG 0x7EU
#define HDLC_ESCAPE 0x7DU
#define HDLC_XOR 0x20U

size_t frame_hdlc(uint8_t *out, size_t out_size, const uint8_t *in, size_t in_size)
{
    // TODO: Add out_size checks to avoid writing outside buffer
    size_t index_in = 0;
    size_t index_out = 0;

    out[index_out++] = HDLC_FLAG;

    for (index_in = 0; index_in < in_size; index_in++)
    {
        if (in[index_in] == HDLC_FLAG || in[index_in] == HDLC_ESCAPE)
        {
            out[index_out++] = HDLC_ESCAPE;
            out[index_out++] = in[index_in] ^ HDLC_XOR;
        }
        else
        {
            out[index_out++] = in[index_in];
        }
    }

    out[index_out++] = HDLC_FLAG;

    return index_out;
}

size_t deframe_hdlc(uint8_t *out, size_t out_size, const uint8_t *in, size_t in_size)
{
    size_t index_in = 0;
    size_t index_out = 0;

    for (index_in = 0; index_in < in_size; index_in++)
    {
        if (in[index_in] == HDLC_FLAG)
        {
            continue;
        }
        else if (in[index_in] == HDLC_ESCAPE)
        {
            index_in++;
            out[index_out++] = in[index_in] ^ HDLC_XOR;
        }
        else
        {
            out[index_out++] = in[index_in];
        }
    }

    return index_out;
}