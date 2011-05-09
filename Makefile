.PHONY:
all:
	scons

.PHONY:
clean:
	scons -c

test:
	scripts/test --verbose --full

check:
	scripts/test --verbose 
