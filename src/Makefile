BD=../build/
# TD=../tests/
CFLAGS=-c#-Wall -Wextra -Werror
LDFLAGS=#-Wall -Wextra -Werror
LCOVFLAGS=-fprofile-arcs -ftest-coverage
CHECKFLAGS=-lcheck
SRC=reader.c viewer.c render.c matrix.c settings.c
OBJECTS=$(addprefix $(BD),$(SRC:.c=.o))
CC=gcc
CC2=gcc
OS:=$(shell uname -s)
BREW=$(HOME)/.brew/
GTKFLAGS=`pkg-config --cflags --libs gtk4-macos --libs`
APP=$(HOME)/Applications/3DViewer_v1.0/
INSTALLFLAGS=

ifeq ($(OS), Darwin)
	CC2+= -D OS_MAC
else
	CC2+= -D OS_LINUX
	CHECKFLAGS+= -lrt -lpthread -lm
endif

# .PHONY: all main debug test gcov_report clean install uninstall 

all: main

# ./$(BD)3DViewer_v1.0 2>/dev/null
main: $(BD)3DViewer_v1.0
	./$(BD)3DViewer_v1.0

./$(BD)3DViewer_v1.0: $(OBJECTS) $(BD)gui.o
	$(CC) $(LDFLAGS) $^ -o $@ $(GTKFLAGS)

./$(BD)%.o: %.c
	$(CC) $(CFLAGS) $(INSTALLFLAGS) $< -o $@ $(GTKFLAGS)

install: INSTALLFLAGS=-DUSER_APP="\"$(APP)\""
install: clean $(BD)3DViewer_v1.0
	@mkdir $(APP) || echo "Directory already exists. Skip."
	cp -rf style/ $(APP)/style/
	cp -rf screenshots/ $(APP)/screenshots/
	cp -rf screenshots/ $(APP)/screencasts/
	cp -rf models/ $(APP)/models/
	cp -rf config/ $(APP)/config/
	cp ./$(BD)3DViewer_v1.0 $(APP)
	make clean

uninstall:
	rm -rf $(APP)
	rm -rf $(HOME)/Applications/3DViewer_v1.0.tar.gz

dvi:
	@open $(APP)/style/manual/manual.html || echo "Please, install application first!"

dist:
	tar -cvf $(APP)../3DViewer_v1.0.tar $(APP)
	gzip $(APP)../3DViewer_v1.0.tar

#### DEBUG ####
debug: $(SRC) gui.c
	$(CC) -g -fsanitize=address  $(LDFLAGS) $^ -o $(BD)3DViewer_v1.0 $(GTKFLAGS)

#### TEST #####
test: $(BD)main_test_exe
	./$^

./$(BD)main_test_exe: INSTALLFLAGS=-DTEST
./$(BD)main_test_exe: $(BD)matrix.o $(BD)render.o $(BD)reader.o $(BD)main_test.o
	$(CC2) $(LDFLAGS) $^ -o $@ $(CHECKFLAGS) $(GTKFLAGS)

# main_test.c: $(TD)main_test.check
# 	checkmk clean_mode=1 $< > $@

#### GCOV #####
gcov_report: matrix.c render.c reader.c main_test.c
	$(CC) $(LDFLAGS) $(LCOVFLAGS) -DTEST $^ -o $(BD)main_gcov_exe $(CHECKFLAGS) $(GTKFLAGS)
	./$(BD)main_gcov_exe
	mv *.gcda *.gcno ./$(BD)
	lcov -t "gcov_report" -o $(BD)gcov_report.info -c -d ./$(BD)
	genhtml -o $(BD)report $(BD)gcov_report.info
	open $(BD)report/index.html

clean:
	rm -rf $(BD)*.o $(BD)*_exe $(BD)3DViewer_v1.0 $(BD)*.dSYM
	rm -rf $(BD)*.gcno $(BD)*.gcda $(BD)*.info $(BD)report/
	rm -rf *.gcno *.gcda

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction ../src/

cpplint:
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h

leaks: $(BD)main_test_exe
	CK_FORK=no leaks --atExit -- $(BD)main_test_exe
