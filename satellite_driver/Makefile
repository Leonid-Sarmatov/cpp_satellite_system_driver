obj-m += satellite.o

# Перечисляем ВСЕ исходные файлы модуля     echo_satellite.o
satellite-y := \
	satellite_main.o \
    probe_disconnect/probe_disconnect.o \
	files_operations/files_operations.o

# Пути к заголовкам
ccflags-y := \
	-I$(src) \
	-I$(src)/probe_disconnect \
	-I$(src)/files_operations

# Директория для временных файлов
BUILD_DIR := build

# Сборка модуля
all:
	mkdir $(BUILD_DIR)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) MO=$(PWD)/$(BUILD_DIR) modules

# Очистка сборки
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) MO=$(PWD)/$(BUILD_DIR) clean
	rm -r $(BUILD_DIR)