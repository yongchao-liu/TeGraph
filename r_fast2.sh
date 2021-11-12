#cgexec -g memory:graph bin/apps/reachability --input=graph/out --source=1 --end=3
#n=1
perf stat -e L1-dcache-load-misses ./bin/apps/fast_2 --input=graph/out --source=32324 --end=3

