<?php
	
	// test with
	// php -d extension=clickhouse_cityhash.so test.php

	define("CSV_RELATIVE_PATH", __dir__ . "/strings_cityhash64.csv"); // first row will be ommited (csv "header")
	
	echo "init\n";

	$ok = 0;
	$no_ok = 0;
	$error_list = [];

	$c = 0;
	if (($handle = fopen(CSV_RELATIVE_PATH, "r")) !== FALSE) {
		while (($line = fgetcsv($handle, null, ",")) !== FALSE) {
			
			if ($c++ == 0) {
				echo "skipping line 0 (header)";
				continue;
			}

			if (count($line) != 2) {
				echo "line params != 2, skipping!\n"; die("-");
				continue;
			}

			$str = $line[0];
			$expected_hash = $line[1];

			echo "line " . $c . "\tstring '" . $str . "'\texpected '" . $expected_hash . "'\n";

			$hash = clickhouse_cityhash64($str);

			echo $expected_hash . "\n" . $hash . "\n--\n";

			if ($expected_hash == $hash) {
				$ok++;
			} else {
				$no_ok++;
				$error_list[] = $str;
			}


		}
		fclose($handle);
	}

	echo "FINISHED; total line count: " . $c . "\n";
	echo "OK: " . $ok . "\n";
	echo "NO OK: " . $no_ok . "\n";

	if (!empty($no_ok)) {
		echo "dumping array of wrong strings:\n";
		var_dump($error_list);
	}

	echo "--\nexiting\n";