# Fract_ol


### Visualizer of Mandelbrot's fractal and it's variations made on OpenCL


### [Задание](/subject/fract_ol.en.pdf)

#### Вкратце:
Существуют __фракталы__, нужно использовать библиотеку __minilibx__. Задача - сделать красиво ~~или ад эпилептика~~.
Можно использовать __многопоточность__!

Используя парадигму **GDD** было разработано приложение которое использует OpenCL для распараллеливания вычислений на GPU 
тем самым обеспечивая плавность работы. 

#### Запуск:

```./fractol``` для запуска из консоли с параметрами по умолчанию: разрешение основного изображения 1300х1300, 
фрактал основного изображениия - множество Мандельброта

```./fractol -h``` для показа помощи по запуску с дополнительными аргументами

```./fractol [resolution of main image] [name of fractal for main image]```

Note: Приложение протетировано на macos с дискретной видеокартой.
Known issue: На данный момент невозможно запустить приложение на ноутбуках, в связи с тем, что на мобильных видеокартах не поддерживается вычисление с double.


![Instructions](/scrn/scrn01_instructions.png)

#### Использование:

* Для любого изображения из стороны в сторону (pan image) необходимо двигать мышкой при зажав левую клавишу мыши.
* Для приближения/удаления изображения используется колесико мышки.
* При движении мышки с зажатой правой клавишей можно менять цвета основного изрображения, а если зажть Shift, 
то цвета меняются по-другому. 
* Фракталы-производные множества Джулии можно изменять при помощи зажатия средней клавиши мыши.



![Mandelbrot](/scrn/scrn00_Mandelbrot.png)

#### Интерфейс:

* **Справа** от основного изображения расположен столбец со всеми доступными в прграмме фракталами. 
  * Первые 3 сверху - производные множества Мандельброта. 
  * 4 - множество Джулиа
  * Ниже - производные Джулии. 
По **клику правой кнопи мыши** на любом из этих изображений оно поменяется местами с основным, 
при этом позиция и увеличение основного изображения сохранятся в маленьком.


* 4 _Верхних_ картинки **левого** столбца - цветовые пресеты и позволяют переключить цвет основного изображения на соответствующий.
* 4 _Нижних_ картинки (или плейсхолдеры) показывают последние 4 сохраненных позиций.
По **клику правой кнопи мыши** на любом из этих изображений оно скопируется в основное.

![General](/scrn/scrn01_general.png)

#### Сохранение:

В программе реализовано сохранение текущего состояния главного изображения для возможности в последующем открыть его ~~и позалипать~~.
* Для того чтобы сохранить текущее состояние необходимо кликнуть правой клавишей мыши при нажатой клавише `cntrl` по основному изображению.
Превью текущего сосотяния появится в области сохранений.
* Для того чтобы загрузить ранее сохранненое состояние можно воспользоваться областью последних сохранений внизу левого столбца
или, если состояние было сохранено достаточно давно, открыть окно сохранений клавишей `S`. 
В открывшемся окне возможно перемещаться с помощью стрелочек вверх и вниз. 
* Для загрузки сосотояния - щелчок правой клавишей мыши на изображении.
* Для удаления сохранения - `cmd` + правая клавиша мыши на сохранении.

![Saves](/scrn/scrn03_saves.png)

#### Help:

* Для вызова окна помощи - `h`
* Для вызова скорой помощи - 112

* ![julia](/scrn/scrn04_julia.png)
* ![One more](/scrn/scrn05_add.png)
