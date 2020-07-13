#include "oslabs.h"

struct PCB get_NULLPCB() {
    struct PCB NULLPCB;
    NULLPCB.process_id=0;
    NULLPCB.arrival_timestamp=0;
    NULLPCB.total_bursttime=0;
    NULLPCB.execution_starttime=0;
    NULLPCB.execution_endtime=0;
    NULLPCB.remaining_bursttime=0;
    NULLPCB.process_priority=0;
    return NULLPCB;
}

int is_NULLPCB(struct PCB maybe_NULLPCB) {
    if (maybe_NULLPCB.process_id==0 &&
        maybe_NULLPCB.arrival_timestamp==0 &&
        maybe_NULLPCB.total_bursttime==0 &&
        maybe_NULLPCB.execution_starttime==0 &&
        maybe_NULLPCB.execution_endtime==0 &&
        maybe_NULLPCB.remaining_bursttime==0 &&
        maybe_NULLPCB.process_priority==0) {
        return 1;
    } else {
        return 0;
    }
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (is_NULLPCB(current_process)) {
        int total_bursttime = new_process.total_bursttime;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + total_bursttime;
        new_process.remaining_bursttime = total_bursttime;
        return new_process;
    } else {
        if (new_process.process_priority >= current_process.process_priority) {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;
            return current_process;
        } else {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            current_process.execution_endtime = 0;
            /* not sure about this next line */
            current_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = current_process;
            (*queue_cnt)++;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt < 1) {
        return get_NULLPCB();
    } else {
        struct PCB next_PCB = ready_queue[0];
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_priority < next_PCB.process_priority) {
                next_PCB = ready_queue[i];
            }
        }
        int j = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_id != next_PCB.process_id) {
                ready_queue[j++] = ready_queue[i];
            }
        }
        next_PCB.execution_starttime = timestamp;
        next_PCB.execution_endtime = timestamp + next_PCB.remaining_bursttime;
        (*queue_cnt) = (*queue_cnt) - 1;
        return next_PCB;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (is_NULLPCB(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        if (current_process.remaining_bursttime <= new_process.total_bursttime) {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;
            return current_process;
        } else {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            /* not sure about this next line */
            current_process.remaining_bursttime = new_process.total_bursttime;
            current_process.execution_starttime = 0;
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime = 0;
            ready_queue[*queue_cnt] = current_process;
            (*queue_cnt)++;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt < 1) {
        return get_NULLPCB();
    } else {
        struct PCB next_PCB = ready_queue[0];
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].remaining_bursttime < next_PCB.remaining_bursttime) {
                next_PCB = ready_queue[i];
            }
        }
        int j = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_id != next_PCB.process_id) {
                ready_queue[j++] = ready_queue[i];
            }
        }
        next_PCB.execution_starttime = timestamp;
        next_PCB.execution_endtime = timestamp + next_PCB.remaining_bursttime;
        (*queue_cnt) = (*queue_cnt) - 1;
        return next_PCB;
    }
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (is_NULLPCB(current_process)) {
        int total_bursttime = new_process.total_bursttime;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + total_bursttime;
        new_process.remaining_bursttime = total_bursttime;
        return new_process;
    } else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (*queue_cnt < 1) {
        return get_NULLPCB();
    } else {
        struct PCB next_PCB = ready_queue[0];
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].arrival_timestamp < next_PCB.arrival_timestamp) {
                next_PCB = ready_queue[i];
            }
        }
        int j = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_id != next_PCB.process_id) {
                ready_queue[j++] = ready_queue[i];
            }
        }
        next_PCB.execution_starttime = timestamp;
        next_PCB.execution_endtime = timestamp + next_PCB.remaining_bursttime;
        (*queue_cnt) = (*queue_cnt) - 1;
        return next_PCB;
    }
}
