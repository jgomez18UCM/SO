Pregunta 1:
- El proposito de la funcion clone_string es alocar la memoria que van a utilizar los punteros.
- No seria posible realizar la copia a traves de strcpy o de una asignacion debido a que estos dos
    metodos no alocan la memoria necesaria, y por tanto darian segmentation error.
Pregunta 2:
- Las posiciones de la cadena donde hay un caracter ':' se transforman en el caracter '\000'.