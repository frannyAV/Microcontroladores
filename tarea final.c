#include <stdio.h>
#include <stdlib.h>

int main() {
    char *data = NULL;
    size_t len = 0;
    size_t size = 0;
    char input[100];
    
    printf("Introduce datos (escribe 'fin' para terminar):\n");
    
    // Leer datos desde el usuario
    while (1) {
        fgets(input, sizeof(input), stdin);
        // Si el usuario escribe "fin", terminamos la entrada
        if (strncmp(input, "fin", 3) == 0) {
            break;
        }
        size_t input_len = strlen(input);
        // Aumentar el tamaño de la memoria dinámica según sea necesario
        if (len + input_len >= size) {
            size += input_len + 1;
            data = realloc(data, size);
            if (data == NULL) {
                perror("Error al asignar memoria");
                return 1;
            }
        }
        // Copiar la entrada a la memoria dinámica
        strcpy(data + len, input);
        len += input_len;
    }

    // Grabar datos en un archivo
    FILE *file = fopen("datos.txt", "w");
    if (file == NULL) {
        perror("Error al abrir el archivo para escritura");
        free(data);
        return 1;
    }

    // Escribir los datos en el archivo
    fprintf(file, "%s", data);

    // Cerrar el archivo
    fclose(file);

    // Liberar la memoria dinámica
    free(data);
    
    printf("Los datos se han grabado en el archivo 'datos.txt'\n");

    return 0;
}
