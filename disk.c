#include "oslabs.h"
#include <stdlib.h>

struct RCB get_NULLRCB() {
    struct RCB NULLRCB;
    NULLRCB.request_id=0;
    NULLRCB.arrival_timestamp=0;
    NULLRCB.cylinder=0;
    NULLRCB.address=0;
    NULLRCB.process_id=0;
    return NULLRCB;
}

int is_NULLRCB(struct RCB NULLRCB) {
    return (NULLRCB.request_id==0 &&
            NULLRCB.arrival_timestamp==0 &&
            NULLRCB.cylinder==0 &&
            NULLRCB.address==0 &&
            NULLRCB.process_id==0);
}

struct RCB handle_request_arrival_fcfs(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt,
        struct RCB current_request,
        struct RCB new_request,
        int timestamp) {
    if (is_NULLRCB(current_request)) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_fcfs(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt) {
    if (*queue_cnt) {
        int i_to_remove = -1;
        for (int i = 0; i < *queue_cnt; i++) {
            if ((i_to_remove < 0) ||
                request_queue[i].arrival_timestamp <
                request_queue[i_to_remove].arrival_timestamp) {
                i_to_remove = i;
            }
        }
        struct RCB request_to_remove = request_queue[i_to_remove];
        int j = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (i != i_to_remove) {
                request_queue[j++] = request_queue[i];
            }
        }
        (*queue_cnt)--;
        return request_to_remove;
    }
    return get_NULLRCB();
}

struct RCB handle_request_arrival_sstf(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt,
        struct RCB current_request,
        struct RCB new_request,
        int timestamp) {
    if (is_NULLRCB(current_request)) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_sstf(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt,
        int current_cylinder) {
    if (*queue_cnt) {
        int current_distance = abs(request_queue[0].cylinder-current_cylinder);
        int new_distance = current_distance;
        int i_to_remove = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            new_distance = abs(request_queue[i].cylinder - current_cylinder);
            if (new_distance < current_distance) {
                i_to_remove = i;
                current_distance = new_distance;
            } else if ((new_distance == current_distance) &&
                       request_queue[i].arrival_timestamp <
                       request_queue[i_to_remove].arrival_timestamp) {
                    i_to_remove = i;
                }
            }
        }
        struct RCB request_to_remove = request_queue[i_to_remove];
        int j = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (i != i_to_remove) {
                request_queue[j++] = request_queue[i];
            }
        }
        (*queue_cnt)--;
        return request_to_remove;
    }
    return get_NULLRCB();
}

struct RCB handle_request_arrival_look(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt,
        struct RCB current_request,
        struct RCB new_request,
        int timestamp) {
    if (is_NULLRCB(current_request)) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_look(
        struct RCB request_queue[QUEUEMAX],
        int *queue_cnt,
        int current_cylinder,
        int scan_direction) {
}
