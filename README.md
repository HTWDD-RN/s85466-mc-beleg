# Superstau
Beleg für Programmierung von Microcontrollern: Spiel mit Arduino

von Maxim Zimmermann (s85526) und Christoph Born (s85466)

![](docs/setup%20whole.jpg)

## Spielkonzept
Zusammenhängende Pixel gleicher Farbe auf der RGB-Matrix repräsentieren ein Fahrzeug. Ziel des Spiels ist es, das rote Auto auszuparken und durch die Lücke in der Begrenzung aus dem Spielfeld zu fahren. Dazu können sämtliche Fahrzeuge einzeln bewegt werden, jedoch nur vorwärts oder rückwärts

## Steuerung
Die Steuerung erfolgt mithilfe eines blinken Cursors in einem der Pixel der Matrix. 5 Taster auf der Tasten-Matrix sind als Steuerkreuz belegt, mit dem der Cursor bewegt werden kann. Wird der mittlere Knopf betätigt, stellt das die Auswahl des aktuell unter dem Cursor befindlichen Fahrzeugs dar. Bis zum erneuten Drücken des mittleren Knopfs ist der Cursor an das Fahrzeug gebunden und die Pfeiltasten bewegen es mit. Ist die Bewegung blockiert durch andere Fahrzeuge, die äußere Begrernzung oder eine unmögliche Fahrtrichtung bewegen sich bei Knopfdruck weder Fahrzeug noch Cursor.

Belegung der Tastenmatrix:
|  |   |         |         |         |
|--|:-:|:-------:|:-------:|:-------:|
|  | - |    -    |    -    |    -    |
|  | - |    -    | **[↑]** |    -    |
|  | - | **[←]** | **[O]** | **[→]** |
|  | - |    -    | **[↓]** |    -    |

## Spielende
Aktuell gibt es 3 verschiedene Level mit unterschiedlichen Schwierigkeitsgraden. Nach Abschluss eines Levels blinkt 3x "you won" auf der RGB-Matrix auf, danach wird das nachfolgende Level gestartet. Nach Abschluss des letzten Levels blinkt "you won" dauerhaft und durch den Reset-Knopf des Arduino kann erneut mit Level 1 gestartet werden.

## Hardware
- Arduino Uno Rev. 3
- RGB Matrix 16x16 mit WS2812B
- Tastenmatrix 4x4
- USB-Stromversorgung mit ausreichender Leistung für die LED-Matrix

![](docs/setup%20close%20up.jpg)

## Software

Code basiert auf [Beispiel XYMatrix aus FastLED-Bibliothek](https://github.com/FastLED/FastLED/blob/master/examples/XYMatrix/XYMatrix.ino)

### Wichtige Variablen-Rollen

| Rolle | Name(n)  | Bschreibung | Wertebereich | ⇒ Datentyp |
|------|--------|-------------|:------:|----------:|
| color id | pixel[i], levelX[i], colorId | identifies object on pixel | 0...14 | uint8_t |
| axis | direction, dir | x or y axis? | -1, 0, 1 | int8_t |
| cursor state | cursorState | move or selected? | 0, 1 | bool
| game coordinate | Coord.x/y, cursorX/Y, posX/Y | | -1, 15 | int8_t |
| pixel coordinate | i/j, posX/Y | | 0, 16 | uint8_t |
| button id | state, button, direction | indicates direction, like hours on clock | 0, 1, 3, 6, 9, 12 | uint8_t |
| board size | size, boardSize | one side of square | 0...16 | uint8_t |
| pixel coordinate offset | xOffset, yOffset | | 0...8 | uint8_t |
| cursor intensity | cursorScale | time dependant | 0...500 | uint16_t |