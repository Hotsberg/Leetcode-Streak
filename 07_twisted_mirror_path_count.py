class Solution(object):
    def uniquePaths(self, grid):
        m, n = len(grid), len(grid[0])
        MOD = 10**9 + 7
        size = m * n
        
        land_R = [-1] * size
        land_D = [-1] * size
        dp = [0] * size
        
        dp[size - 1] = 1
        
        for r in range(m - 1, -1, -1):
            r_offset = r * n
            for c in range(n - 1, -1, -1):
                idx = r_offset + c
                
                if grid[r][c] == 0:
                    land_R[idx] = idx
                    land_D[idx] = idx
                else:
                    if r + 1 < m:
                        land_R[idx] = land_D[idx + n]
                    if c + 1 < n:
                        land_D[idx] = land_R[idx + 1]
                
                if grid[r][c] == 0 and idx != size - 1:
                    total = 0
                    
                    if c + 1 < n:
                        target_r = land_R[idx + 1]
                        if target_r != -1:
                            total = dp[target_r]
                            
                    if r + 1 < m:
                        target_d = land_D[idx + n]
                        if target_d != -1:
                            total = (total + dp[target_d]) % MOD
                            
                    dp[idx] = total
                    
        return dp[0]
