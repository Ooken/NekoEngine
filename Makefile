#colors


ifeq (create,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

ifeq ($(OS),Windows_NT)
DIRS := $(shell dir /AD /B | findstr /v /i "\.git$")
CP := copy Makefile_sub /A "$(RUN_ARGS)/Makefile"
else
DIRS := $(shell ls -d */)
CP := cp Makefile_sub $(RUN_ARGS)/Makefile
ccred:="\033[46;31m"
ccyellow:="\033[46;34m"
ccgreen:="\033[40;32m"
ccend:="\033[0m"
endif

.PHONY: all
all: 
ifeq ($(OS),Windows_NT)
	@for %%x in ($(DIRS)) do make --silent -C %%x
else
	@for ob in $(DIRS) ; do \
	echo "          " $(ccgreen) build: $$ob $(ccend);make --silent -C $$ob ; \
	done
endif

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	@for %%x in ($(DIRS)) do make --silent -C %%x clean
else
	@for ob in $(DIRS) ; do \
	echo "          " $(ccgreen) clean: $$ob $(ccend);make --silent -C $$ob clean ; \
	done
endif

.PHONY:create
create:
	@echo $(ccgreen)CREATE PROJECT:$(ccyellow) $(RUN_ARGS)$(ccend)
	-@mkdir $(RUN_ARGS)
	@$(CP)