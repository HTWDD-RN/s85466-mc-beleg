#include <FastLED.h>

#define LED_PIN 4
#define KEYP_PIN A1

#define COLOR_ORDER GRB
#define CHIPSET WS2812B

#define BRIGHTNESS 64

#define NON 0  // NONE
#define CRE 1  // CAR RED
#define CYE 2  // CAR YELOW
#define BWH 3  // BUS WHITE
#define CB1 4  // CAR BROWN 1
#define TOR 5  // TRUCK ORANGE
#define CG1 6  // CAR GREEN 1
#define CG2 7  // CAR GREEN 2
#define CB2 8  // CAR BROWN 2
#define CLG 9  // CAR LIGHT GREEN
#define CBL 10 // CAR BLUE
#define CPU 11 // CAR PURPLE
#define TGR 12 // TRUCK GREEN
#define EXT 14 // EXIT
#define BOR 13 // BORDER

#define X_AXIS 0
#define Y_AXIS 1

#define CURSOR_MOVE false
#define CURSOR_SELECTED true
#define MAX_VEHICLE_SIZE 3 + 1
#define BOARD_SIZE 8
#define BLINK_RATE 500

#define COL1_PIN 5
#define COL2_PIN 6
#define COL3_PIN 7
#define COL4_PIN 8
#define ROW1_PIN 9
#define ROW2_PIN 10
#define ROW3_PIN 11
#define ROW4_PIN 12

typedef struct
{
    int x;
    int y;
} Coord;

int cursorX = 3;
int cursorY = 1;
bool cursorState = CURSOR_MOVE;

int state = 0;

int pixel[BOARD_SIZE * BOARD_SIZE] = {
    BOR, BOR, BOR, BOR, BOR, BOR, BOR, BOR,
    BOR, NON, CYE, CYE, BWH, NON, CB1, BOR,
    BOR, TOR, CG1, CG1, BWH, NON, CB1, BOR,
    EXT, TOR, CG2, CB2, CRE, CRE, NON, BOR,
    BOR, TOR, CG2, CB2, NON, CBL, CBL, BOR,
    BOR, NON, CPU, CPU, CPU, NON, NON, BOR,
    BOR, NON, NON, NON, TGR, TGR, TGR, BOR,
    BOR, BOR, BOR, BOR, BOR, BOR, BOR, BOR};

Coord neighbors[MAX_VEHICLE_SIZE];

const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

const bool kMatrixSerpentineLayout = true;
const bool kMatrixVertical = false;

uint16_t XY(uint8_t x, uint8_t y)
{
    uint16_t i;

    if (kMatrixSerpentineLayout == false)
    {
        if (kMatrixVertical == false)
        {
            i = (y * kMatrixWidth) + x;
        }
        else
        {
            i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
        }
    }

    if (kMatrixSerpentineLayout == true)
    {
        if (kMatrixVertical == false)
        {
            if (y & 0x01)
            {
                // Odd rows run backwards
                uint8_t reverseX = (kMatrixWidth - 1) - x;
                i = (y * kMatrixWidth) + reverseX;
            }
            else
            {
                // Even rows run forwards
                i = (y * kMatrixWidth) + x;
            }
        }
        else
        { // vertical positioning
            if (x & 0x01)
            {
                i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
            }
            else
            {
                i = kMatrixHeight * (kMatrixWidth - x) - (y + 1);
            }
        }
    }

    return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
CRGB *const leds(leds_plus_safety_pixel + 1);

uint16_t XYsafe(uint8_t x, uint8_t y)
{
    if (x >= kMatrixWidth)
        return -1;
    if (y >= kMatrixHeight)
        return -1;
    return XY(x, y);
}

void DrawOneFrame(int cursorScale)
{
    int xOffset = (kMatrixWidth - BOARD_SIZE) / 2;
    int yOffset = (kMatrixHeight - BOARD_SIZE) / 2;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            CRGB color = getPixelColor(i, j);
            if (i == cursorX && j == cursorY)
            {
                color.r = max(color.r, 20) * BLINK_RATE / cursorScale;
                color.g = max(color.g, 20) * BLINK_RATE / cursorScale;
                color.b = max(color.b, 20) * BLINK_RATE / cursorScale;
            }

            drawPixel(xOffset + i, yOffset + j, color);
            // drawPixel(i*2, j*2, color);
            // drawPixel(i*2, j*2+1, color);
            // drawPixel(i*2+1, j*2, color);
            // drawPixel(i*2+1, j*2+1, color);
        }
    }
}

