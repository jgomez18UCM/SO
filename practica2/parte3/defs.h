#define MAX_CHARACTERS_NIF 9

typedef struct{
    int id;
    char NIF[MAX_CHARACTERS_NIF+1];
    char* first_name;
    char* last_name;
} student_t;

typedef enum {
    Q_NONE, Q_ID, Q_NIF
} query_t;