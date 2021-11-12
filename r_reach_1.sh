#cgexec -g memory:graph bin/apps/reachability --input=graph/out --source=1 --end=3
#./bin/apps/short_1 --input=/scratch1/hcy/flickr-growth/flickr.data --source=5000 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/wikipedia-growth/growth.data --source=1 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/wikiconflict/conflict.data --source=5000 --end=3 --num=100000
#./bin/apps/short_1 --input=/scratch1/hcy/ca-cit-HepPh/arxiv.data --source=5000 --end=3 --num=100000
#./bin/apps/reachability_o --input=/scratch1/hcy/edit-enwiki/edit.data --source=0 --end=1000
  perf stat -e instructions:u  ./bin/apps/reachability_o  --input=/scratch1/hcy/delicious-ti/delicious.data --source=0 --end=1000


