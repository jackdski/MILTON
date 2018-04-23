/*
 * ECEN 2270
 * Bluetooth.h
 *  Bluetooth UART communication for MILTON
 *
 * Jack Danielski
 *
 */

//Connect 9.7 to the RX
void configureBluetooth();
void sendCommands(int16_t x, int16_t y);
void sendCommand(uint8_t xPer, uint8_t yPer, uint8_t xDir, uint8_t yDir);
