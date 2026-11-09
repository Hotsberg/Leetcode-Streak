#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long free_time;
    int id;
} BusyServer;

typedef struct {
    BusyServer* data;
    int size;
} BusyHeap;

static int cmp_busy(BusyServer a, BusyServer b) {
    if (a.free_time != b.free_time) return a.free_time < b.free_time;
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

static void build(int* tree, int node, int l, int r) {
    tree[node] = r - l + 1;
    if (l == r) return;
    int mid = l + (r - l) / 2;
    build(tree, 2 * node, l, mid);
    build(tree, 2 * node + 1, mid + 1, r);
}

static void update(int* tree, int node, int l, int r, int pos, int val) {
    if (l == r) {
        tree[node] = val;
        return;
    }
    int mid = l + (r - l) / 2;
    if (pos <= mid) update(tree, 2 * node, l, mid, pos, val);
    else update(tree, 2 * node + 1, mid + 1, r, pos, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

static int find_first_free(int* tree, int node, int l, int r) {
    if (l == r) return l;
    int mid = l + (r - l) / 2;
    if (tree[2 * node] > 0) return find_first_free(tree, 2 * node, l, mid);
    return find_first_free(tree, 2 * node + 1, mid + 1, r);
}

static int query_first(int* tree, int node, int l, int r, int ql, int qr) {
    if (l > qr || r < ql || tree[node] == 0) return -1;
    if (ql <= l && r <= qr) {
        return find_first_free(tree, node, l, r);
    }
    int mid = l + (r - l) / 2;
    int res = query_first(tree, 2 * node, l, mid, ql, qr);
    if (res != -1) return res;
    return query_first(tree, 2 * node + 1, mid + 1, r, ql, qr);
}

int* busiestServers(int k, int* arrival, int arrivalSize, int* load, int loadSize, int* returnSize) {
    int n = arrivalSize;
    int* request_cnt = (int*)calloc((size_t)k, sizeof(int));
    int* tree = (int*)malloc((size_t)(4 * k) * sizeof(int));
    build(tree, 1, 0, k - 1);

    BusyHeap busy_h;
    busy_h.data = (BusyServer*)malloc((size_t)k * sizeof(BusyServer));
    busy_h.size = 0;

    for (int i = 0; i < n; i++) {
        long long cur_time = arrival[i];
        long long cur_load = load[i];

        while (busy_h.size > 0 && busy_h.data[0].free_time <= cur_time) {
            BusyServer b = busy_pop(&busy_h);
            update(tree, 1, 0, k - 1, b.id, 1);
        }

        int target = i % k;
        int assigned = query_first(tree, 1, 0, k - 1, target, k - 1);
        if (assigned == -1 && target > 0) {
            assigned = query_first(tree, 1, 0, k - 1, 0, target - 1);
        }

        if (assigned != -1) {
            request_cnt[assigned]++;
            update(tree, 1, 0, k - 1, assigned, 0);
            BusyServer b = {cur_time + cur_load, assigned};
            busy_push(&busy_h, b);
        }
    }

    int max_req = 0;
    for (int i = 0; i < k; i++) {
        if (request_cnt[i] > max_req) {
            max_req = request_cnt[i];
        }
    }

    int count = 0;
    for (int i = 0; i < k; i++) {
        if (request_cnt[i] == max_req) {
            count++;
        }
    }

    int* ans = (int*)malloc((size_t)count * sizeof(int));
    *returnSize = count;
    int idx = 0;
    for (int i = 0; i < k; i++) {
        if (request_cnt[i] == max_req) {
            ans[idx++] = i;
        }
    }

    free(request_cnt);
    free(tree);
    free(busy_h.data);

    return ans;
}
