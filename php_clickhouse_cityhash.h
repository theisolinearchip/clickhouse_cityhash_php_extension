// module constants

#define PHP_CLICKHOUSE_CITYHASH_EXTNAME "clickhouse_cityhash"
#define PHP_CLICKHOUSE_CITYHASH_VERSION "0.0.1"

// exported function headers

PHP_FUNCTION(clickhouse_cityhash64);
PHP_FUNCTION(clickhouse_cityhash64_with_seed);
PHP_FUNCTION(clickhouse_cityhash64_with_seeds);

PHP_FUNCTION(clickhouse_cityhash128);
PHP_FUNCTION(clickhouse_cityhash128_with_seed);