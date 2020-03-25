gcc -c  main.c  -o main.o

#создаём объектные файлы для статической библиотеки
gcc -c revert_string.c  -o stat_revert_string.o
#создаём статическую библиотеку
ar rcs libRevStr.a stat_revert_string.o
#линкуем статическую библиотеку с main.o
gcc   main.o  -L/projects/os_lab_2019/lab2/src/revert_string -lRevStr  -o stat-linked
#вызываем 
./stat-linked "Eclipse Che"
