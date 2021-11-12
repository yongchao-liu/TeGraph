#cgexec -g memory:graph bin/apps/reachability --input=graph/out --source=1 --end=3
#n=1
n=$($RANDOM % 1000000)
./bin/apps/close_1 --input=graph/out --source=32324 --end=3