CRGB toColor(int colorId)
{
    switch (colorId)
    {
    case 0:
        return CRGB(0, 0, 0);
    case 1:
        return CRGB(255, 0, 0); // RED CAR!!!
    case 2:
        return CRGB(230, 215, 0); // Yellow
    case 3:
        return CRGB(255, 255, 255); // White
    case 4:
        return CRGB(95, 60, 30); // Rose
    case 5:
        return CRGB(255, 110, 0); // Dark Orange
    case 6:
        return CRGB(0, 255, 0); // Green
    case 7:
        return CRGB(5, 40, 0); // Dark Green
    case 8:
        return CRGB(255, 0, 255); // Magenta
    case 9:
        return CRGB(0, 128, 128); // Teal
    case 10:
        return CRGB(0, 0, 255); // Blue
    case 11:
        return CRGB(80, 30, 10); // Violet
    case 12:
        return CRGB(0, 255, 127); // Cyan
    case 13:
        return CRGB(10, 10, 8);
    case 14:
        return CRGB(0, 0, 0);

    default:
        return CRGB(5, 5, 4);
    }
}

int drawPixel(int posX, int posY, CRGB color)
{
    // invert x coords to fix flipped display
    leds[XY(kMatrixWidth - 1 - posX, posY)] = color;
}

int getPixel(int posX, int posY)
{
    return pixel[posX + posY * BOARD_SIZE];
}

void setPixel(int posX, int posY, int colorId)
{
    pixel[posX + posY * BOARD_SIZE] = colorId;
}

CRGB getPixelColor(int posX, int posY)
{
    return toColor(getPixel(posX, posY));
}

/*void drawCursor(int brightness)
{
    CRGB originalColor = getPixelColor(cursorX, cursorY);
    CRGB color = {0, 0, 0};
    color.red = originalColor.red * brightness / 255;
    color.green = originalColor.green * brightness / 255;
    color.blue = originalColor.blue * brightness / 255;
    drawPixel(cursorX, cursorY, color);
}*/

void getNeighboringPixels(int posX, int posY)
{
    int direction = -1;
    for (int i = 0; i < MAX_VEHICLE_SIZE; i++)
    {
        neighbors[i].x = -1;
        neighbors[i].y = -1;
    }
    int neighborCounter = 1;
    int colorId = getPixel(posX, posY);
    if (colorId == BOR || colorId == NON || colorId == EXT)
    {
        neighbors[MAX_VEHICLE_SIZE - 1].y = -1;
        return;
    }

    neighbors[0].x = posX;
    neighbors[0].y = posY;

    for (int offsetX = 0; offsetX < MAX_VEHICLE_SIZE + 1; offsetX++)
    {
        //        Serial.print(posX + offsetX - (MAX_VEHICLE_SIZE / 2 - 1));
        //        Serial.print(":");
        //        Serial.println(posY);
        if ((posX + offsetX - 2) != posX && getPixel(posX + offsetX - (MAX_VEHICLE_SIZE / 2 - 1), posY) == colorId)
        {
            neighbors[neighborCounter].x = posX + offsetX - 2;
            neighbors[neighborCounter].y = posY;
            neighborCounter++;
            direction = X_AXIS;
        }
    }
    for (int offsetY = 0; offsetY < MAX_VEHICLE_SIZE + 1; offsetY++)
    {
        if ((posY + offsetY - 2) != posY && getPixel(posX, posY + offsetY - (MAX_VEHICLE_SIZE / 2 - 1)) == colorId)
        {
            //            Serial.print(getPixel(posX, posY + offsetY - (MAX_VEHICLE_SIZE / 2)));
            //            Serial.print("==");
            //            Serial.println(colorId);
            neighbors[neighborCounter].x = posX;
            neighbors[neighborCounter].y = posY + offsetY - 2;
            neighborCounter++;
            direction = Y_AXIS;
        }
    }
    neighbors[MAX_VEHICLE_SIZE - 1].x = neighborCounter;
    neighbors[MAX_VEHICLE_SIZE - 1].y = direction;
    // printNeighbors();
}

bool noNeighboursCollision(int direction)
{
    int colorId = getPixel(neighbors[0].x, neighbors[0].y);
    int segments = neighbors[MAX_VEHICLE_SIZE - 1].x;
    for (int i = 0; i < segments; i++)
    {
        // Serial.print(i);
        // Serial.print(" - ");
        bool noCollission = true;
        switch (direction)
        {
        case 12:
            noCollission = noCollision(neighbors[i].x, neighbors[i].y - 1, colorId);
            break;
        case 3:
            noCollission = noCollision(neighbors[i].x + 1, neighbors[i].y, colorId);
            break;
        case 6:
            noCollission = noCollision(neighbors[i].x, neighbors[i].y + 1, colorId);
            // Serial.println(noCollission);
            break;
        case 9:
            noCollission = noCollision(neighbors[i].x - 1, neighbors[i].y, colorId);
            break;
        default:
            noCollission = true;
        }
        if (noCollission == false)
        {
            return false;
        }
    }
    return true;
}

