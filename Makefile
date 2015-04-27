#colors
ccred:="\033[46;31m"
ccyellow:="\033[46;34m"
ccgreen:="\033[40;32m"
ccend:="\033[0m"
ifeq ($(OS),Windows_NT)
#ifeq (create,$(firstword $(MAKECMDGOALS)))
#  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
#  $(eval $(RUN_ARGS):;@:)
#endif
DIRS := $(shell dir /AD /B | findstr /v /i "\.git$")
else
DIRS := $(shell ls -d */)
endif

.PHONY: all
all:
	@echo $(DIRS)
	@ echo $(foreach var,$(NUMBERS),.\a.out $(var) && )
#	@for ob in $(DIRS) ; do \
#	echo "          " $(ccgreen) build: $$ob $(ccend);make --silent -C $$ob ; \
#	done

.PHONY: clean
clean:
	@for ob in $(DIRS) ; do \
	echo "          " $(ccgreen) clean: $$ob $(ccend);make --silent -C $$ob clean ; \
	done

.PHONY:create
create:
	@mkdir $(RUN_ARGS)
	@cp Makefile_sub $(RUN_ARGS)/Makefile
	@echo $(ccgreen)"CREATED PROJECT:"$(ccyellow) $(RUN_ARGS)$(ccend)