class Solution(object):
    def countDaysTogether(self, arriveAlice, leaveAlice, arriveBob, leaveBob):
        days_in_months = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
        
        def day_of_year(date_str):
            month, day = map(int, date_str.split('-'))
            return sum(days_in_months[:month - 1]) + day
        
        alice_start = day_of_year(arriveAlice)
        alice_end = day_of_year(leaveAlice)
        bob_start = day_of_year(arriveBob)
        bob_end = day_of_year(leaveBob)
        
        overlap_start = max(alice_start, bob_start)
        overlap_end = min(alice_end, bob_end)
        
        if overlap_start <= overlap_end:
            return overlap_end - overlap_start + 1
        return 0
