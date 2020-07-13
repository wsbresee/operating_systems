#include "oslabs.h"
#include <stdio.h>

struct MEMORY_BLOCK get_NULLBLOCK() {
    struct MEMORY_BLOCK NULLBLOCK;
    NULLBLOCK.start_address=0;
    NULLBLOCK.end_address=0;
    NULLBLOCK.segment_size=0;
    NULLBLOCK.process_id=0; //0 indicates a free block
    return NULLBLOCK;
}

int is_NULLBLOCK(struct MEMORY_BLOCK block) {
    if (block.start_address == 0 &&
            block.end_address == 0 &&
            block.segment_size == 0 &&
            block.process_id == 0) {
        return 1;
    }
    return 0;
}

struct MEMORY_BLOCK best_fit_allocate(
        int request_size,
        struct MEMORY_BLOCK memory_map[MAPMAX],
        int *map_cnt,
        int process_id) {
    int min_difference = -1;
    int current_difference = 0;
    int i_of_allocated_block = -1;
    for (int i = 0; i < *map_cnt; i++) {
        current_difference = memory_map[i].segment_size - request_size;
        if ((memory_map[i].process_id == 0) && (current_difference >= 0) &&
                ((min_difference < 0) ||
                 (current_difference < min_difference))) {
            min_difference = current_difference;
            i_of_allocated_block = i;
        }
    }
    if (min_difference > 0) {
        struct MEMORY_BLOCK new_block = get_NULLBLOCK();
        new_block.start_address =
            memory_map[i_of_allocated_block].start_address + request_size;
        new_block.end_address = memory_map[i_of_allocated_block].end_address;
        new_block.segment_size =
            new_block.end_address - new_block.start_address + 1;
        new_block.process_id = 0;
        memory_map[*map_cnt] = new_block;
        (*map_cnt)++;
        memory_map[i_of_allocated_block].end_address =
            memory_map[i_of_allocated_block].start_address + request_size - 1;
        memory_map[i_of_allocated_block].segment_size = request_size;
    }
    if (!is_NULLBLOCK(memory_map[i_of_allocated_block])) {
        memory_map[i_of_allocated_block].process_id = process_id;
        memory_map[i_of_allocated_block].process_id = process_id;
    }
    if (i_of_allocated_block >= 0) {
        return memory_map[i_of_allocated_block];
    }
    return get_NULLBLOCK();
}

struct MEMORY_BLOCK first_fit_allocate(
        int request_size,
        struct MEMORY_BLOCK memory_map[MAPMAX],
        int *map_cnt,
        int process_id) {
    int lowest_address = -1;
    int i_of_allocated_block = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].segment_size >= request_size &&
                ((memory_map[i].start_address <
                  memory_map[i_of_allocated_block].start_address) ||
                 (lowest_address < 0))) {
            i_of_allocated_block = i;
            lowest_address = memory_map[i_of_allocated_block].start_address;
        }
    }
    if (memory_map[i_of_allocated_block].segment_size > request_size) {
        struct MEMORY_BLOCK new_block = get_NULLBLOCK();
        new_block.start_address =
            memory_map[i_of_allocated_block].start_address + request_size;
        new_block.end_address = memory_map[i_of_allocated_block].end_address;
        new_block.segment_size =
            new_block.end_address - new_block.start_address + 1;
        new_block.process_id = 0;
        memory_map[*map_cnt] = new_block;
        (*map_cnt)++;
        memory_map[i_of_allocated_block].end_address =
            memory_map[i_of_allocated_block].start_address + request_size - 1;
        memory_map[i_of_allocated_block].segment_size = request_size;
    }
    if (!is_NULLBLOCK(memory_map[i_of_allocated_block])) {
        memory_map[i_of_allocated_block].process_id = process_id;
    }
    if (i_of_allocated_block >= 0) {
        return memory_map[i_of_allocated_block];
    }
    return get_NULLBLOCK();
}

struct MEMORY_BLOCK worst_fit_allocate(
        int request_size,
        struct MEMORY_BLOCK memory_map[MAPMAX],
        int *map_cnt,
        int process_id) {
    int worst_fit = 0;
    int i_of_allocated_block = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].segment_size >= request_size &&
                memory_map[i].segment_size > worst_fit) {
            i_of_allocated_block = i;
            worst_fit = memory_map[i_of_allocated_block].segment_size;
        }
    }
    if (memory_map[i_of_allocated_block].segment_size > request_size) {
        struct MEMORY_BLOCK new_block = get_NULLBLOCK();
        new_block.start_address =
            memory_map[i_of_allocated_block].start_address + request_size;
        new_block.end_address = memory_map[i_of_allocated_block].end_address;
        new_block.segment_size =
            new_block.end_address - new_block.start_address + 1;
        new_block.process_id = 0;
        memory_map[*map_cnt] = new_block;
        (*map_cnt)++;
        memory_map[i_of_allocated_block].end_address =
            memory_map[i_of_allocated_block].start_address + request_size - 1;
        memory_map[i_of_allocated_block].segment_size = request_size;
    }
    if (!is_NULLBLOCK(memory_map[i_of_allocated_block])) {
        memory_map[i_of_allocated_block].process_id = process_id;
    }
    if (i_of_allocated_block >= 0) {
        return memory_map[i_of_allocated_block];
    }
    return get_NULLBLOCK();
}

