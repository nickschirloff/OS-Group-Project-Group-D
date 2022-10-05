cmake_minimum_required(VERSION 3.23)
project(group C)

set(CMAKE_C_STANDARD 99)

add_executable(group main.c forked_process_piping_test.c forking.c server.c GetFileData.c MessageQueueSender.c MessageQueueReceiver.c forking.h main.c)
