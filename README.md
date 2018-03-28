Calibration mode design:

I set the initial loop iterations count of every subtask to be 1000.
I use an incrementor with the initial value of 2^14 and try to add the incrementor to the loop iterations count.
If the incrementor is too large, I divide the incrementor by 2 and try again.
If the incrementor is small enough so that the sum of the incrementor and the loop iterations count does not
run longer than the designated execution time, I add the incrementor to the loop iterations count.
I repeat the above process until incrementor becomes 0.

This implementation is easier than the binary search approach. I was trying to implement a simple binary search
for the loop iterations count, by setting the lower bound to 0 and the upper bound to INT_MAX. The running time
is too long. Therefore, I chose this implementation. Generally, this implementation is fast enough for our purpose.

I chose the initial loop iterations count to be 1000 because this is smaller than the loop iterations count for 1ms
of execution time, so I think this is small enough. I chose the incrementor to be 2^14 because this number is in the
order of the loop iterations count for 1ms of execution time, so I think this is large enough.
