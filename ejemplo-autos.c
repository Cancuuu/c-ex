/*
    Hacer uso de Memoria dinámica, punteros y archivos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char marca[20];
    char modelo[20];
    char dominio[6];
    float precio;
} Auto;

void agregar(char *nombre_archivo);
void listar(char *nombre_archivo);
void buscar(char *nombre_archivo);
void vaciar_archivo(char *nombre_archivo);

FILE *abrir_archivo(char *nombre_archivo, char *modo);
void imprimir_auto(Auto *automovil);

int main()
{
    int opcion = 0;
    char *nombre_archivo = "autos.bin";

    printf("MENU DE OPCIONES:");
    printf("\n****************\n");
    printf("\n   1) AGREGAR AUTOS");
    printf("\n   2) LISTAR AUTOS");
    printf("\n   3) BUSCAR AUTOS");
    printf("\n   4) VACIAR ARCHIVO");
    printf("\n   5) SALIR DEL PROGRAMA");

    fflush(stdin); //limpio buffer
    printf("\n\n\t Ingrese su opcion: ");
    scanf("%d", &opcion); //almaceno valor elegido en opcion

    switch (opcion)
    {
    case 1:
        agregar(nombre_archivo);
        break;
    case 2:
        listar(nombre_archivo);
        break;
    case 3:
        buscar(nombre_archivo);
        break;
    case 4:
        vaciar_archivo(nombre_archivo);
        break;
    case 5:
        printf("\n\n\nFIN DEL PROGRAMA.....GRACIAS");
        exit(0);
    default:
        printf("\nLa opcion no es valida");
    }

    printf("presione enter para continuar...");
    fflush(stdin);
    getchar();

    return 0;
}

void agregar(char *nombre_archivo)
{
    // abro el archivo
    FILE *archivo = abrir_archivo(nombre_archivo, "ab");

    // esta es la colección de automóviles
    Auto *automoviles = malloc(sizeof(Auto));

    Auto *temp = NULL;

    // este es el automovil que uso actualmente
    Auto *automovil = automoviles; // es el primer elemento en la colección

    int contador = 1;
    char opcion = 'S';

    while (toupper(opcion) == 'S')
    {
        printf("\n\tIntroduce la MARCA: ");
        fflush(stdin);
        gets(automovil->marca);
        printf("\n\tIntroduce el MODELO: ");
        fflush(stdin);
        gets(automovil->modelo);
        printf("\n\tIntroduce el DOMINIO: ");
        fflush(stdin);
        gets(automovil->dominio);
        printf("\n\tIntroduce el PRECIO: ");
        fflush(stdin);
        scanf("%f", &automovil->precio);
        fflush(stdin);

        printf("Quiere agregar un auto mas? (S / N)\n");
        fflush(stdin);
        scanf(" %c", &opcion);
        fflush(stdin);

        if (toupper(opcion) == 'S')
        {
            contador++;
            temp = realloc(automoviles, sizeof(Auto) * contador);

            if (temp == NULL)
            {
                printf("Error al realocar memoria");
                exit(-1);
            }

            automoviles = temp;

            automovil = automoviles + contador - 1;
        }
    }

    // fwrite(QUE ESCRIBO, CUANTO PESA CADA ELEMENTO, CUANTOS ELEMENTOS ESCRIBO, DONDE LO ESCRIBO)
    fwrite(automoviles, sizeof(Auto), contador, archivo);

    // cierro el archivo
    fclose(archivo);

    // libero memoria
    free(automoviles);
}

void listar(char *nombre_archivo)
{
    // abro el archivo
    FILE *archivo = abrir_archivo(nombre_archivo, "rb");

    Auto *automovil = malloc(sizeof(Auto));

    int cantidad_autos = 0;

    // fseek me permite posicionarme en un lugar específico dentro de un archivo
    fseek(archivo, 0, SEEK_END);

    // obtengo la cantidad de autos
    cantidad_autos = ftell(archivo) / sizeof(Auto);

    printf("\nLa cantidad de autos que hay en el archivo es de %d\n\n", cantidad_autos);

    // vuelvo a pararme al principio del archivo
    rewind(archivo);

    fread(automovil, sizeof(Auto), 1, archivo);

    while (!feof(archivo))
    {
        imprimir_auto(automovil);

        fread(automovil, sizeof(Auto), 1, archivo);
    }

    // cierro el archivo
    fclose(archivo);

    // libero memoria
    free(automovil);
}

void buscar(char *nombre_archivo)
{
    // abro el archivo
    FILE *archivo = abrir_archivo(nombre_archivo, "rb");

    Auto *automovil = malloc(sizeof(Auto));

    char *valor_busqueda = malloc(sizeof(char) * 50);
    int encontro = 0;

    printf("Ingrese el modelo o marca: ");
    gets(valor_busqueda);

    fread(automovil, sizeof(Auto), 1, archivo);

    while (!feof(archivo))
    {
        if (strcmp(automovil->marca, valor_busqueda) == 0 || strcmp(automovil->modelo, valor_busqueda) == 0)
        {
            imprimir_auto(automovil);
            encontro = 1;
        }

        fread(automovil, sizeof(Auto), 1, archivo);
    }

    if (encontro == 0)
    {
        printf("No hay un auto que tenga el modelo o marca %s", valor_busqueda);
    }

    // cierro el archivo
    fclose(archivo);

    // libero memoria
    free(automovil);
    free(valor_busqueda);
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

FILE *abrir_archivo(char *nombre_archivo, char *modo)
{
    FILE *archivo = fopen(nombre_archivo, modo);

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    }

    return archivo;
}

void imprimir_auto(Auto *automovil)
{
    printf("-----Datos de un automovil -----\n");
    printf("\nLa marca es: %s\n", automovil->marca);
    printf("\nEl modelo es: %s\n", automovil->modelo);
    printf("\nEl Dominio es: %s\n", automovil->dominio);
    printf("\nEl precio es: %.2f\n\n", automovil->precio);
}