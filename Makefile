#  Copyright (C) Diana Cismaru & Alexandra Dragusin (2021 - 2022 / 311CA)
VG=valgrind
VFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes \
       --verbose

all: build

build:
	gcc -g main.c tree.c utils.c -o sd_fs

run-vg: build
	$(VG) $(VFLAGS) --log-file=VG_LOG.txt ./sd_fs

clean:
	rm *.o sd_fs

run:
	./sd_fs

pack:
	zip -FSr 311CA_CismaruDiana_DragusinAlexandra_Tema3_SD.zip README.md Makefile *.c *.h