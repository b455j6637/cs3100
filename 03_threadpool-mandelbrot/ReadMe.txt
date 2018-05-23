Modify the Mandelbrot program to use a producer / consumer pattern. Include a write-up that contains performance values (wall time, speedup, and efficiency) from 1 to 8 worker threads in the thread pool. The write-up must also contain a report of performance values as you vary the task size. Consider at least the following sizes: 1 pixel, multiple pixels but less than 1 row, 1 row, multiple rows but less than evenly divided chunks, rows/n size chunks.

Note: Correctly timing the generation of the mandelbrot image is more complex than it may seem using this pattern.

The following aspects are likely to be used again:

A Thread Safe Task Queue
Generic Worker Thread
Task Coordination