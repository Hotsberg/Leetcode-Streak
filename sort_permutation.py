class Solution(object):
    def sortPermutation(self, nums):
        k = -1
        out_of_place_count = 0
        
        for i, num in enumerate(nums):
            if num != i:
                if k == -1:
                    k = num
                else:
                    k &= num
                out_of_place_count += 1
                
        if out_of_place_count == 0:
            return 0
            
        return k
