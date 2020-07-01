#include "oslabs.h"

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process == [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]) {
        int total_bursttime = new_process->total_bursttime;
        new_process->execution_starttime = timestamp;
        new_process->execution_endttime = timestamp + total_bursttime;
        new_process->remaining_bursttime = total_bursttime;
        return new_process;
    } else {
        if (new_process->process_priority >= current_process->process_priority) {
            new_process->execution_starttime = 0;
            new_process->execution_endtime = 0;
            new_process->remaining_bursttime = new_process->total_bursttime;
            ready_queue[queue_cnt++] = new_process;
            return current_process;
        } else {
            new_process->execution_starttime = timestamp;
            new_process->execution_endttime = timestamp + new_process->total_bursttime;
            new_process->remaining_bursttime = new_process->total_bursttime;
            current_process->execution_endtime = 0;
            /* not sure about this next line */
            current_process->remaining_bursttime = new_process->total_bursttime;
            ready_queue[queue_cnt++] = current_process;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (queue_cnt < 1) {
        return [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0];
    } else {
        struct PCB highest_priority_PCB = [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0];
        highest_priority_PCB->process_priority = 1024;
        for (int i = 0; i < queue_cnt; i++) {
            if (ready_queue[i]->process_priority < highest_priority_PCB->process_priority) {
                highest_priority_PCB = ready_queue[i];
            }
        }
        highest_priority_PCB->execution_starttime = timestamp;
        highest_priority_PCB->execution_endtime = timestamp + highest_priority_PCB->remaining_bursttime;
        return highest_priority_PCB;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    if (current_process == [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]) {
        new_process->execution_starttime = timestamp;
        new_process->execution_endtime = timestamp + new_process->total_bursttime;
        new_process->remaining_bursttime = new_process->total_bursttime;
        return new_process;
    } else {
        if (current_process->remaining_bursttime <= new_process->total_bursttime) {
            new_process->execution_starttime = 0;
            new_process->execution_endtime = 0;
            new_process->remaining_bursttime = new_process->total_bursttime;
            ready_queue[queue_cnt++] = new_process;
            return current_process;
        } else {
            new_process->execution_starttime = timestamp;
            new_process->execution_endtime = timestamp + new_process->total_bursttime;
            /* not sure about this next line */
            current_process->remaining_bursttime = new_process->total_bursttime;
            current_process->execution_starttime = 0;
            current_process->execution_endtime = 0;
            current_process->remaining_bursttime = 0;
            return new_process;
        }
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (queue_cnt < 1) {
        return [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0];
    } else {
        struct PCB next_PCB = [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0];
        for (int i = 0; i < queue_cnt; i++) {
            if (ready_queue[i]->remaining_bursttime < next_PCB->remaining_bursttime) {
                next_PCB = ready_queue[i];
            }
        }
        next_PCB->execution_starttime = timestamp;
        next_PCB->execution_endtime = timestamp + next_PCB->remaining_bursttime;
        return next_PCB;
    }
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process == [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]) {
        int total_bursttime = new_process->total_bursttime;
        new_process->execution_starttime = timestamp;
        new_process->execution_endttime = timestamp + total_bursttime;
        new_process->remaining_bursttime = total_bursttime;
        return new_process;
    } else {
        new_process->execution_starttime = 0;
        new_process->execution_endtime = 0;
        new_process->remaining_bursttime = new_process->total_bursttime;
        ready_queue[queue_cnt++] = new_process;
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (queue_cnt < 1) {
        return [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0];
    } else {
        struct PCB next_PCB = ready_queue[0];
        for (int i = 0; i < queue_cnt; i++) {
            if (ready_queue[i]->arrival_timestamp < next_PCB->arrival_timestamp) {
                next_PCB = ready_queue[i];
            }
        }
        next_PCB->execution_starttime = timestamp;
        next_PCB->execution_endtime = timestamp + next_PCB->remaining_bursttime;
        return next_PCB;
    }
}
