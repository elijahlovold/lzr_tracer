#include <sd_rec.h>

#include <stdio.h>

#include "pico/stdlib.h"
#include "f_util.h"
#include "ff.h"

#include "hw_config.h"

sd_card_t *pSD = NULL;

FIL open_file;
int file_open = 0; 

// -------------- file open/close functions --------------
int close_sd_file() {
    FRESULT fr = f_close(&open_file);

    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
        return 0;
    }
    file_open = 0;
    gpio_put(REC_LED, 0);
    printf("f_close success\n");
    return 1;
}

int open_sd_file(unsigned char f_num, unsigned char mode) {
    if (file_open) close_sd_file();

    char filename[] = "rec_entry_0.raw";
    filename[10] = (char)('0' + f_num);
    FRESULT fr = f_open(&open_file, filename, mode);
    if (FR_OK != fr && FR_EXIST != fr) {
        printf("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
        return 0;
    }
    printf("f_open(%s) opened successfully\n", filename);
    file_open = 1;
    return 1;
}
// -------------------------------------------------------


int init_sd() {
    gpio_init(REC_LED);
    gpio_set_dir(REC_LED, GPIO_OUT);
    gpio_put(REC_LED, 0);

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    pSD = sd_get_by_num(0);

    FRESULT fr = f_mount(&(pSD->fatfs), pSD->pcName, 1);

    // f_mount error: A hard error occurred in the low level disk I/O layer (1)
    if (FR_OK != fr) {
        printf("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
        return 0;
    }
    printf("sd card opened successfully!\n");
    return 1;
}

void close_sd() {
    close_sd_file();
    f_unmount(pSD->pcName);
}

int start_recording(unsigned int sample_rate, unsigned char f_num) {
    if (f_num > 9) f_num = 9;
    if (!open_sd_file(f_num, FA_WRITE | FA_CREATE_ALWAYS)) return 0; 

    unsigned int bytes_written;
    FRESULT fr = f_write(&open_file, &sample_rate, sizeof(unsigned int), &bytes_written);
    if (FR_OK != fr || bytes_written == 0) {
        printf("error writting to file\n");
        return 0;
    }

    gpio_put(REC_LED, 1);
    printf("streaming ready\n");
    return 1;
}

int record_pt(const struct rec_pt* point, int num_points) {
    if (!file_open) return 0;

    unsigned int bytes_written;
    FRESULT fr = f_write(&open_file, point, sizeof(struct rec_pt)*num_points, &bytes_written);
    if (FR_OK != fr || bytes_written == 0) {
        printf("error writting to file\n");
        return 0;
    }
    return 1;
}

int end_recording() {
    return close_sd_file();
}


int start_streaming(unsigned int* sample_rate, unsigned char f_num) {
    if (f_num > 9) f_num = 9;
    if (!open_sd_file(f_num, FA_READ)) return 0; 

    unsigned int bytes_read;
    FRESULT fr = f_read(&open_file, sample_rate, sizeof(unsigned int), &bytes_read);
    if (FR_OK != fr || bytes_read == 0) {
        printf("error reading file\n");
        return 0;
    }

    printf("recording ready\n");
    return 1;
}

int stream_pt(struct rec_pt* point) {
    if (!file_open) return 0;

    unsigned int bytes_read;
    FRESULT fr = f_read(&open_file, point, sizeof(struct rec_pt), &bytes_read);
    if (FR_OK != fr || bytes_read == 0) {
        printf("error reading file\n");
        return 0;
    }
    return 1;
}

int end_streaming() {
    return close_sd_file();
}
