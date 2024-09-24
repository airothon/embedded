// debug.h
#ifndef DEBUG_H
#define DEBUG_H

// #define DEBUG
// DEBUG modu için LOG makrosu
#ifdef DEBUG
  #define LOG(x) Serial.print(x)
  #define LOGLN(x) Serial.println(x)
#else
  #define LOG(x)
  #define LOGLN(x)
#endif

#endif // DEBUG_H
