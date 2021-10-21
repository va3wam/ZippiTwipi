# LiquidCrystal_I2C
LiquidCrystal Arduino library for the DFRobot I2C LCD displays

Modified to support specifying SCL SDA ports when initialising LCD. 

### LiquidCrystal_I2C.h 

```
void init();
```

changed to 

```
void init(uint8_t SDA, uint8_t SCL_PORT);
```

and 

```
void init_priv();
```
changed to

```
void init_priv(uint8_t SDA_PORT, uint8_t SCL_PORT);
```

### LiquidCrystal_I2C.cpp

Changed init to this:

```
void LiquidCrystal_I2C::init(uint8_t SDA_PORT, uint8_t SCL_PORT){
        init_priv(SDA_PORT, SCL_PORT);
}
```

and init_priv to this:

```
void LiquidCrystal_I2C::init_priv(uint8_t SDA_PORT, uint8_t SCL_PORT)
{
        Wire.begin(SDA_PORT, SCL_PORT);
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
        begin(_cols, _rows);
}
```

In the application code you can now do this:

```
LiquidCrystal_I2C lcd(0x27,20,4);

lcd.init(2,14);

```
