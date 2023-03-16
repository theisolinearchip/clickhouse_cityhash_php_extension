PHP_ARG_ENABLE(clickhouse_cityhash, Whether to enable the ClickhouseCityhash extension, [ --enable-clickhouse-cityhash Enable ClichouseCityhash])

if test "$PHP_CLICKHOUSE_CITYHASH" != "no"; then
    PHP_NEW_EXTENSION(clickhouse_cityhash, php_clickhouse_cityhash.c, $ext_shared)
fi