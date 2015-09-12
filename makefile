default: main

.PHONY: main clean

CFLAGS=$(shell pkg-config wayland-client wayland-egl wayland-cursor egl glesv2 libdrm --cflags --libs)

main: src/main.c
	$(CC) -std=c11 -Wall -pthread -Isrc -lm -DHAVE_CONFIG $(CFLAGS) src/main.c -o main

clean:
	rm -f main
