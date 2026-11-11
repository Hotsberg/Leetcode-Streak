#include <stdio.h>

long long minimumCost(int cost1, int cost2, int costBoth, int need1, int need2) {
    long long c1 = cost1;
    long long c2 = cost2;
    long long cb = costBoth;
    long long n1 = need1;
    long long n2 = need2;

    long long min_req = (n1 < n2) ? n1 : n2;
    long long max_req = (n1 > n2) ? n1 : n2;

    long long opt1 = n1 * c1 + n2 * c2;
    long long opt2 = min_req * cb + (n1 - min_req) * c1 + (n2 - min_req) * c2;
    long long opt3 = max_req * cb;

    long long ans = opt1;
    if (opt2 < ans) {
        ans = opt2;
    }
    if (opt3 < ans) {
        ans = opt3;
    }

    return ans;
}
