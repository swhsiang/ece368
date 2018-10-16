test-pq:
	clang -g -o pq.o -c pq.c
	clang -g test_pq.c pq.o -o test_out

clean-test-pq:
	rm *.o
	rm test_out

test-util:
	clang -g -o pq.o -c pq.c
	clang -g -o util.o -c util.c
	clang -g test_util.c util.o pq.o -o util_out

clean-test-util:
	rm *.o
	rm util_out
