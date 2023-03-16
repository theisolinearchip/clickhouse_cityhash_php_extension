#include <stdio.h>
#include "../../include/city.c"

int main() {

	char a[] = "Lorem ipsum dolor sit amet";
	size_t len = sizeof(a)-1;

	// uint64 b = CityHash64(a, len);
	// uint64 b = CityHash64WithSeed(a, len, 99);
	// uint64 b = CityHash64WithSeeds(a, len, 99, 1);

	// uint128 b = CityHash128(a, len);
	// uint128 s = { first: 111111111111, second: 2384724234 };
	// uint128 b = CityHash128WithSeed(a, len, s);

	// printf("str len: %ld\n128b hash: %lu%lu\n", len, b.first, b.second);
	
	uint64 b = CityHash64(a, len);

	printf("input: %s\nstr len: %ld\n64b hash: %ld\n", a, len, b);

	return 0;

}