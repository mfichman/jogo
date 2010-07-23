struct Geom_Rectangle {
    uint16_t _refcount;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    Bob_Mike *jim;
    Bob *jim;
};

void Geom_Rectangle__init(GeomRectangle** self) {
    refcount = 0;
}

void Geom_Rectangle__destroy(GeomRectangle* self) {
    free(self);
}


void Geom_Rectangle_width(GeomRectangle* self, int32_t width) {
}

void Geom_Rectangle_height(GeomRectangle* self, int32_t height) {
}


