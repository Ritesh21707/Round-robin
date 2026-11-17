// Round Robin scheduling engine compiled to WebAssembly.
// The scheduling logic is written in C++; the browser UI calls this function.
extern "C" {
__attribute__((visibility("default"))) void* memcpy(void* dest, const void* src, unsigned long n) {
    unsigned char* d = (unsigned char*)dest; const unsigned char* s = (const unsigned char*)src;
    for (unsigned long i = 0; i < n; ++i) d[i] = s[i];
    return dest;
}
struct Segment { int pid; int start; int end; };

// Inputs are process arrays in the order supplied by the UI.
// Outputs: segment arrays and completion times (indexed by original process index).
__attribute__((visibility("default")))
int run_round_robin(int n, const int* arrival, const int* burst, int quantum,
                    int* out_pid, int* out_start, int* out_end, int* completion,
                    int* out_queue, int* out_queue_size) {
    if (n <= 0 || n > 40 || quantum <= 0) return -1;
    int order[40], remaining[40], done[40] = {};
    for (int i = 0; i < n; ++i) { order[i] = i; remaining[i] = burst[i]; completion[i] = -1; }
    // Stable sort by arrival time.
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
        if (arrival[order[j]] < arrival[order[i]]) { int t = order[i]; order[i] = order[j]; order[j] = t; }

    int queue[10000], head = 0, tail = 0, next = 0, time = 0, finished = 0, count = 0;
    while (finished < n && count < 10000) {
        if (head == tail && next < n && time < arrival[order[next]]) {
            out_pid[count] = -1; out_start[count] = time; out_end[count] = arrival[order[next]];
            out_queue_size[count] = 0; ++count;
            time = arrival[order[next]];
        }
        while (next < n && arrival[order[next]] <= time) queue[tail++] = order[next++];
        if (head == tail) continue;
        int p = queue[head++];
        int run = remaining[p] < quantum ? remaining[p] : quantum;
        out_pid[count] = p; out_start[count] = time; out_end[count] = time + run; ++count;
        time += run; remaining[p] -= run;
        while (next < n && arrival[order[next]] <= time) queue[tail++] = order[next++];
        if (remaining[p] > 0) queue[tail++] = p;
        else { completion[p] = time; done[p] = 1; ++finished; }
        out_queue_size[count - 1] = tail - head;
        for (int k = head; k < tail; ++k) out_queue[(count - 1) * 40 + (k - head)] = queue[k];
    }
    return count;
}
}
