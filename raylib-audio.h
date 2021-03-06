//
// Created by Joseph Montanez on 2018-12-02.
//

#ifndef RAYLIB_RAYLIB_AUDIO_H
#define RAYLIB_RAYLIB_AUDIO_H

extern zend_class_entry *php_raylib_audio_ce;

typedef struct _php_raylib_audio_object {
    zend_object std;
} php_raylib_audio_object;

static inline php_raylib_audio_object *php_raylib_audio_fetch_object(zend_object *obj) {
    return (php_raylib_audio_object *)((char *)obj - XtOffsetOf(php_raylib_audio_object, std));
}

#define Z_AUDIO_OBJ_P(zv) php_raylib_audio_fetch_object(Z_OBJ_P(zv));

void php_raylib_audio_startup(INIT_FUNC_ARGS);

#endif //RAYLIB_RAYLIB_AUDIO_H
