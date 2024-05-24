format:
	clang-format-14 -i  src/* include/*
init_project: 
	pio init --board esp32doit-devkit-v1
generate_commands_json:
	pio run -t compiledb
build:
	pio run -j 4
flash:
	pio run -j 4 --target upload --upload-port /dev/ttyUSB0
littlefs_flash:
	pio run --target uploadfs --upload-port /dev/ttyUSB0
