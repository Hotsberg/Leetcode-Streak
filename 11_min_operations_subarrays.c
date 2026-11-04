#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

static int cmp(const void* a, const void* b) {
    ll diff = *(const ll*)a - *(const ll*)b;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

static int get_rank(ll val, const ll* vals, int M) {
    int l = 0, r = M - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (vals[mid] == val) return mid + 1;
        if (vals[mid] < val) l = mid + 1;
        else r = mid - 1;
    }
    return 1;
}

static void update(ll* bit, int idx, ll val, int M) {
    for (; idx <= M; idx += idx & -idx) {
        bit[idx] += val;
    }
}

static ll query(const ll* bit, int idx) {
    ll sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += bit[idx];
    }
    return sum;
}

static int find_kth(const ll* bit_cnt, ll k, int M) {
    int idx = 0;
    for (int i = 1 << 17; i > 0; i >>= 1) {
        if (idx + i <= M && bit_cnt[idx + i] < k) {
            k -= bit_cnt[idx + i];
            idx += i;
        }
    }
    int res = idx + 1;
    if (res > M) {
        res = M;
    }
    return res;
}

long long minOperations(int* nums, int numsSize, int x, int k) {
    int n = numsSize;

    ll* sorted_vals = (ll*)malloc((size_t)n * sizeof(ll));
    for (int i = 0; i < n; i++) {
        sorted_vals[i] = nums[i];
    }
    qsort(sorted_vals, n, sizeof(ll), cmp);

    int M = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || sorted_vals[i] != sorted_vals[i - 1]) {
            sorted_vals[M++] = sorted_vals[i];
        }
    }

    int* ranks = (int*)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) {
        ranks[i] = get_rank(nums[i], sorted_vals, M);
    }

    ll* bit_cnt = (ll*)calloc((size_t)(M + 2), sizeof(ll));
    ll* bit_sum = (ll*)calloc((size_t)(M + 2), sizeof(ll));
    ll* cost_arr = (ll*)malloc((size_t)(n - x + 1) * sizeof(ll));

    for (int i = 0; i < n; i++) {
        update(bit_cnt, ranks[i], 1, M);
        update(bit_sum, ranks[i], nums[i], M);

        if (i >= x) {
            update(bit_cnt, ranks[i - x], -1, M);
            update(bit_sum, ranks[i - x], -nums[i - x], M);
        }

        if (i >= x - 1) {
            int median_rank = find_kth(bit_cnt, (x + 1) / 2, M);
            ll median_val = sorted_vals[median_rank - 1];

            ll left_cnt = query(bit_cnt, median_rank);
            ll left_sum = query(bit_sum, median_rank);
            ll total_sum = query(bit_sum, M);

            ll right_cnt = x - left_cnt;
            ll right_sum = total_sum - left_sum;

            cost_arr[i - x + 1] = (left_cnt * median_val - left_sum) + (right_sum - right_cnt * median_val);
        }
    }

    const ll INF = 1e18;
    ll** dp = (ll**)malloc((size_t)(k + 1) * sizeof(ll*));
    for (int j = 0; j <= k; j++) {
        dp[j] = (ll*)malloc((size_t)(n + 1) * sizeof(ll));
        for (int i = 0; i <= n; i++) {
            dp[j][i] = INF;
        }
    }

    for (int i = 0; i <= n; i++) {
        dp[0][i] = 0;
    }

    for (int j = 1; j <= k; j++) {
        for (int i = 1; i <= n; i++) {
            dp[j][i] = dp[j][i - 1];
            if (i >= x) {
                ll prev = dp[j - 1][i - x];
                if (prev < INF) {
                    ll c = cost_arr[i - x];
                    if (prev + c < dp[j][i]) {
                        dp[j][i] = prev + c;
                    }
                }
            }
        }
    }

    ll result = dp[k][n];

    free(sorted_vals);
    free(ranks);
    free(bit_cnt);
    free(bit_sum);
    free(cost_arr);
    for (int j = 0; j <= k; j++) {
        free(dp[j]);
    }
    free(dp);

    return result;
}
