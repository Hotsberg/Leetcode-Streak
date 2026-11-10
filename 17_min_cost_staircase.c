#include <stdio.h>
#include <stdlib.h>

long long climbStairs(int n, int* costs, int costsSize) {
    long long* dp = (long long*)malloc((size_t)(n + 1) * sizeof(long long));
    dp[0] = 0;

    for (int i = 1; i <= n; i++) {
        long long step_cost = costs[i - 1];
        long long min_val = dp[i - 1] + step_cost + 1;

        if (i >= 2) {
            long long val2 = dp[i - 2] + step_cost + 4;
            if (val2 < min_val) {
                min_val = val2;
            }
        }
        if (i >= 3) {
            long long val3 = dp[i - 3] + step_cost + 9;
            if (val3 < min_val) {
                min_val = val3;
            }
        }
        dp[i] = min_val;
    }

    long long result = dp[n];
    free(dp);
    return result;
}
