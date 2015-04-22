#colors
ccred:="\033[46;31m"
ccyellow:="\033[46;34m"
ccgreen:="\033[40;32m"
ccend:="\033[0m"

DIRS := $(shell ls -d */)

.PHONY: all
all:
	@for ob in $(DIRS) ; do \
	echo "          " $(ccgreen) build: $$ob $(ccend);make --silent -C $$ob ; \
	done

.PHONY: clean
clean:
	@for ob in $(DIRS) ; do \
	echo "          " $(ccgreen) clean: $$ob $(ccend);make --silent -C $$ob clean ; \
	done
