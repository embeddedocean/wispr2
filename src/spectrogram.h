/*
 * Spectrogram header file
 * ------
 * THIS SOFTWARE IS PROVIDED BY EOS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL EOS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Embedded Ocean Systems (EOS), 2015
 *
 */
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "arm_math.h"
#include "arm_const_structs.h"

#define HAMMING_WINDOW 1
#define HANN_WINDOW 2
#define BLACKMAN_WINDOW 3
 
#define MAX_FFT_SIZE 1024

extern int spectrum_init_f32(uint16_t size, uint8_t wintype);
extern int spectrum_f32(uint8_t *input, float32_t *output, uint16_t nsamps, uint16_t overlap);
extern int spectrum_init_q31(uint16_t size, uint8_t wintype);
extern int spectrum_q31(uint8_t *input, q31_t *output, uint16_t nsamps, uint16_t overlap);
extern void spectrum_window(float32_t *w, uint8_t type, uint16_t size);


/*
typedef struct {
	uint16_t nfft;      // size of fft
	uint16_t overlap;   // data overlap for each fft
	uint16_t navg;      // num of time bins to average
	uint32_t fs;        // sampling freq [hz]
	float32_t dtime;        // size of time bin in seconds.
	float32_t dfreq;        // size of frequency bins in Hz.
	float32_t *magnitude;
	float32_t *window;
	uint16_t num_time_bins;  // ((nsamps - nfft)/(nfft - overlap)) / navg
	uint16_t num_freq_bins;  // nfft/2 +1
} spectrum_f32_t;

typedef struct {
	uint16_t nfft;      // size of fft
	uint16_t overlap;   // data overlap for each fft
	uint16_t navg;      // num of time bins to average
	uint32_t fs;        // sampling freq [hz]
	float32_t dtime;        // size of time bin in seconds.
	float32_t dfreq;        // size of frequency bins in Hz.
	uint16_t num_time_bins;  // ((nsamps - nfft)/(nfft - overlap)) / navg
	uint16_t num_freq_bins;  // nfft/2 +1
} spectrogram_q31_t;
*/
//extern int spectrogram_f32(uint8_t *input, float32_t *output, uint16_t nsamps, uint16_t overlap);
//extern int spectrogram_q31(uint8_t *input, int32_t *output, uint16_t nsamps, uint16_t overlap);

//extern void spectrogram_clear(spectrogram_f32_t *psd);

#endif /* _PSD_H */

