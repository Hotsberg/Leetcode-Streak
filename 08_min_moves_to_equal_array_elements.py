class Solution(object):
    def minMoves(self, nums):
        min_val = min(nums)
        return sum(num - min_val for num in nums)
