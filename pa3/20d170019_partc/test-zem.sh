echo "Running test-zem.c"
gcc test-zem.c zemaphore.c -o test-zem -lpthread
./test-zem
echo
echo "Running test-toggle.c"
gcc test-toggle.c zemaphore.c -o test-toggle -lpthread
./test-toggle
