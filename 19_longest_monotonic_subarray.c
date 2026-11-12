int longestMonotonicSubarray(int* nums, int numsSize) {
    if (numsSize == 0) {
        return 0;
    }

    int max_len = 1;
    int inc = 1;
    int dec = 1;

    for (int i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1]) {
            inc++;
            dec = 1;
        } else if (nums[i] < nums[i - 1]) {
            dec++;
            inc = 1;
        } else {
            inc = 1;
            dec = 1;
        }

        if (inc > max_len) {
            max_len = inc;
        }
        if (dec > max_len) {
            max_len = dec;
        }
    }

    return max_len;
}