void moveNeighbours(int direction)
{
    int colorId = getPixel(neighbors[0].x, neighbors[0].y);
    int segments = neighbors[MAX_VEHICLE_SIZE - 1].x;
    Serial.print("Num Segments to move: ");
    Serial.println(segments);
    for (int i = 0; i < segments; i++)
    {
        setPixel(neighbors[i].x, neighbors[i].y, NON);
    }

    for (int i = 0; i < segments; i++)
    {
        if (neighbors[i].x == -1 || neighbors[i].y == -1)
        {
            continue;
        }

        switch (direction)
        {
        case 12:
            setPixel(neighbors[i].x, neighbors[i].y - 1, colorId);
            break;
        case 3:
            setPixel(neighbors[i].x + 1, neighbors[i].y, colorId);
            break;
        case 6:
            setPixel(neighbors[i].x, neighbors[i].y + 1, colorId);
            break;
        case 9:
            setPixel(neighbors[i].x - 1, neighbors[i].y, colorId);
            break;
        }
    }
}

bool noCollision(int posX, int posY, int allowedColor)
{
    int colorId = getPixel(posX, posY);
    if (colorId == allowedColor)
    {
        return true;
    }
    if (colorId == NON)
    {
        return true;
    }
    if (colorId == EXT)
    {
        // WIN
        // TODO: WIN
        return true;
    }
    
    return false;
}

void printNeighbors()
{
    for (int i = 0; i < MAX_VEHICLE_SIZE; i++)
    {
        Serial.print(i);
        Serial.print(": ");
        Serial.print(neighbors[i].x);
        Serial.print(", ");
        Serial.println(neighbors[i].y);
    }
}

void moveCursor(int direction)
{
    switch (direction)
    {
    case 12:
        cursorY--;
        if (cursorY < 0)
            cursorY = 0;
        break;
    case 3:
        cursorX++;
        if (cursorX >= BOARD_SIZE)
            cursorX = BOARD_SIZE - 1;
        break;
    case 6:
        cursorY++;
        if (cursorY >= BOARD_SIZE)
            cursorY = BOARD_SIZE - 1;
        break;
    case 9:
        cursorX--;
        if (cursorX < 0)
            cursorX = 0;
        break;
    }
}

void checkButtons()
{
    /*    C4 C3 C2 C1
       . R1 X  X  X  X
       . R1 X  X  X  X
       . R2 X  X  X  X
       . R3 X  X  X  X
    */

    // COL1
    digitalWrite(COL1_PIN, LOW);
    digitalWrite(COL2_PIN, LOW);
    digitalWrite(COL3_PIN, LOW);
    digitalWrite(COL1_PIN, HIGH);
    if (digitalRead(ROW3_PIN))
    {
        // right
        checkButtonPress(3);
        return;
    }

    // COL2
    digitalWrite(COL1_PIN, LOW);
    digitalWrite(COL2_PIN, LOW);
    digitalWrite(COL3_PIN, LOW);
    digitalWrite(COL2_PIN, HIGH);
    if (digitalRead(ROW3_PIN))
    {
        // select
        checkButtonPress(1);
        return;
    }
    if (digitalRead(ROW2_PIN))
    {
        // up
        checkButtonPress(12);
        return;
    }
    if (digitalRead(ROW4_PIN))
    {
        // down
        checkButtonPress(6);
        return;
    }

    // COL3
    digitalWrite(COL1_PIN, LOW);
    digitalWrite(COL2_PIN, LOW);
    digitalWrite(COL3_PIN, LOW);
    digitalWrite(COL3_PIN, HIGH);
    if (digitalRead(ROW3_PIN))
    {
        // left
        checkButtonPress(9);
        return;
    }

    // Nothing
    checkButtonPress(0);
}

void checkButtonPress(int button)
{
    if (state != button)
    {
        // change
        Serial.println(button);
 
        state = button;
        if (button == 1)
        {
            cursorState = !cursorState;
            return;
        }
        if (cursorState == CURSOR_SELECTED)
        {
            getNeighboringPixels(cursorX, cursorY);
            int carSegments = neighbors[MAX_VEHICLE_SIZE - 1].x;
            int dir = neighbors[MAX_VEHICLE_SIZE - 1].y;
            Serial.print("direction: ");
            Serial.println(dir);
            Serial.print("Segments: ");
            Serial.println(carSegments);
            if (dir == -1)
            {
                return;
            }
            if ((dir == X_AXIS && (button == 3 || button == 9)) || (dir == Y_AXIS && (button == 12 || button == 6)))
            {
                bool noCollission = noNeighboursCollision(button);
                Serial.print("collision: ");
                if (noCollission == true)
                {
                    Serial.println("false");
                    moveNeighbours(button);
                    moveCursor(button);
                }
                else
                {
                    Serial.println("true");
                }
            }
        }
        else
        {
            moveCursor(button);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness(BRIGHTNESS);
    pinMode(COL1_PIN, OUTPUT);
    pinMode(COL2_PIN, OUTPUT);
    pinMode(COL3_PIN, OUTPUT);
    pinMode(COL4_PIN, OUTPUT);

    pinMode(ROW1_PIN, INPUT);
    pinMode(ROW2_PIN, INPUT);
    pinMode(ROW3_PIN, INPUT);
    pinMode(ROW4_PIN, INPUT);
}

void loop()
{
    uint32_t ms = millis();
    DrawOneFrame(ms % BLINK_RATE + 1);
    FastLED.show();
    delay(10);

    checkButtons();
}