<< Report >>
Source: data.xls

Across all three performance metrics, data collected shows the average result yielded over the following number of task samples: 10, 15, 20, 25, 30.

For efficiency, we see that consistent with expectations, the SC algorithm was the most frequent top performer with increasing number of task samples. The FIFO algorithm demonstrated the most consistent performance while LRU trailed closely behind in performance to the SC algorithm. Nonetheless, with exception to SC algorithm at the beginning, at minimum all algorithms show that cache loading was always in active operation more than 65% of the time. Otherwise, overall we see that efficiency increased with increased number of task samples.

For latency, we see that MRU was the most frequent top performer yielding lowest values. In contrast, FIFO yielded significantly higher and consistent latency, but FIFO and SC shows notably improved latency at the last increase of task samples. Otherwise, except at the last increase of task samples, overall we see that latency worsened with increased number of task samples.

For response time, we see that MRU was the most frequent top performer. This performance metric shows a better distinction between all algorithms with fewer overlaps. Thus, in contrast we see that the SC algorithm was the most frequent worst performer. Otherwise, overall we see that response time worsened with increased number of task samples.

From these results, we see how overall the performance metrics change uniformly with increased number of task samples. Albeit, however, the results nonetheless deviate considerably from being able to infer a better conclusion on the correlation between task samples, performance metrics, and memory scheduling algorithm effectiveness. It was expected that SC would clearly be shown to be the better memory scheduling algorithms. Contrary to this, MRU demonstrated the most frequent and consistent top performance in latency and response time. However, the SC algorithm has shown to be the most consistently efficient algorithm. Nonetheless, the stability of the simulation is questionable; thus, yielding inconclusive results.