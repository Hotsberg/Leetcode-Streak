#include <stdlib.h>
#include <stdbool.h>

static int cmp(const void* a, const void* b) {
    return (*(const int*)a - *(const int*)b);
}

int findTheDistanceValue(int* arr1, int arr1Size, int* arr2, int arr2Size, int d) {
    qsort(arr2, arr2Size, sizeof(int), cmp);
    int count = 0;

    for (int i = 0; i < arr1Size; i++) {
        int x = arr1[i];
        int l = 0, r = arr2Size - 1;
        bool valid = true;

        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (abs(arr2[mid] - x) <= d) {
                valid = false;
                break;
            }
            if (arr2[mid] < x) {
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        if (valid) {
            count++;
        }
    }

    return count;
}
