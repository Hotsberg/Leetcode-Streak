#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int idx;
} Query;

static int cmp_logs(const void* a, const void* b) {
    int* logA = *(int**)a;
    int* logB = *(int**)b;
    return logA[1] - logB[1];
}

static int cmp_queries(const void* a, const void* b) {
    const Query* qA = (const Query*)a;
    const Query* qB = (const Query*)b;
    return qA->time - qB->time;
}

int* countServers(int n, int** logs, int logsSize, int* logsColSize, int x, int* queries, int queriesSize, int* returnSize) {
    qsort(logs, logsSize, sizeof(int*), cmp_logs);

    Query* q_arr = (Query*)malloc((size_t)queriesSize * sizeof(Query));
    for (int i = 0; i < queriesSize; i++) {
        q_arr[i].time = queries[i];
        q_arr[i].idx = i;
    }
    qsort(q_arr, queriesSize, sizeof(Query), cmp_queries);

    int* cnt = (int*)calloc((size_t)(n + 1), sizeof(int));
    int active_servers = 0;
    int left = 0;
    int right = 0;

    int* ans = (int*)malloc((size_t)queriesSize * sizeof(int));
    *returnSize = queriesSize;

    for (int i = 0; i < queriesSize; i++) {
        int cur_time = q_arr[i].time;
        int cur_idx = q_arr[i].idx;
        int min_time = cur_time - x;

        while (right < logsSize && logs[right][1] <= cur_time) {
            int server = logs[right][0];
            if (cnt[server] == 0) {
                active_servers++;
            }
            cnt[server]++;
            right++;
        }

        while (left < logsSize && logs[left][1] < min_time) {
            int server = logs[left][0];
            cnt[server]--;
            if (cnt[server] == 0) {
                active_servers--;
            }
            left++;
        }

        ans[cur_idx] = n - active_servers;
    }

    free(q_arr);
    free(cnt);

    return ans;
}
