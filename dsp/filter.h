/*

Implement functions to apply filter coefficients to a sampled signal using e.g. Direct Form I/II, biquad cascade, etc.


Direct form I (FIR):
y[n] = h[0]x[n] + h[1]x[n − 1] + · · · + h[N]x[n − N]
     = SUM for k=0 to N of h[k]x[k − n]

For efficiency, take advantage of the linear phase characteristic of the FIR (symmetric coefficients) to cut number of multiplications
in half, i.e. y[n] = h[0]*(x[n] + x[n-N]) + h[1](x[n-1] + x[n-(N-1)]) ... (is this correct?)

     
Higher order IIR filters may be sensitive to rounding errors in coefficients, when poles and zeros are either close to each other
or close to the unit circle. Splitting the filter into cascaded second-order sections (biquads) and chaining them together
can make the filter much more stable. They can then be efficiently realized with Direct form II transposed. Cascaded biquads and
DF2T is the de facto standard for IIR filters in embedded DSP applications.

*/