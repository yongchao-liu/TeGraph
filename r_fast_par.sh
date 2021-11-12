#cgexec -g memory:graph bin/apps/reachability --input=graph/out --source=1 --end=3
#perf stat -a -e LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetch-misses ./bin/apps/short --input=graph/out --source=1 --end=3
# perf stat -e instructions:u ./bin/apps/short --input=graph/out --source=1 --end=3
#perf stat -e L1-dcache-load-misses ./bin/apps/short --input=graph/out --source=1 --end=3
./bin/apps/fast_parall --input=graph/out --source=1 --end=3

