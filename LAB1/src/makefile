build-sharpen: ./a.out

./a.out: image_sharpener.cpp libppm.cpp
	g++ -g image_sharpener.cpp libppm.cpp

run-sharpen: ./a.out
	./a.out ../images/$(INPUT).ppm ../images/$(OUTPUT).ppm

clean:
	rm a.out