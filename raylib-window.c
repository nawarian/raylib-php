//
// Created by Joseph Montanez on 4/7/18.
//

#include "php.h"
#undef LOG_INFO
#undef LOG_WARNING
#undef LOG_DEBUG
#define Rectangle RectangleWin
#define CloseWindow CloseWindowWin
#define ShowCursor ShowCursorWin
#define DrawTextA DrawTextAWin
#define DrawTextExA DrawTextExAWin
#define LoadImageA LoadImageAWin
#include "raylib.h"
#include "raylib-image.h"
#include "raylib-utils.h"
#include "raylib-window.h"


//------------------------------------------------------------------------------------------------------
//-- raylib Window PHP Custom Object
//------------------------------------------------------------------------------------------------------
zend_object_handlers php_raylib_window_object_handlers;

void php_raylib_window_free_storage(zend_object *object TSRMLS_DC)
{
    php_raylib_window_object *intern = php_raylib_window_fetch_object(object);

    zend_object_std_dtor(&intern->std);
}

zend_object * php_raylib_window_new(zend_class_entry *ce TSRMLS_DC)
{
    php_raylib_window_object *intern;
    intern = (php_raylib_window_object*) ecalloc(1, sizeof(php_raylib_window_object) + zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &php_raylib_window_object_handlers;

    return &intern->std;
}

// PHP object handling

PHP_METHOD(Window, __construct)
{
    php_raylib_window_object *intern = Z_WINDOW_OBJ_P(getThis());
}

PHP_METHOD(Window, init)
{
    zend_object *width;
    zend_long height;
    zend_string *title;

    ZEND_PARSE_PARAMETERS_START(3, 3)
            Z_PARAM_LONG(width)
            Z_PARAM_LONG(height)
            Z_PARAM_STR(title)
    ZEND_PARSE_PARAMETERS_END();

    InitWindow(zend_long_2int(width), zend_long_2int(height), title->val);
}

PHP_METHOD(Window, isReady)
{
    RETURN_BOOL(IsWindowReady());
}

PHP_METHOD(Window, shouldClose)
{
    RETURN_BOOL(WindowShouldClose());
}

PHP_METHOD(Window, isMinimized)
{
    RETURN_BOOL(IsWindowMinimized());
}

PHP_METHOD(Window, toggleFullscreen)
{
    ToggleFullscreen;
}

PHP_METHOD(Window, setIcon)
{
    zval *image;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_OBJECT(image)
    ZEND_PARSE_PARAMETERS_END();

    php_raylib_image_object *intern = Z_IMAGE_OBJ_P(image);

    SetWindowIcon(intern->image);
}

PHP_METHOD(Window, setTitle)
{
    zend_string *title;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(title)
    ZEND_PARSE_PARAMETERS_END();

    SetWindowTitle(title->val);
}

PHP_METHOD(Window, setPosition)
{
    zend_long x;
    zend_long y;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(x)
            Z_PARAM_LONG(y)
    ZEND_PARSE_PARAMETERS_END();

    SetWindowPosition(zend_long_2int(x), zend_long_2int(y));
}

//void SetWindowMonitor(int monitor);
PHP_METHOD(Window, setMonitor)
{
    zend_long monitor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(monitor)
    ZEND_PARSE_PARAMETERS_END();

    SetWindowMonitor(zend_long_2int(monitor));
}

//void SetWindowMinSize(int width, int height);
PHP_METHOD(Window, setMinSize)
{
    zend_long width;
    zend_long height;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(width)
            Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    SetWindowMinSize(zend_long_2int(width), zend_long_2int(height));
}

//void SetWindowSize(int width, int height);
PHP_METHOD(Window, setSize)
{
    zend_long width;
    zend_long height;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(width)
            Z_PARAM_LONG(height)
    ZEND_PARSE_PARAMETERS_END();

    SetWindowSize(zend_long_2int(width), zend_long_2int(height));
}

//int GetScreenWidth(void);
PHP_METHOD(Window, getScreenWidth)
{
    RETURN_LONG(GetScreenWidth());
}

//int GetScreenHeight(void);
PHP_METHOD(Window, getScreenHeight)
{
    RETURN_LONG(GetScreenHeight());
}

const zend_function_entry php_raylib_window_methods[] = {
        PHP_ME(Window, __construct, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Window, init, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, isReady, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, shouldClose, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, isMinimized, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, toggleFullscreen, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setIcon, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setTitle, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setPosition, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setMonitor, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setMinSize, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, setSize, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, getScreenWidth, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(Window, getScreenHeight, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_FE_END
};

// Extension class startup

void php_raylib_window_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "raylib", "Window", php_raylib_window_methods);
    php_raylib_window_ce = zend_register_internal_class(&ce TSRMLS_CC);
    php_raylib_window_ce->create_object = php_raylib_window_new;

    memcpy(&php_raylib_window_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    php_raylib_window_object_handlers.offset = XtOffsetOf(php_raylib_window_object, std);
    php_raylib_window_object_handlers.free_obj = &php_raylib_window_free_storage;
    php_raylib_window_object_handlers.clone_obj = NULL;
}

#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawTextA
#undef DrawTextExA
#undef LoadImageA