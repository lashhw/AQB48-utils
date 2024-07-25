all:
	g++ -Wall -O3 merge_ply.cpp -o merge_ply

clean:
	rm -rf merge_ply
