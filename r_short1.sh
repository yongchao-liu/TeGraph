#cgexec -g memory:graph bin/apps/reachability --input=graph/out --source=1 --end=3
#perf stat -e instructions:u ./bin/apps/short_1 --input=/scratch1/hcy/flickr-growth/flickr.data --source=5000 --end=3 --num=100000
#perf stat -e L1-dcache-load-misses ./bin/apps/short_1 --input=/scratch1/hcy/flickr-growth/flickr.data --source=5000 --end=3 --num=100000
#perf stat -a -e LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetch-misses ./bin/apps/short_1 --input=/scratch1/hcy/flickr-growth/flickr.data --source=5000 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/wikipedia-growth/growth.data --source=1 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/wikiconflict/conflict.data --source=5000 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/ca-cit-HepPh/arxiv.data --source=5000 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/edit-enwiki/edit.data --source=5000 --end=3 --num=100000
perf stat -a -e LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetch-misses ./bin/apps/short_1 --input=/scratch1/hcy/delicious-ti/delicious.data --source=5000 --end=3 --num=100000
#    perf stat -e instructions:u ./bin/apps/short_1 --input=/scratch1/hcy/delicious-ti/delicious.data --source=5000 --end=3 --num=100000
#perf stat -e L1-dcache-load-misses ./bin/apps/short_1 --input=/scratch1/hcy/delicious-ti/delicious.data --source=5000 --end=3 --num=100000


