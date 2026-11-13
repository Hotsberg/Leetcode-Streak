#include <stdlib.h>

typedef struct {
    long long val;
    int count;
} Block;

long long countNonDecreasingSubarrays(int* nums, int numsSize, int k) {
    int n = numsSize;
    Block* dq = (Block*)malloc((size_t)(n + 1) * sizeof(Block));
    int front = n;
    int back = n - 1;

    long long cost = 0;
    long long ans = 0;
    int r = n - 1;

    for (int l = n - 1; l >= 0; l--) {
        long long cur = nums[l];
        int merged_count = 1;

        while (front <= back && dq[front].val <= cur) {
            cost += (cur - dq[front].val) * dq[front].count;
            merged_count += dq[front].count;
            front++;
        }

        front--;
        dq[front].val = cur;
        dq[front].count = merged_count;

        while (cost > (long long)k && r >= l) {
            cost -= (dq[back].val - nums[r]);
            dq[back].count--;
            if (dq[back].count == 0) {
                back--;
            }
            r--;
        }

        ans += (long long)(r - l + 1);
    }

    free(dq);
    return ans;
}
