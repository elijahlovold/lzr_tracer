#ifndef _SD_REC
#define _SD_REC 1

#ifdef __cplusplus
extern "C" {
#endif

#define REC_LED 2 

typedef unsigned char l_state;

struct rec_pt {
    float th_x;
    float th_y;
    l_state laser_on;
};

int init_sd();
void close_sd();

int start_recording(unsigned int sample_rate, unsigned char f_num); 
int record_pt(const struct rec_pt* point, int num_points);
int end_recording(); 

int start_streaming(unsigned int* sample_rate, unsigned char f_num); 
int stream_pt(struct rec_pt* point);
int end_streaming(); 

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

