# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -O -lm

# Nombre del ejecutable final
TARGET = genetic_algorithm

# Archivos fuente
SRCS = main.c individual.c genetic_algorithm.c

# Archivos objeto (se generan a partir de los archivos fuente)
OBJS = $(SRCS:.c=.o)

# Regla por defecto (compila todo)
all: $(TARGET)

# Regla para compilar el ejecutable final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar cada archivo fuente en un archivo objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

# Regla para recompilar desde cero
rebuild: clean all