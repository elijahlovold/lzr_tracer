#ifndef _SD_REC
#define _SD_REC 1

#ifdef __cplusplus
extern "C" {
#endif

#include <utils.h>

#define REC_LED 4 
#define STR_LED 5 

int init_sd();
void close_sd();

int start_recording(unsigned int sample_rate, unsigned char f_num); 
int record_pt(const struct lzr_state* point, int num_points);
int end_recording(); 
uint8_t recording_active();

int start_streaming(unsigned int* sample_rate, unsigned char f_num); 
int stream_pt(struct lzr_state* point);
int end_streaming(); 
uint8_t stream_active();

// ---- file format ---- 
// uint32_t SAMPLE_RATE
// 
// - beginning of data block -
// rec_struct data[0]
// rec_struct data[1]
// ...
// rec_struct data[NUM_SAMPLES-1]
// 
// - eof -

#ifdef __cplusplus
}
#endif

#endif

