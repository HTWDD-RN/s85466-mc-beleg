#include <FastLED.h>
#include <Math.h>

#define LED_PIN 4

#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define NUM_CARS 13

#define BRIGHTNESS 64
#define DEFAULT_COLOR CRGB(10, 10, 7)

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

// Param for different pixel layouts
const bool kMatrixSerpentineLayout = true;
const bool kMatrixVertical = false;

struct Car
{
    CRGB color;
    uint8_t type;
    uint8_t pos[2];
    uint8_t direction;
    uint8_t length;
};

enum CarType
{
    NON,
    CAR,
    REDCAR,
    CURSOR
};

enum Direction
{
    VERTICAL,
    HORIZONTAL
};

struct Car cars[10];

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

struct Car getCarAt(int posX, int posY)
{
    for (int i = 0; i < NUM_CARS; i++)
    {
        struct Car car = cars[i];

        if (car.type == CAR || car.type == REDCAR)
        {

            // Serial.print("X3: ");
            // Serial.println(posX);
            // Serial.print("Y3: ");
            // Serial.println(posY);

            if (getIntersection(car, posX, posY))
            {
                Serial.println("FOUND CAR");
                return car;
            }
        }
    }
    struct Car car1 = {DEFAULT_COLOR, NON};
    return car1;
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

void renderFrame()
{
    for (int posX = 0; posX < 16; posX++)
    {
        for (int posY = 0; posY < 16; posY++)
        {
            struct Car car = getCarAt(posX, posY);

            Serial.print("(");
            Serial.print(posX);
            Serial.print(",");
            Serial.print(posY);
            Serial.println(") ");

            // Serial.print(car.color.red);
            // Serial.print(" ");
            // Serial.print(car.color.green);
            // Serial.print(" ");
            // Serial.println(car.color.blue);

            leds[XY(posX, posY)] = car.color;
            FastLED.show();
        }
    }
}

bool getIntersection(struct Car car, int x, int y)
{
    // Serial.print("X2: ");
    // Serial.println(car.pos[0]);
    // Serial.print("Y2: ");
    // Serial.println(car.pos[1]);

    if (car.direction == HORIZONTAL)
    {
        if (car.pos[1] == y)
        {
            return (car.pos[0] + car.length > x) && (car.pos[0] <= x);
        }
    }
    else if (car.direction == VERTICAL)
    {
        if (car.pos[0] == x)
        {
            bool ex = (car.pos[1] + car.length > y) && (car.pos[1] <= y);
            // Serial.print("EX: ");
            // Serial.println(ex);
            return ex;
        }
    }
    return false;
}

void setup()
{
    Serial.begin(115200);

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness(BRIGHTNESS);

    {
        // Cursor
        cars[0].color = CRGB(0, 0, 0);
        cars[0].type = CURSOR;
        cars[0].pos[0] = 3;
        cars[0].pos[1] = 3;
        cars[0].length = 1;

        // RED CAR
        cars[1].color = CRGB(255, 0, 0);
        cars[1].type = REDCAR;
        cars[1].pos[0] = 3;
        cars[1].pos[1] = 2;
        cars[1].direction = HORIZONTAL;
        cars[1].length = 2;

        // CAR lower left
        cars[2].color = CRGB(0, 255, 0);
        cars[2].type = CAR;
        cars[2].direction = HORIZONTAL;
        cars[2].pos[0] = 0;
        cars[2].pos[1] = 5;
        cars[2].length = 2;

        // TRUCK lower right
        cars[3].color = CRGB(255, 255, 0);
        cars[3].type = CAR;
        cars[3].direction = HORIZONTAL;
        cars[3].pos[0] = 3;
        cars[3].pos[1] = 5;
        cars[3].length = 3;

        // Turquoise car
        cars[4].color = CRGB(0, 0, 255);
        cars[4].type = CAR;
        cars[4].direction = HORIZONTAL;
        cars[4].pos[0] = 1;
        cars[4].pos[1] = 4;
        cars[4].length = 2;

        // Brown car
        cars[5].color = CRGB(0, 255, 255);
        cars[5].type = CAR;
        cars[5].direction = VERTICAL;
        cars[5].pos[0] = 3;
        cars[5].pos[1] = 3;
        cars[5].length = 2;

        // light green car
        cars[6].color = CRGB(255, 0, 255);
        cars[6].type = CAR;
        cars[6].direction = HORIZONTAL;
        cars[6].pos[0] = 4;
        cars[6].pos[1] = 3;
        cars[6].length = 2;

        // green car 1
        cars[7].color = CRGB(255, 255, 255);
        cars[7].type = CAR;
        cars[7].direction = VERTICAL;
        cars[7].pos[0] = 0;
        cars[7].pos[1] = 2;
        cars[7].length = 2;

        // green car 2
        cars[8].color = CRGB(255, 255, 255);
        cars[8].type = CAR;
        cars[8].direction = VERTICAL;
        cars[8].pos[0] = 1;
        cars[8].pos[1] = 2;
        cars[8].length = 2;

        // orange truck
        cars[9].color = CRGB(255, 128, 128);
        cars[9].type = CAR;
        cars[9].direction = VERTICAL;
        cars[9].pos[0] = 5;
        cars[9].pos[1] = 0;
        cars[9].length = 3;

        // brown pickup top
        cars[10].color = CRGB(128, 255, 255);
        cars[10].type = CAR;
        cars[10].direction = VERTICAL;
        cars[10].pos[0] = 4;
        cars[10].pos[1] = 0;
        cars[10].length = 2;

        // white bus
        cars[11].color = CRGB(255, 255, 128);
        cars[11].type = CAR;
        cars[11].direction = VERTICAL;
        cars[11].pos[0] = 2;
        cars[11].pos[1] = 0;
        cars[11].length = 3;

        // yellow car
        cars[12].color = CRGB(255, 128, 255);
        cars[12].type = CAR;
        cars[12].direction = HORIZONTAL;
        cars[12].pos[0] = 0;
        cars[12].pos[1] = 0;
        cars[12].length = 2;
    }

    // struct Car car = getCarAt(2, 0);

    // Serial.print(car.color.red);
    // Serial.print(",");
    // Serial.print(car.color.green);
    // Serial.print(",");
    // Serial.println(car.color.blue);
}

void loop()
{
    renderFrame();

    while (1)
    {
    }
}