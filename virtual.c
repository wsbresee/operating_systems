#include "oslabs.h"

int process_page_access_fifo(
        struct PTE page_table[TABLEMAX],
        int *table_cnt,
        int page_number,
        int frame_pool[POOLMAX],
        int *frame_cnt,
        int current_timestamp) {
    if (page_table[page_number].is_valid) {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    if (*frame_cnt > 0) {
        page_table[*table_cnt].is_valid = 1;
        page_table[*table_cnt].frame_number = frame_pool[*frame_cnt];
        page_table[*table_cnt].arrival_timestamp = current_timestamp;
        page_table[*table_cnt].last_access_timestamp = current_timestamp;
        page_table[*table_cnt].reference_count = 1;
        return page_table[*table_cnt].frame_number;
    }
    int i_to_replace = -1;
    for (int i = 0; i < *table_cnt; i++) {
        if ((i_to_replace == -1 ||
             page_table[i].arrival_timestamp <
             page_table[i_to_replace].arrival_timestamp) &&
            page_table[i].is_valid) {
            i_to_replace = i;
        }
    }
    int newly_freed_frame = page_table[i_to_replace].frame_number;
    page_table[i_to_replace].is_valid = 0;
    page_table[i_to_replace].arrival_timestamp = 0;
    page_table[i_to_replace].last_access_timestamp = 0;
    page_table[i_to_replace].reference_count = 0;
    page_table[i_to_replace].frame_number = -1;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    page_table[page_number].frame_number = newly_freed_frame;
    return newly_freed_frame;
}

int count_page_faults_fifo(
        struct PTE page_table[TABLEMAX],
        int table_cnt,
        int reference_string[REFERENCEMAX],
        int reference_cnt,
        int frame_pool[POOLMAX],
        int frame_cnt) {
    int page_faults = 0;
    int current_timestamp = 1;
    for (int j = 0; j < reference_cnt; j++) {
        int i = reference_string[j];
        if (page_table[i].is_valid) {
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].reference_count++;
        } else if (frame_cnt > 0) {
            page_table[i].is_valid = 1;
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].frame_number = frame_pool[frame_cnt-- - 1];
            page_table[i].reference_count = 1;
            page_faults++;
        } else {
            int i_to_replace = -1;
            for (int i = 0; i < table_cnt; i++) {
                if ((i_to_replace == -1 ||
                     page_table[i].arrival_timestamp <
                     page_table[i_to_replace].arrival_timestamp) &&
                    page_table[i].is_valid) {
                    i_to_replace = i;
                }
            }
            int newly_freed_frame = page_table[i_to_replace].frame_number;
            page_table[i_to_replace].is_valid = 0;
            page_table[i_to_replace].arrival_timestamp = 0;
            page_table[i_to_replace].last_access_timestamp = 0;
            page_table[i_to_replace].reference_count = 0;
            page_table[i_to_replace].frame_number = -1;
            page_table[i].is_valid = 1;
            page_table[i].arrival_timestamp = current_timestamp;
            page_table[i].last_access_timestamp = current_timestamp;
            page_table[i].reference_count = 1;
            page_table[i].frame_number = newly_freed_frame;
            page_faults++;
        }
    }
    return page_faults;
}

