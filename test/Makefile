# -----------------------------------------------------------------------------
# 
# 
# -----------------------------------------------------------------------------

all:
	gcc main.c ../src/fftr.c -g -Wall -I../src -o fftr_test

iterate:
	make all && ./fftr_test && python3 test_plot.py input.txt output.txt

clean:
	rm -rf fftr_test *.txt *.dSYM
