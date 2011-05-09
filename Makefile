.PHONY:
all:
	scons

.PHONY:
clean:
	scons -c

test:
	scripts/test