int process_page_access_lru(
        struct PTE page_table[TABLEMAX],
        int *table_cnt,
        int page_number,
        int frame_pool[POOLMAX],
        int *frame_cnt,
        int current_timestamp) {
    if (page_table[page_number].is_valid) {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    if (*frame_cnt > 0) {
        page_table[*table_cnt].is_valid = 1;
        page_table[*table_cnt].frame_number = frame_pool[*frame_cnt];
        page_table[*table_cnt].arrival_timestamp = current_timestamp;
        page_table[*table_cnt].last_access_timestamp = current_timestamp;
        page_table[*table_cnt].reference_count = 1;
        return page_table[*table_cnt].frame_number;
    }
    int i_to_replace = -1;
    for (int i = 0; i < *table_cnt; i++) {
        if ((i_to_replace == -1 ||
             page_table[i].last_access_timestamp <
             page_table[i_to_replace].last_access_timestamp) &&
            page_table[i].is_valid) {
            i_to_replace = i;
        }
    }
    int newly_freed_frame = page_table[i_to_replace].frame_number;
    page_table[i_to_replace].is_valid = 0;
    page_table[i_to_replace].arrival_timestamp = 0;
    page_table[i_to_replace].last_access_timestamp = 0;
    page_table[i_to_replace].reference_count = 0;
    page_table[i_to_replace].frame_number = -1;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    page_table[page_number].frame_number = newly_freed_frame;
    return newly_freed_frame;
}

int count_page_faults_lru(
        struct PTE page_table[TABLEMAX],
        int table_cnt,
        int reference_string[REFERENCEMAX],
        int reference_cnt,
        int frame_pool[POOLMAX],
        int frame_cnt) {
    int page_faults = 0;
    int current_timestamp = 1;
    for (int j = 0; j < reference_cnt; j++) {
        int i = reference_string[j];
        if (page_table[i].is_valid) {
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].reference_count++;
        } else if (frame_cnt > 0) {
            page_table[i].is_valid = 1;
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].frame_number = frame_pool[frame_cnt-- - 1];
            page_table[i].reference_count = 1;
            page_faults++;
        } else {
            int i_to_replace = -1;
            for (int i = 0; i < table_cnt; i++) {
                if ((i_to_replace == -1 ||
                     page_table[i].last_access_timestamp <
                     page_table[i_to_replace].last_access_timestamp) &&
                    page_table[i].is_valid) {
                    i_to_replace = i;
                }
            }
            int newly_freed_frame = page_table[i_to_replace].frame_number;
            page_table[i_to_replace].is_valid = 0;
            page_table[i_to_replace].last_access_timestamp = 0;
            page_table[i_to_replace].reference_count = 0;
            page_table[i_to_replace].frame_number = -1;
            page_table[i].is_valid = 1;
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].reference_count = 1;
            page_table[i].frame_number = newly_freed_frame;
            page_faults++;
        }
    }
    return page_faults;
}

int process_page_access_lfu(
        struct PTE page_table[TABLEMAX],
        int *table_cnt,
        int page_number,
        int frame_pool[POOLMAX],
        int *frame_cnt,
        int current_timestamp) {
    if (page_table[page_number].is_valid) {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }
    if (*frame_cnt > 0) {
        page_table[*table_cnt].is_valid = 1;
        page_table[*table_cnt].frame_number = frame_pool[*frame_cnt];
        page_table[*table_cnt].arrival_timestamp = current_timestamp;
        page_table[*table_cnt].last_access_timestamp = current_timestamp;
        page_table[*table_cnt].reference_count = 1;
        return page_table[*table_cnt].frame_number;
    }
    int i_to_replace = -1;
    for (int i = 0; i < *table_cnt; i++) {
        if ((i_to_replace == -1 ||
             page_table[i].reference_count <=
             page_table[i_to_replace].reference_count) &&
            page_table[i].is_valid) {
            i_to_replace = i;
        }
    }
    int newly_freed_frame = page_table[i_to_replace].frame_number;
    page_table[i_to_replace].is_valid = 0;
    page_table[i_to_replace].arrival_timestamp = 0;
    page_table[i_to_replace].last_access_timestamp = 0;
    page_table[i_to_replace].reference_count = 0;
    page_table[i_to_replace].frame_number = -1;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    page_table[page_number].frame_number = newly_freed_frame;
    return newly_freed_frame;
}

int count_page_faults_lfu(
        struct PTE page_table[TABLEMAX],
        int table_cnt,
        int reference_string[REFERENCEMAX],
        int reference_cnt,
        int frame_pool[POOLMAX],
        int frame_cnt) {
    int page_faults = 0;
    int current_timestamp = 1;
    for (int j = 0; j < reference_cnt; j++) {
        int i = reference_string[j];
        if (page_table[i].is_valid) {
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].reference_count++;
        } else if (frame_cnt > 0) {
            page_table[i].is_valid = 1;
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].frame_number = frame_pool[frame_cnt-- - 1];
            page_table[i].reference_count = 1;
            page_faults++;
        } else {
            int i_to_replace = -1;
            for (int i = 0; i < table_cnt; i++) {
                if ((i_to_replace == -1 ||
                     page_table[i].reference_count <=
                     page_table[i_to_replace].reference_count) &&
                    page_table[i].is_valid) {
                    i_to_replace = i;
                }
            }
            int newly_freed_frame = page_table[i_to_replace].frame_number;
            page_table[i_to_replace].is_valid = 0;
            page_table[i_to_replace].arrival_timestamp = 0;
            page_table[i_to_replace].last_access_timestamp = 0;
            page_table[i_to_replace].reference_count = 0;
            page_table[i_to_replace].frame_number = -1;
            page_table[i].is_valid = 1;
            page_table[i].last_access_timestamp = current_timestamp++;
            page_table[i].reference_count = 1;
            page_table[i].frame_number = newly_freed_frame;
            page_faults++;
        }
    }
    return page_faults;
}
