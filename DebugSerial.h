#ifndef DEBUG_SERIAL_H_
#define DEBUG_SERIAL_H_

#define ENABLE_DEBUG_SERIAL 1

#if defined(ENABLE_DEBUG_SERIAL)
#define XDEBUG_SERIAL Serial
#define xSerialBegin(baud) XDEBUG_SERIAL.begin(baud)
#define xDebug(...) XDEBUG_SERIAL.print(__VA_ARGS__)
#define xDebugLn(...) XDEBUG_SERIAL.print(__VA_ARGS__)
#define xFDebug(s) XDEBUG_SERIAL.print(F(s))
#define xFDebugLn(s) XDEBUG_SERIAL.print(F(s))
#else
#define xSerialBegin(baud)
#define xDebug(...)
#define xDebugLn(...)
#define xFDebug(s)
#define xFDebugLn(s)
#endif

#endif  // DEBUG_SERIAL_H_
