class Solution(object):
    def minCost(self, grid):
        m, n = len(grid), len(grid[0])
        
        dp = [set() for _ in range(n)]
        dp[0].add(grid[0][0])
        
        for j in range(1, n):
            val = grid[0][j]
            dp[j] = {x ^ val for x in dp[j - 1]}
            
        for i in range(1, m):
            new_dp = [set() for _ in range(n)]
            val = grid[i][0]
            new_dp[0] = {x ^ val for x in dp[0]}
            
            for j in range(1, n):
                val = grid[i][j]
                combined = dp[j] | new_dp[j - 1]
                new_dp[j] = {x ^ val for x in combined}
                
            dp = new_dp
            
        return min(dp[-1])
