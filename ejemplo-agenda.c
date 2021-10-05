/*
    Realizar un programa que gestione una agenda de contactos haciendo uso de ARCHIVOS, PUNTEROS y MEMORIA DINÁMICA.

    El programa contará con un menú de opciones como el siguiente:
    1 - Agregar contacto.
    2 - Listar contactos.
    3 - Buscar contacto por apellido y nombre.
    4 - Editar un contacto.
    5 - Ver log.
    6 - Borrar contactos.
    7 - Salir.
    
    ------------------------------------------------------------------------------------
    Cada acción realizada debe dejar asentada la acción y la hora en que se realizó 
    en un archivo separada por un salto de línea.
    Ejemplo:
    19:44:26 :: Se crea contacto.
    19:44:28 :: Se listan los contactos.
    ------------------------------------------------------------------------------------

    1) Crear un contacto con los datos nombre, apellido, edad, telefono, direccion. 
    Preguntar al usuario si quiere agregar otro antes de volver al menú principal

    2) listar todos los contactos de la agenda y al principio decir cuántos contactos tiene la agenda en total.

    3) Buscar un contacto dentro de la agenda por nombre y apellido y mostrarlo por pantalla. 
    Si no lo encuentra, decir que no pudo encontrarlo.

    4) Buscar un contacto por nombre y apellido y permitir al usuario modificar todos los datos de dicho contacto.
    Guardar todos los datos modificados en el archivo de contactos.

    5) Permitir al usuario visualizar todas las entradas del log de auditoría.

    6) Preguntar al usuario si está seguro y caso afirmativo borrar el contenido de la agenda.
    
    ------------------------------------------------------------------------------------

    NOTA: para obtener la hora usando time.h pueden usar el siguiente snippet:
    time_t t;
    struct tm *timeinfo;
    time(&t);
    timeinfo = localtime(&t);
    printf("Asi se ve el tiempo en formato hora:minutos:segundos => %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#define nombre_archivo_contactos "contactos.bin"
#define nombre_log "log_agenda.txt"

typedef struct
{
    char nombre[30];
    char apellido[30];
    int edad;
    char telefono[20];
    char direccion[100];
} Contacto;

// funciones principales
void agregar_contacto(char *nombre_archivo);
void listar_contactos(char *nombre_archivo);
void buscar_contacto(char *nombre_archivo);
void editar_contacto(char *nombre_archivo);
void ver_log(char *nombre_archivo);
void borrar_contactos(char *nombre_archivo);

// funciones auxiliares
void ingresar_datos_contacto(Contacto *contacto);
int buscar_contacto_nombre_apellido(FILE *archivo, char *nombre, char *apellido, Contacto *contacto);
void imprimir_contacto(Contacto *contacto);
FILE *abrir_archivo(char *nombre_archivo, char *modo);
void agregar_log(char *dato);
int menu();
void limpiar_pantalla();
void presione_tecla();

int main()
{
    int opcion = 0;

    while (opcion != 7)
    {
        opcion = menu();

        switch (opcion)
        {
        case 1:
            agregar_contacto(nombre_archivo_contactos);
            break;
        case 2:
            listar_contactos(nombre_archivo_contactos);
            break;
        case 3:
            buscar_contacto(nombre_archivo_contactos);
            break;
        case 4:
            editar_contacto(nombre_archivo_contactos);
            break;
        case 5:
            ver_log(nombre_log);
            break;
        case 6:
            borrar_contactos(nombre_archivo_contactos);
            break;
        case 7:
            printf("\nGracias, vuelva pronto!\n");
            break;
        default:
            break;
        }

        presione_tecla();
    }

    return 0;
}

void agregar_contacto(char *nombre_archivo)
{
    FILE *archivo = abrir_archivo(nombre_archivo, "ab");

    int cantidad_de_contactos = 1;
    Contacto *contactos = calloc(cantidad_de_contactos, sizeof(Contacto));

    // esta variable será la que se posicione sobre el contacto actual dentro de la colección de contactos
    Contacto *contacto = contactos;

    char opcion = 'S';

    while (toupper(opcion) == 'S')
    {
        ingresar_datos_contacto(contacto);

        printf("\nDesea agregar otro contacto? S/N\n");
        fflush(stdin);
        scanf(" %c", &opcion);
        fflush(stdin);

        // si se agrega otro contacto reservo memoria para que funcione
        if (toupper(opcion) == 'S')
        {
            cantidad_de_contactos++;
            Contacto *temp = realloc(contactos, sizeof(Contacto) * cantidad_de_contactos);

            if (temp == NULL)
            {
                printf("Ocurrio un error en realloc de contactos!");
                exit(-1); // indica al sistema operativo que ocurrió un error en nuestro programa
            }

            // asigno el puntero a
            contactos = temp;

            // me muevo al siguiente contacto de la colección
            contacto = contactos + cantidad_de_contactos - 1;
        }

        // se genera una nueva entrada en el log.
        agregar_log("Se crea contacto.");
    }

    fwrite(contactos, sizeof(Contacto), cantidad_de_contactos, archivo);
    fclose(archivo);
    free(contactos);
}

void listar_contactos(char *nombre_archivo)
{
    FILE *archivo = abrir_archivo(nombre_archivo, "rb");
    Contacto *contacto = malloc(sizeof(Contacto));
    int cantidad_contactos = 0;

    // me paro al final del archivo.
    fseek(archivo, 0, SEEK_END);

    cantidad_contactos = ftell(archivo) / sizeof(Contacto);

    printf("Hay un total de %d contactos en la agenda y se listan a continuacion\n\n", cantidad_contactos);

    rewind(archivo);

    fread(contacto, sizeof(Contacto), 1, archivo);

    while (!feof(archivo))
    {
        imprimir_contacto(contacto);
        fread(contacto, sizeof(Contacto), 1, archivo);
    }

    fclose(archivo);

    agregar_log("Se listan los contactos.");
}

void buscar_contacto(char *nombre_archivo)
{
    FILE *archivo = abrir_archivo(nombre_archivo, "rb");
    Contacto *contacto = malloc(sizeof(Contacto));
    char *nombre = malloc(sizeof(char) * 30);
    char *apellido = malloc(sizeof(char) * 30);
    int se_encuentra = 0;

    printf("ingrese el nombre del contacto a buscar:");
    gets(nombre);
    printf("ingrese el apellido del contacto a buscar:");
    gets(apellido);

    se_encuentra = buscar_contacto_nombre_apellido(archivo, nombre, apellido, contacto);

    if (se_encuentra == 0)
    {
        printf("El contacto con nombre %s y apellido %s no se pudo encontrar", nombre, apellido);
    }
    else if (se_encuentra == 1)
    {
        imprimir_contacto(contacto);
    }

    fclose(archivo);
    free(contacto);
    free(nombre);
    free(apellido);

    agregar_log("Se busco un contacto.");
}

void editar_contacto(char *nombre_archivo)
{
    FILE *archivo = abrir_archivo(nombre_archivo, "r+b");
    Contacto *contacto = malloc(sizeof(Contacto));
    char nombre[30], apellido[30];
    int se_encuentra = 0;

    printf("ingrese el nombre del contacto a buscar:");
    gets(nombre);
    printf("ingrese el apellido del contacto a buscar:");
    gets(apellido);

    se_encuentra = buscar_contacto_nombre_apellido(archivo, nombre, apellido, contacto);

    if (se_encuentra == 0)
    {
        printf("El contacto con nombre %s y apellido %s no se pudo encontrar", nombre, apellido);
    }
    else if (se_encuentra == 1)
    {
        printf("Se procede a editar al contacto %s %s", contacto->nombre, contacto->apellido);

        ingresar_datos_contacto(contacto);

        fseek(archivo, sizeof(Contacto) * -1, SEEK_CUR);
        fwrite(contacto, sizeof(Contacto), 1, archivo);
    }

    fclose(archivo);
    free(contacto);

    agregar_log("Se edito un contacto.");
}

void ver_log(char *nombre_archivo)
{
    int fin_de_archivo;
    char texto[200];
    FILE *archivo = abrir_archivo(nombre_archivo, "r");

    printf("El contenido del archivo de logs es el siguiente:\n\n");

    fin_de_archivo = fscanf(archivo, " %[^\n]", texto);

    while (fin_de_archivo != EOF)
    {
        printf("%s\n", texto);
        fin_de_archivo = fscanf(archivo, " %[^\n]", texto);
    }

    fclose(archivo);
}

void borrar_contactos(char *nombre_archivo)
{
    char opcion = 'N';
    printf("Esta seguro que desea borrar los contactos?\n");
    printf("Esta operacion no se puede deshacer. S/N\n");
    fflush(stdin);
    scanf(" %c", &opcion);
    fflush(stdin);

    if (toupper(opcion) == 'S')
    {
        printf("los contactos fueron borrados con exito.\n");
        FILE *archivo = abrir_archivo(nombre_archivo, "wb");
        fclose(archivo);
    }
    else
    {
        printf("No se borraron los contactos.\n");
    }
}

void ingresar_datos_contacto(Contacto *contacto)
{
    printf("Carge los datos del contacto: \n");
    printf("------------------------------\n");

    printf("\tNombre: ");
    gets(contacto->nombre);

    printf("\tApellido: ");
    gets(contacto->apellido);

    printf("\tEdad: ");
    scanf("%d", &contacto->edad);
    fflush(stdin);

    printf("\tTelefono: ");
    gets(contacto->telefono);

    printf("\tDireccion: ");
    gets(contacto->direccion);
}

int buscar_contacto_nombre_apellido(FILE *archivo, char *nombre, char *apellido, Contacto *contacto)
{
    rewind(archivo);

    fread(contacto, sizeof(Contacto), 1, archivo);

    while (!feof(archivo))
    {
        if (strcmp(contacto->nombre, nombre) == 0 && strcmp(contacto->apellido, apellido) == 0)
        {
            // si lo encontramos retornamos 1
            return 1;
        }

        fread(contacto, sizeof(Contacto), 1, archivo);
    }

    // si no se encuentra retorna 0.
    return 0;
}

void imprimir_contacto(Contacto *contacto)
{
    printf("------------------------------\n");
    printf("Datos del contacto: \n");
    printf("------------------------------\n");
    printf("\tNombre: %s\n", contacto->nombre);
    printf("\tApellido: %s\n", contacto->apellido);
    printf("\tEdad: %d\n", contacto->edad);
    printf("\tTelefono: %s\n", contacto->telefono);
    printf("\tDireccion: %s\n", contacto->direccion);
    printf("------------------------------\n");
}

int menu()
{
    int opcion = 0;

    limpiar_pantalla();

    printf("--------------------------------------------\n");
    printf("---------Menu de opciones de agenda---------\n");
    printf("--------------------------------------------\n");
    printf("\t1 - Agregar contacto.\n");
    printf("\t2 - Listar contactos.\n");
    printf("\t3 - Buscar contacto por apellido y nombre.\n");
    printf("\t4 - Editar un contacto.\n");
    printf("\t5 - Ver log.\n");
    printf("\t6 - Borrar contactos.\n");
    printf("\t7 - Salir.\n");

    scanf("%d", &opcion);
    fflush(stdin);

    limpiar_pantalla();

    return opcion;
}

FILE *abrir_archivo(char *nombre_archivo, char *modo)
{
    FILE *archivo = fopen(nombre_archivo, modo);

    if (archivo == NULL)
    {
        printf("Ocurrio un error al abrir el archivo");
        exit(-1);
    }

    return archivo;
}

void limpiar_pantalla()
{
    system("clear");
}

void presione_tecla()
{
    printf("\nPresione enter para continuar...");
    getchar();
}

void agregar_log(char *dato)
{
    FILE *archivo_log = fopen(nombre_log, "a");

    time_t t;
    struct tm *timeinfo;
    time(&t);
    timeinfo = localtime(&t);

    fprintf(archivo_log, "%02d:%02d:%02d :: %s\n",
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec,
            dato);

    fclose(archivo_log);
}