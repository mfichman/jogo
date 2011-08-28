.PHONY:
all:
	scons -j 16

.PHONY:
clean:
	scons -c

test:
	scripts/test --verbose --full

check:
	scripts/test --verbose 
