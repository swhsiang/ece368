test-pq:
	clang -g -o pq.o -c pq.c
	clang -g test_pq.c pq.o -o test_out

clean-test-pq:
	rm *.o
	rm test_out

test-util:
	clang -g -c pq.c -o pq.o 
	clang -g -c tree.c -o tree.o 
	clang -g -o util.o -c util.c
	clang -g test_util.c tree.o util.o pq.o -o util_out

clean-test-util:
	rm *.o
	rm util_out

huff:
	clang -g -c pq.c -o pq.o 
	clang -g -c tree.c -o tree.o 
	clang -g -o util.o -c util.c
	clang -g huff.c tree.o util.o pq.o -o huff

clean-huff:
	rm *.o
	rm huff

unhuff:
	clang -g -c pq.c -o pq.o 
	clang -g -o util.o -c util.c
	clang -g unhuff.c pq.o util.o -o unhuff

clean-unhuff:
	rm *.o
	rm unhuff

