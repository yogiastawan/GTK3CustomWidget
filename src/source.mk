CC= gcc
CFLAGS= -Wall -Werror -Wextra -v -std=c99 
CFLAGSEND= -lm -ldl
DBGCFLAGS= -g

GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`
SRC = $(wildcard *.c) $(wildcard */*.c)
OBJ = $(SRC:.c=.o)
EXEC= GTK3CustomWidget

RELBUILDDIR=../build-release/
DBGBUILDDIR=../build-debug/
RELOBJS=$(addprefix $(RELBUILDDIR)src/, $(OBJ))
DBGOBJS=$(addprefix $(DBGBUILDDIR)src/, $(OBJ))
RELOBJRES=$(wildcard $(RELBUILDDIR)res/*.o)
DBGOBJRES=$(wildcard $(DBGBUILDDIR)res/*.o)
RELEXEC=$(addprefix $(RELBUILDDIR), $(EXEC))
DBGEXEC=$(addprefix $(DBGBUILDDIR), $(EXEC))
release: $(RELEXEC)
debug: $(DBGEXEC)
$(RELEXEC): $(RELOBJS) $(RELOBJRES)
	@echo -e "=>\e[94mCreate \"\e[4m$(@F)\e[24m\" executable to release\e[0m"
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGSEND) $(GTKFLAGS)
	@echo -e "=>\e[92mCreating \"\e[4m$(@F)\e[24m\" into \"\e[4m$(@D)\e[24m\" Success\e[0m"

$(RELBUILDDIR)src/%.o: %.c
	@echo -e "=>\e[94mPreparing \"\e[4m$<\e[24m\" to release\e[0m"

	@mkdir -p $(RELBUILDDIR)/src/$(<D)
	$(CC)  $(CFLAGS) -c $< -o $@ $(CFLAGSEND) $(GTKFLAGS)
	@echo -e "=>\e[92mBuilding \"\e[4m$(@F)\e[24m\" into \"\e[4m$(@D)\e[24m\" Success\e[0m"

############################# FOR DEBUG #######################

$(DBGEXEC): $(DBGOBJS) $(DBGOBJRES)
	@echo -e "=>\e[94mCreate \"\e[4m$(@F)\e[24m\" executable to debug\e[0m"
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $^ $(CFLAGSEND) $(GTKFLAGS)
	@echo -e "=>\e[92mCreating \"\e[4m$(@F)\e[24m\" into \"\e[4m$(@D)\e[24m\" Success\e[0m"

#create all object file (.o) for debug
$(DBGBUILDDIR)src/%.o: %.c
	@echo -e "=>\e[94mPreparing \"\e[4m$<\e[24m\" to debug\e[0m"
	@mkdir -p $(DBGBUILDDIR)src/$(<D)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -c $< -o $@ $(CFLAGSEND) $(GTKFLAGS)
	@echo -e "=>\e[92mBuilding \"\e[4m$(@F)\e[24m\" into \"\e[4m$(@D)\e[24m\" Success\e[0m"

clean:
	@echo -e "=>\e[94mDeleting all object file\e[0m"
	rm -f $(RELOBJS) $(DBGOBJS)
	@echo -e "=>\e[94mDeleting all executable file\e[0m"
	rm -f $(RELEXEC) $(DBGEXEC)
	@echo -e "=>\e[94mDeleting all folder container\e[0m"
	rm -rf $(RELBUILDDIR)src $(DBGBUILDDIR)src