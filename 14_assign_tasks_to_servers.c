#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int weight;
    int id;
} FreeServer;

typedef struct {
    long long free_time;
    int weight;
    int id;
} BusyServer;

typedef struct {
    FreeServer* data;
    int size;
} FreeHeap;

static int cmp_free(FreeServer a, FreeServer b) {
    if (a.weight != b.weight) return a.weight < b.weight;
    return a.id < b.id;
}

static void free_push(FreeHeap* h, FreeServer item) {
    h->data[h->size] = item;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (cmp_free(h->data[i], h->data[p])) {
            FreeServer tmp = h->data[i];
            h->data[i] = h->data[p];
            h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static FreeServer free_pop(FreeHeap* h) {
    FreeServer top = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (2 * i + 1 < h->size) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (cmp_free(h->data[left], h->data[smallest])) smallest = left;
        if (right < h->size && cmp_free(h->data[right], h->data[smallest])) smallest = right;
        if (smallest != i) {
            FreeServer tmp = h->data[i];
            h->data[i] = h->data[smallest];
            h->data[smallest] = tmp;
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

typedef struct {
    BusyServer* data;
    int size;
} BusyHeap;

static int cmp_busy(BusyServer a, BusyServer b) {
    if (a.free_time != b.free_time) return a.free_time < b.free_time;
    if (a.weight != b.weight) return a.weight < b.weight;
    return a.id < b.id;
}

static void busy_push(BusyHeap* h, BusyServer item) {
    h->data[h->size] = item;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (cmp_busy(h->data[i], h->data[p])) {
            BusyServer tmp = h->data[i];
            h->data[i] = h->data[p];
            h->data[p] = tmp;
            i = p;
        } else {
            break;
        }
    }
}

static BusyServer busy_pop(BusyHeap* h) {
    BusyServer top = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (2 * i + 1 < h->size) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (cmp_busy(h->data[left], h->data[smallest])) smallest = left;
        if (right < h->size && cmp_busy(h->data[right], h->data[smallest])) smallest = right;
        if (smallest != i) {
            BusyServer tmp = h->data[i];
            h->data[i] = h->data[smallest];
            h->data[smallest] = tmp;
            i = smallest;
        } else {
            break;
        }
    }
    return top;
}

int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    int n = serversSize;
    int m = tasksSize;
    *returnSize = m;

    FreeHeap free_h;
    free_h.data = (FreeServer*)malloc((size_t)n * sizeof(FreeServer));
    free_h.size = 0;

    for (int i = 0; i < n; i++) {
        FreeServer s = {servers[i], i};
        free_push(&free_h, s);
    }

    BusyHeap busy_h;
    busy_h.data = (BusyServer*)malloc((size_t)n * sizeof(BusyServer));
    busy_h.size = 0;

    int* ans = (int*)malloc((size_t)m * sizeof(int));
    long long current_time = 0;

    for (int j = 0; j < m; j++) {
        if ((long long)j > current_time) {
            current_time = (long long)j;
        }

        while (busy_h.size > 0 && busy_h.data[0].free_time <= current_time) {
            BusyServer b = busy_pop(&busy_h);
            FreeServer f = {b.weight, b.id};
            free_push(&free_h, f);
        }

        if (free_h.size == 0) {
            current_time = busy_h.data[0].free_time;
            while (busy_h.size > 0 && busy_h.data[0].free_time <= current_time) {
                BusyServer b = busy_pop(&busy_h);
                FreeServer f = {b.weight, b.id};
                free_push(&free_h, f);
            }
        }

        FreeServer s = free_pop(&free_h);
        ans[j] = s.id;

        BusyServer b = {current_time + tasks[j], s.weight, s.id};
        busy_push(&busy_h, b);
    }

    free(free_h.data);
    free(busy_h.data);

    return ans;
}
