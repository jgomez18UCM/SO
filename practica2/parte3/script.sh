#!/usr/bin/bash


if ! ls | grep  -q student-records$;then 
    echo "El archivo student-records no existe"
    exit
elif ! test -x ./student-records;then
        echo "El archivo no es ejecutable, el programa terminara"
        exit
fi

if ! ls | grep -q records.txt;then
    echo "El archivo records.txt no existe"
    exit
elif  ! test "$(stat --format "%F" records.txt)" = "fichero regular" ; then
    echo "El archivo records.txt no es un fichero regular"
    exit
fi

recs="$(cat records.txt | tr '\n' ' ')"

#Acto seguido, el script recorrerá con un bucle for todos
#los registros en formato ASCII almacenados en la variable records.
#En la primera iteración del bucle se creará un nuevo fichero binario
#de registros de estudiantes llamado database, invocando al programa
#student-records con el primer registro del fichero y la opción -c.
#En las iteraciones restantes se añadirán los demás registros uno 
#a uno al fichero database utilizando la opción -a.

rm database

for i in $recs; do
    if ! test -f database; then
    ./student-records -f database -c $i
    else
    ./student-records -f database -a $i
    fi
done

#  'El script mostrará el contenido del fichero databases de dos formas:
#  usando la opción -l de student-records y empleando el programa xxd,
#   que simplemente realizará un volcado binario del fichero.'

./student-records -f database -l
xxd database

# Finalmente, se utilizará otro bucle para recorrer de nuevo todos los registros
# en formato ASCII almacenados en la variable records.
# En este caso para cada registro se comprobará que el NIF del estudiante
# se encuentra en el fichero database, invocando el programa
# student-records con las opciones -q -n. Para extraer el NIF de 
# cada estudiante del registro en formato ASCII correspondiente
# se usará el comando cut y un pipe (consultar página de manual de cut ).
for i in $recs; do
./student-records -f database -qn $(cut -d':' -f2 <<< $i)
done