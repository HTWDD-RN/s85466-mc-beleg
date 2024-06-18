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
Aufgrund von mangelndem RAM kann immer nur ein Level geladen werden. Nach Abschluss kann es durch den Reset-Knopf des Arduino erneut gestartet werden.

Weitere Level mit unterschiedlichen Schwierigkeitsgraden sind im Code definiert, müssen aber entsprechend ein-/auskommentiert und das Programm erneut auf den Ardino geladen werden.

## Hardware
- Arduino Uno Rev. 3
- RGB Matrix 16x16 mit WS2812B
- Tastenmatrix 4x4
- USB-Stromversorgung mit ausreichender Leistung für die LED-Matrix

![](docs/setup%20close%20up.jpg)