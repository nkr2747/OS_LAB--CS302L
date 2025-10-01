build-part1: part1
part1: mainPart1.cpp processor.cpp
	g++ mainPart1.cpp processor.cpp -o part1
run-part1: part1
	./part1 FIFO process3.dat
build-part2: part2
part2: mainPart2.cpp processor.cpp
	g++ mainPart2.cpp processor.cpp -o part2
run-part2: part2
	./part2 FIFO process1.dat
clean:
	@del -f Schedule*
	@del -f *.exe