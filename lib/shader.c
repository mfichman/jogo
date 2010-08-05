typedef struct Jet_OpenGLShader {
    Jet_CoreEngine *engine;
    Jet_ResourceState state;
    String *name;
    bool enabled;
} Jet_OpenGLShader;

void Jet_OpenGLShader__init(
        Jet_OpenGLShader **self,
        Jet_CoreEngine *engine, 
        String *name) {
    
    *self = malloc(sizeof(Jet_OpenGLShader));

    (*self)->engine = engine;
    (*self)->name = name;
    (*self)->engine->refcount++;
    (*self)->name->refcount++;
}

void Jet_OpenGLShader__destroy(
        Jet_OpenGLShader *self) {

    Jet_OpenGLShader_state__b(RenderState::UNLOADED);
    if (!(--self.engine->refcount)) Jet_CoreEngine__destroy(self.engine);
    if (!(--self.name->refcount)) String__destroy(self.name);
    free(self);
}

void Jet_OpenGLShader_name(
        Jet_OpenGLShader *self,
        String **_out) {

    *_out = self->name; 
}

void Jet_OpenGLShader_enabled__q(
        Jet_OpenGLShader *self,
        bool *_out) {

    *_out = self->enabled;
}

void Jet_OpenGLShader_state(
        Jet_OpenGLShader *self,
        ResourceState *_out) {

    
}
