.SILENT:
	SHELL=/bin/bash
	.SHELLFLAGS=--norc --noprofile -euo pipefail -c
project:
	gcc -std=c99 -Wall -Wpedantic -o camel camel.c
	./camel

installemscript:
	cd .. && git clone https://github.com/emscripten-core/emsdk.git
	cd ../emsdk && emsdk install latest && emsdk active latest && emsdk_env.bat && emcc camel.c -o camel.html
runserver:
	cd ../emsdk && http-server
