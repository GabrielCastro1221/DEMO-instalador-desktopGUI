# Instalador de Miscelánea Bellavista POS

Asistente de instalación con la **misma identidad visual del Sistema POS**: tema oscuro,
barra lateral de pasos, tarjetas, campos con título superior y botones primarios/secundarios.

> **Demo visual:** no instala archivos, no crea usuarios y no se conecta a MySQL. Los servicios
> (`src/services/`) son puntos de integración vacíos a propósito.

## Pasos del asistente

1. Bienvenida · 2. Términos de uso · 3. Privacidad · 4. Empresa · 5. Administrador ·
6. Base de datos (MySQL) · 7. Instalación · 8. Finalizar

## Compilar y ejecutar

Se compila dentro del repositorio del POS (usa `../external`: ImGui, GLFW y GLEW ya compilado).

```bash
cd installer
cmake -S . -B build
cmake --build build -j$(nproc)
./build/installer
```

El CMake copia la carpeta `assets/` (fuentes) junto al ejecutable (`build/assets/`).
Si distribuyes el binario, **incluye esa carpeta** a su lado.

## Tipografía

Fuente: **Inter** (Regular, Medium y SemiBold), incluida en `assets/fonts/`.
El instalador busca las fuentes en este orden:

1. Variable de entorno `INSTALLER_ASSETS_DIR` (carpeta que contiene `fonts/`)
2. `assets/fonts` junto al ejecutable (y una o dos carpetas arriba)
3. `assets/fonts` y `installer/assets/fonts` desde el directorio actual
4. Ruta de `assets` fijada por CMake al compilar
5. Fuentes del sistema (Ubuntu, Noto Sans, Liberation, DejaVu)
6. Fuente interna de Dear ImGui (último recurso)

Si algún archivo falta se muestra un aviso en la terminal y el instalador sigue funcionando.

## Estructura relevante

```
installer/
├── assets/fonts/            Inter (+ licencia)
└── src/
    ├── core/styles/
    │   ├── ColorPalette.h   Paleta (igual a la del tema oscuro del POS)
    │   ├── Theme.cpp        Espaciados, redondeos y colores de ImGui
    │   └── Fonts.cpp        Carga de la tipografía con respaldos
    ├── components/
    │   ├── UiKit.{h,cpp}    Componentes reutilizables: tarjetas, campos, avisos,
    │   │                    casillas, botones, progreso, iconos, medidor de contraseña
    │   └── *SetupView, InstallProgressView, FinishView   Vistas de cada paso
    └── views/
        ├── InstallerWindow  Barra lateral + cabecera + cuerpo + pie
        └── LegalText        Textos de Términos y Privacidad (plantilla)
```

## Cambios de diseño respecto a la versión anterior

- Barra lateral de 8 pasos agrupados (como el menú del POS) con estados *pendiente / activo / completado*.
  Los pasos completados son clicables para volver atrás (no durante la instalación).
- Cabecera con título, subtítulo e indicador "Paso N de 8".
- Tipografía Inter en lugar de la fuente de mapa de bits por defecto de ImGui; textos con tildes y eñes.
- Formularios en tarjetas, con etiquetas sobre cada campo, asterisco rojo en obligatorios,
  mensajes de error en línea y contorno de enfoque.
- Empresa: vista previa en vivo de cómo se verá la marca en el POS.
- Administrador: botón mostrar/ocultar contraseña y medidor de fortaleza.
- Base de datos: botón "Probar conexión" con estado en línea (en lugar de una ventana emergente).
- Instalación: lista de tareas con iconos animados y barra de progreso.
- Finalización: resumen de lo configurado y botón **Finalizar** que cierra el instalador.
- Ventana de 1180×760 centrada, tamaño mínimo 980×640, sin archivo `imgui.ini`.
- `Theme.cpp` ahora **sí** se compila y se aplica (antes no estaba en el CMake).

## Qué NO cambió

La lógica de pasos y validaciones (nombre comercial obligatorio; administrador con contraseña de
8 o más caracteres y confirmación igual; host, puerto, base de datos y usuario obligatorios) y los
servicios vacíos se conservaron.
