/* Click Configurations for CLICK_CFG_A
 * -----------------------------------------
 * | -- | -- | ZD | ZS | YD | YS | XD | XS |
 * -----------------------------------------
 * ZD: Double click Z axis
 * ZS: Single click Z axis
 * YD: Double click Y axis
 * YS: Single click Y axis
 * XD: Double click X axis
 * XS: Single click X axis
 */
#define  DOUBLE_Z   0b00100000
#define  SINGLE_Z   0b00010000
#define  DOUBLE_Y   0b00001000
#define  SINGLE_Y   0b00000100
#define  DOUBLE_X   0b00000010
#define  SINGLE_X   0b00000001
#define  DOUBLE_ALL 0b00101010
#define  SINGLE_ALL 0b00010101

/* Click Configurations for CLICK_SRC_A
 * -------------------------------------------------------
 * | -- | IA | DBCLICK | SCLICK | Sign |  Z  |  Y  |  X  |
 * -------------------------------------------------------
 * IA: Interrupt Active?
 * DBCLICK: Double Click Enabled?
 * SCLICK: Single Click Enabled?
 * Sign: (default: 0) 0 = Positive detect, 1 = Negative detect
 * Z: Z click detected?
 * Y: Y click detected?
 * X: X click detected?
 */
#define Z_DOUBLE_CLICK 0b01100100
#define Y_DOUBLE_CLICK 0b01100010
#define X_DOUBLE_CLICK 0b01100001

enum functionTypes {
  CALIBRATE,
  COMPASS,
  FLASHLIGHT_COOL,
  FLASHLIGHT_WARM,
  LIGHTNING_COOL,
  LIGHTNING_WARM,
  BREATH,
  RAINBOW,
  RAINBOW_BREATH,
  OFF,
  LAST
};
