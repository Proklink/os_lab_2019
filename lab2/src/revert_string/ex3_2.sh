#создаём объектные файлы для динамической библиотеки
gcc -c -fPIC revert_string.c  -o shared_revert_string.o
#создаём динамическую библиотеку
gcc -shared shared_revert_string.o -o libShRevStr.so
#линкуем динамическую библиотеку с main.o
gcc  main.o -L/projects/os_lab_2019/lab2/src/revert_string -lShRevStr -o shared-linked
#указываем переменной окружения для библиотек местоположение нашей динамической библиотеки
LD_LIBRARY_PATH=/projects/os_lab_2019/lab2/src/revert_string  
#вызываем
./shared-linked "Eclipse Che" 
