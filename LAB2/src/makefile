build-part1: part1.out

part1.out: part1_searcher.cpp
	g++ -g part1_searcher.cpp -o part1.out

run-part1: part1.out
	./part1.out file.txt NGTNIJGK 0 67108863

clean-part1:
	rm part1.out

build-part2: part2_searcher part2_partitioner

part2_searcher: part2_searcher.cpp
	g++ part2_searcher.cpp -o part2_searcher
part2_partitioner: part2_partitioner
	g++ part2_partitioner.cpp -o part2_partitioner
run-part2:
	./part2_partitioner file.txt MYOA 0 127 32
clean-part2:
	rm part2_searcher part2_partitioner


build-part3: part3_searcher part3_partitioner

part3_searcher: part3_searcher.cpp
	g++ part3_searcher.cpp -o part3_searcher
part3_partitioner: part3_partitioner
	g++ part3_partitioner.cpp -o part3_partitioner
run-part3:
	./part3_partitioner file.txt MYOA 0 127 32
clean-part3:
	rm part3_searcher part3_partitioner
clean: clean-part1 clean-part2 clean-part3