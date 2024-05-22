
typedef struct {
    int allocated_size; // rozmiar zaalokowanego bufora
    int count;          // liczba element�w w wektorze
    char* ptr;           // wska�nik do pocz�tku bufora
} vectorCH_t;


// Inicjalizacja wektora *v. Pocz�tkowo tablica b�dzie mia�a rozmiar 1.
void init(vectorCH_t* v) {
    v->count = 0;
    v->allocated_size = 1;
    v->ptr = (char*)malloc(v->allocated_size * sizeof(char));
}

// Realokacja wektora *v, tak aby bufor mia� pojemno�� reallocate_size.
void reallocate(vectorCH_t* v, int reallocate_size) {
    /*fprintf(stderr, "Zmiana rozmiaru %d->%d [%d element�w u�ywanych]\n",
        v->allocated_size, reallocate_size, v->count);*/
    v->allocated_size = reallocate_size;
    char* newPtr = (char*)malloc(v->allocated_size * sizeof(char));
    if (newPtr) {
        memcpy(newPtr, v->ptr, v->count * sizeof(char));
        free(v->ptr);
        v->ptr = newPtr;
    }
}


// Umieszczenie warto�ci val jako nowego (ko�cowego) elementu wektora *v.
void push_back(vectorCH_t* v, char val) {
    //Realokacja wymagana, gdy bufor nie jest w stanie pomie�ci� wi�cej element�w.
    if (v->count == v->allocated_size)
        reallocate(v, 2 * v->allocated_size);
    v->ptr[v->count] = val;
    v->count++;
}


// Pobranie i usuni�cie warto�ci ko�cowego elementu wektora *v.
char pop_back(vectorCH_t* v) {
    v->count--;
    char retv = v->ptr[v->count];

    //Realokacja realizowana, gdy rozmiar bufora jest ponad dwukrotnie
    //nadmiarowy w stosunku do liczby przechowywanych element�w.
    if (4 * v->count <= v->allocated_size)
        reallocate(v, v->allocated_size / 2);
    return retv;
}

void delete_vector(vectorCH_t* v) {
    free(v->ptr);
    v->ptr = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int allocated_size; // rozmiar zaalokowanego bufora
    int count;          // liczba element�w w wektorze
    vectorCH_t* ptr;           // wska�nik do pocz�tku bufora
} vectorV_t;

// Inicjalizacja wektora *v. Pocz�tkowo tablica b�dzie mia�a rozmiar 1.
void init(vectorV_t* v) {
    v->count = 0;
    v->allocated_size = 1;
    v->ptr = (vectorCH_t*)malloc(v->allocated_size * sizeof(vectorCH_t));
}

// Realokacja wektora *v, tak aby bufor mia� pojemno�� reallocate_size.
void reallocate(vectorV_t* v, int reallocate_size) {
    /*fprintf(stderr, "Zmiana rozmiaru %d->%d [%d element�w u�ywanych]\n",
        v->allocated_size, reallocate_size, v->count);*/
    v->allocated_size = reallocate_size;
    vectorCH_t* newPtr = (vectorCH_t*)malloc(v->allocated_size * sizeof(vectorCH_t));
    if (newPtr) {
        memcpy(newPtr, v->ptr, v->count * sizeof(vectorCH_t));
        free(v->ptr);
        v->ptr = newPtr;
    }
}


// Umieszczenie warto�ci val jako nowego (ko�cowego) elementu wektora *v.
void push_back(vectorV_t* v, vectorCH_t val) {
    //Realokacja wymagana, gdy bufor nie jest w stanie pomie�ci� wi�cej element�w.
    if (v->count == v->allocated_size)
        reallocate(v, 2 * v->allocated_size);
    v->ptr[v->count] = val;
    v->count++;
}

void delete_vector(vectorV_t* v) {
    free(v->ptr);
    v->ptr = NULL;
}


// Pobranie i usuni�cie warto�ci ko�cowego elementu wektora *v.
vectorCH_t pop_back(vectorV_t* v) {
    v->count--;
    vectorCH_t retv = v->ptr[v->count];

    //Realokacja realizowana, gdy rozmiar bufora jest ponad dwukrotnie
    //nadmiarowy w stosunku do liczby przechowywanych element�w.
    if (4 * v->count <= v->allocated_size)
        reallocate(v, v->allocated_size / 2);
    return retv;
}

////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int x;
    int y;
}Point;

typedef struct {
    int allocated_size; // rozmiar zaalokowanego bufora
    int count;          // liczba element�w w wektorze
    Point* ptr;           // wska�nik do pocz�tku bufora
} vectorPt_t;

// Inicjalizacja wektora *v. Pocz�tkowo tablica b�dzie mia�a rozmiar 1.
void init(vectorPt_t* v) {
    v->count = 0;
    v->allocated_size = 1;
    v->ptr = (Point*)malloc(v->allocated_size * sizeof(Point));
}

// Realokacja wektora *v, tak aby bufor mia� pojemno�� reallocate_size.
void reallocate(vectorPt_t* v, int reallocate_size) {
    /*fprintf(stderr, "Zmiana rozmiaru %d->%d [%d element�w u�ywanych]\n",
        v->allocated_size, reallocate_size, v->count);*/
    v->allocated_size = reallocate_size;
    Point* newPtr = (Point*)malloc(v->allocated_size * sizeof(Point));
    if (newPtr) {
        memcpy(newPtr, v->ptr, v->count * sizeof(Point));
        free(v->ptr);
        v->ptr = newPtr;
    }
}


// Umieszczenie warto�ci val jako nowego (ko�cowego) elementu wektora *v.
void push_back(vectorPt_t* v, Point val) {
    //Realokacja wymagana, gdy bufor nie jest w stanie pomie�ci� wi�cej element�w.
    if (v->count == v->allocated_size)
        reallocate(v, 2 * v->allocated_size);
    v->ptr[v->count] = val;
    v->count++;
}

void delete_vector(vectorPt_t* v) {
    free(v->ptr);
    v->ptr = NULL;
}


// Pobranie i usuni�cie warto�ci ko�cowego elementu wektora *v.
Point pop_back(vectorPt_t* v) {
    v->count--;
    Point retv = v->ptr[v->count];

    //Realokacja realizowana, gdy rozmiar bufora jest ponad dwukrotnie
    //nadmiarowy w stosunku do liczby przechowywanych element�w.
    if (4 * v->count <= v->allocated_size)
        reallocate(v, v->allocated_size / 2);
    return retv;
}