struct MEMORY_BLOCK next_fit_allocate(
        int request_size,
        struct MEMORY_BLOCK memory_map[MAPMAX],
        int *map_cnt,
        int process_id,
        int last_address) {
    int diff_from_last_address = -1;
    int current_diff_from_last_address = -1;
    int i_of_allocated_block = -1;
    for (int i = 0; i < *map_cnt; i++) {
        current_diff_from_last_address =
            memory_map[i].start_address - last_address;
        if (memory_map[i].segment_size >= request_size &&
                current_diff_from_last_address >= 0 &&
                (current_diff_from_last_address < diff_from_last_address ||
                 diff_from_last_address < 0)) {
            i_of_allocated_block = i;
            diff_from_last_address = current_diff_from_last_address;
        }
    }
    if (memory_map[i_of_allocated_block].segment_size > request_size) {
        struct MEMORY_BLOCK new_block = get_NULLBLOCK();
        new_block.start_address =
            memory_map[i_of_allocated_block].start_address + request_size;
        new_block.end_address = memory_map[i_of_allocated_block].end_address;
        new_block.segment_size =
            new_block.end_address - new_block.start_address + 1;
        new_block.process_id = 0;
        memory_map[*map_cnt] = new_block;
        (*map_cnt)++;
        memory_map[i_of_allocated_block].end_address =
            memory_map[i_of_allocated_block].start_address + request_size - 1;
        memory_map[i_of_allocated_block].segment_size = request_size;
    }
    if (!is_NULLBLOCK(memory_map[i_of_allocated_block])) {
        memory_map[i_of_allocated_block].process_id = process_id;
    }
    if (i_of_allocated_block >= 0) {
        return memory_map[i_of_allocated_block];
    }
    return get_NULLBLOCK();
}

void release_memory(
        struct MEMORY_BLOCK freed_block,
        struct MEMORY_BLOCK memory_map[MAPMAX],
        int *map_cnt) {
    int preceeding_i = -1;
    int following_i = -1;
    int freed_i = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].start_address == freed_block.start_address) {
            freed_i = i;
        } else if (((memory_map[i].end_address + 1) ==
                freed_block.start_address) &&
                memory_map[i].process_id == 0) {
            preceeding_i = i;
        } else if (((memory_map[i].start_address - 1) ==
                freed_block.end_address) &&
                memory_map[i].process_id == 0) {
            following_i = i;
        }
    }
    if (freed_i >= 0) {
        memory_map[freed_i].process_id = 0;
    }
    if (preceeding_i >= 0) {
        memory_map[freed_i].start_address =
            memory_map[preceeding_i].start_address;
        memory_map[freed_i].segment_size +=
            memory_map[preceeding_i].segment_size;
        memory_map[preceeding_i].process_id = 0;
        memory_map[preceeding_i].start_address = 0;
        memory_map[preceeding_i].end_address = 0;
        memory_map[preceeding_i].segment_size = 0;
    }
    if (following_i >= 0) {
        memory_map[freed_i].end_address = memory_map[following_i].end_address;
        memory_map[freed_i].segment_size +=
            memory_map[following_i].segment_size;
        memory_map[following_i].process_id = 0;
        memory_map[following_i].start_address = 0;
        memory_map[following_i].end_address = 0;
        memory_map[following_i].segment_size = 0;
    }
    int j = 0;
    int end = *map_cnt;
    for (int i = 0; i < end; i++) {
        if (!is_NULLBLOCK(memory_map[i])) {
            memory_map[j].process_id = memory_map[i].process_id;
            memory_map[j].start_address = memory_map[i].start_address;
            memory_map[j].end_address = memory_map[i].end_address;
            memory_map[j].segment_size = memory_map[i].segment_size;
            j++;
        } else {
            (*map_cnt)--;
        }
    }
}

// int main() {
//     struct MEMORY_BLOCK memory_map[MAPMAX];
//     memory_map[0] = get_NULLBLOCK();
//     memory_map[0].start_address = 0;
//     memory_map[0].end_address = 1023;
//     memory_map[0].segment_size = 1024;
//     memory_map[0].process_id = 0;
//     int cnt = 1;
//     int *map_cnt = &cnt;
//     int request_size = 10;
//     int process_id = 32;
//     printf("%s", "bang");
//     struct MEMORY_BLOCK memory_map[i_of_allocated_block] = best_fit_allocate(request_size, memory_map, map_cnt, process_id);
//     printf("%d", memory_map[0].process_id);
// }
