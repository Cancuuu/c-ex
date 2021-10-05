#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char marca[20];
    char modelo[20];
    char dominio[20];
    float precio;

} Carro;

void imprimir_auto(Carro *carro);
void agregar(char *archivo);
void listar(char *archivo);
void buscar(char *archivo);
void vaciar_archivo(char *archivo);

int main()
{
    char *nombre_archivo = "autos.bin";

    vaciar_archivo(nombre_archivo);
    return 0;
}

void agregar(char *archivo)
{
    //abrir archivo
    FILE *archivo_abierto = fopen(archivo, "ab");

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    }

    //malloc de memoria para el struct
    Carro *carros = (Carro *)malloc(sizeof(Carro));

    //pedir datos
    printf("Ingrese la marca: ");
    scanf("%s", carros->marca);
    printf("Ingrese el modelo: ");
    scanf("%s", carros->modelo);
    printf("Ingrese el dominio: ");
    scanf("%s", carros->dominio);
    printf("Ingrese el precio: ");
    scanf("%f", &carros->precio);

    // fwrite(QUE ESCRIBO, CUANTO PESA CADA ELEMENTO, CUANTOS ELEMENTOS ESCRIBO, DONDE LO ESCRIBO)
    fwrite(carros, sizeof(Carro), 1, archivo_abierto);

    // cierro el archivo
    fclose(archivo_abierto);

    // libero memoria
    free(carros);
}

void listar(char *archivo)
{
    //abrir archivo
    FILE *archivo_abierto = fopen(archivo, "rb");

    Carro *carros = malloc(sizeof(Carro));

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    };

    int cantidad_autos = 0;

    // fseek me permite posicionarme en un lugar específico dentro de un archivo
    fseek(archivo_abierto, 0, SEEK_END);

    // obtengo la cantidad de autos
    cantidad_autos = ftell(archivo_abierto) / sizeof(Carro);

    printf("\nLa cantidad de autos que hay en el archivo es de %d\n\n", cantidad_autos);

    // vuelvo al principio del archivo
    rewind(archivo_abierto);

    // leo el archivo

    fread(carros, sizeof(Carro), 1, archivo_abierto);

    while (!feof(archivo_abierto))
    {
        imprimir_auto(carros);
        fread(carros, sizeof(Carro), 1, archivo_abierto);
    };
}

void buscar(char *archivo)
{

    FILE *archivo_abierto = fopen(archivo, "rb");

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    };

    Carro *carros = malloc(sizeof(Carro));

    char *query = malloc(sizeof(char) * 20);

    printf("Ingrese el modelo o marca: ");
    gets(query);

    fread(carros, sizeof(Carro), 1, archivo_abierto);

    while (!feof(archivo_abierto))
    {
        if (strcmp(carros->modelo, query) == 0 || strcmp(carros->marca, query) == 0)
        {
            imprimir_auto(carros);
        }
        else
        {
            printf("No se encontró el auto\n");
        }
        fread(carros, sizeof(Carro), 1, archivo_abierto);
    }
}

void vaciar_archivo(char *nombre_archivo) //funcion que eliminta todo el contenido del archivo
{
    char opcion = 'N';

    printf("Esta seguro que lo quiere vaciar? Esta accion no se puede deshacer (S / N)\n");
    fflush(stdin);
    scanf(" %c", &opcion);
    fflush(stdin);

    if (toupper(opcion) == 'S')
    {
        FILE *archivo = fopen(nombre_archivo, "wb");
        fclose(archivo);
        printf("Chau archivo!\n");
    }
    else
    {
        printf("Tranqui que no se borro nada.\n");
    }
}

void imprimir_auto(Carro *carro)
{
    printf("-----Datos de un automovil -----\n");
    printf("\nLa marca es: %s\n", carro->marca);
    printf("\nEl modelo es: %s\n", carro->modelo);
    printf("\nEl Dominio es: %s\n", carro->dominio);
    printf("\nEl precio es: %.2f\n\n", carro->precio);
}