
//convert from 24 hour time HHMM to seconds in integer
int convertTime(int timeHH, int timeMM);
//convert from military time s1: HHMM s2: HHMM to 12-hour HH:MM - HH:MM
char* convertToTimeFMT(int s1HH, int s1MM, int s2HH, int s2MM);
//convert from seconds to HH:MM:SS
char* convertToTimerFMT(int seconds);