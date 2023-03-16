#include <php.h>
#include "php_clickhouse_cityhash.h"

#include "city.c"

// function arguments
// ==================

// VOID generic arguments
ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()

// cityhashXX (only string to hash)
ZEND_BEGIN_ARG_INFO_EX(arginfo_clickhouse_cityhash, 0, 0, 1)
	ZEND_ARG_INFO(0, buf)
ZEND_END_ARG_INFO()

// cityhashXX (string + one seed argument)
ZEND_BEGIN_ARG_INFO_EX(arginfo_clickhouse_cityhash_seed_param_1, 0, 0, 2)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()

// cityhashXX (string + two seed arguments - work both for two 64bits seed
// or a single 128one divided in two parts)
ZEND_BEGIN_ARG_INFO_EX(arginfo_clickhouse_cityhash_seed_param_2, 0, 0, 3)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(0, seed_0)
	ZEND_ARG_INFO(0, seed_1)
ZEND_END_ARG_INFO()

// register functions and stuff
// ==================

zend_function_entry clickhouse_cityhash_functions[] = {
	PHP_FE(clickhouse_cityhash64, arginfo_clickhouse_cityhash)
	PHP_FE(clickhouse_cityhash64_with_seed, arginfo_clickhouse_cityhash_seed_param_1)
	PHP_FE(clickhouse_cityhash64_with_seeds, arginfo_clickhouse_cityhash_seed_param_2)
	PHP_FE(clickhouse_cityhash128, arginfo_clickhouse_cityhash)
	PHP_FE(clickhouse_cityhash128_with_seed, arginfo_clickhouse_cityhash_seed_param_2)	// one seed split in two params
	PHP_FE_END
};

zend_module_entry clickhouse_cityhash_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_CLICKHOUSE_CITYHASH_EXTNAME,
	clickhouse_cityhash_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_CLICKHOUSE_CITYHASH_VERSION,
	STANDARD_MODULE_PROPERTIES
};

// use a macro to output additional C code, to make ext dynamically loadable
// this key name must match the initial part of zend_module_entry -> ZEND_GET_MODULE([POTATO]) // zend_module_entry [POTATO]_module_entry
ZEND_GET_MODULE(clickhouse_cityhash)

// implemented functions (headers must be added on .h too!)
// =======================================================


// cityhash64
// ----------

PHP_FUNCTION(clickhouse_cityhash64) {

	char *buf;
	size_t len; // not int! (according to docs, the zend_parse_parameters on "s" -string- now
				// returns the size as a size_t type, NOT an int anymore -otherwise will fail)

	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &buf, &len) == FAILURE) {
		RETURN_NULL();
	}

	uint64 a = CityHash64(buf, len);

	// for unsigned 64 it's better to return a string
	// RETURN_LONG(a);

	result = strpprintf(0, "%lu", (unsigned long)a);
	RETURN_STR(result);
}

PHP_FUNCTION(clickhouse_cityhash64_with_seed) {

	char *buf;
	size_t len;

	size_t seed;

	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl", &buf, &len, &seed) == FAILURE) {
		RETURN_NULL();
	}

	uint64 a = CityHash64WithSeed(buf, len, seed);

	result = strpprintf(0, "%lu", (unsigned long)a);
	RETURN_STR(result);
}

PHP_FUNCTION(clickhouse_cityhash64_with_seeds) {

	char *buf;
	size_t len;

	size_t seed_0;
	size_t seed_1;

	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sll", &buf, &len, &seed_0, &seed_1) == FAILURE) {
		RETURN_NULL();
	}

	uint64 a = CityHash64WithSeeds(buf, len, seed_0, seed_1);

	result = strpprintf(0, "%lu", (unsigned long)a);
	RETURN_STR(result);
}


// cityhash128
// -----------

PHP_FUNCTION(clickhouse_cityhash128) {

	char *buf;
	size_t len;

	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &buf, &len) == FAILURE) {
		RETURN_NULL();
	}

	// we're using the uint128 typedef on city.h
	uint128 a = CityHash128(buf, len);

	result = strpprintf(0, "%lu%lu", (unsigned long)a.first, (unsigned long)a.second);
	RETURN_STR(result);
}

// instead of passing a single 128b seed, use two 64b fragments and combine them
PHP_FUNCTION(clickhouse_cityhash128_with_seed) {

	char *buf;
	size_t len;

	size_t seed_0;
	size_t seed_1;

	zend_string *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sll", &buf, &len, &seed_0, &seed_1) == FAILURE) {
		RETURN_NULL();
	}

	uint128 seed = { first: seed_0, second: seed_1 };

	uint128 a = CityHash128WithSeed(buf, len, seed);

	result = strpprintf(0, "%lu%lu", (unsigned long)a.first,  (unsigned long)a.second);
	RETURN_STR(result);
}
