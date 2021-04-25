# Умная грядка

Устройство поливает грядку, если почва стала сухой (уровень влажности почвы для полива и регулярность проверки влажности настраивается). Также если станет темно (уровень освещения для включения лампы настраивается) устройство включит освещение. Если станет холодно для растения устройство сигнализирует о том, что лучше переместить его в другое место. 

Настройки задаются с помощью удобного меню прямо на устройстве и сохраняются в энергонезависимую память (EEPROM).

Устройство будет выглядеть как небольшая «комнатная» грядка.

## Необходимые компоненты:

Arduino-совместимая плата (UNO, Nano, Leonardo, Micro)
1. 4 кнопки
2. 2 резистора 10 кОм
3. Источник питания 7 - 12 v
4. MOSFET-транзистор 
5. Датчик влажности почвы
6. Термистор
7. Фоторезистор
8. Помпа
9. Реле
10. Текстовый LCD экран 
11. Соединительные провода
12. Разные крепежи

## Принцип работы

Устройство следит за влажностью почвы, температурой воздуха и уровнем освещённости.

Если почва становится более сухая, чем заданное значение, то включится полив (измерение влажности почвы происходят каждый заданный промежуток времени, во избежание коррозии датчика влажности почвы (например, каждые 5 минут)).

Если уровень освещённости или температура становятся ниже заданного значение, то устройство начнет сигнализировать об этом и подаст напряжение на пины для возможного дальнейшего добавления реле на освещение и обогрев.

По одинарному клику на основном экране, вы попадаете в меню настроек полива (регулярность проверки влажности почвы и влажность почвы для полива)

По двойному клику — меню настроек сигнализирования (температуры и уровня освещённости)

По тройному клику — меню пресетов

По четверному клику — меню перезагрузки и сброса настроек


## Используемые в скетче библиотеки

• [GyverButton](https://github.com/AlexGyver/GyverLibs/tree/master/GyverButton)  —  для упрощения работы с кнопкой
• LiquidCrystal  —  для работы с текстовым LCD-экраном
• EEPROM — для работы с энергонезависимой памятью (EEPROM)

## Преимущества устройства

• Автономное использование от аккумуляторов или блока питания
• Полностью автономная настройка всех параметров устройства из меню настроек (не требует изменений в прошивке)
• Удобный и понятный интерфейс

