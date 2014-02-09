#include <pebble.h>

int convertTime(int timeHH, int timeMM) {
	return timeHH*3600 + timeMM*60;
}

char* convertToTimeFMT(int s1HH, int s1MM, int s2HH, int s2MM)
{
	char *s12_1;
	char *s12_2;

	if(s1HH <= 0) {
		s1HH = 12;
		s12_1 = "AM";
	}
	else if(s1HH > 0 && s1HH < 12) {
		s12_1 = "AM";
	}
	else if(s1HH == 12) {
		s12_1 = "PM";
	}
	else {
		s12_1 = "PM";
		s1HH = s1HH-12;
	}


	if(s2HH <= 0) {
		s2HH = 12;
		s12_2 = "AM";
	}
	else if(s2HH > 0 && s2HH < 12) {
		s12_2 = "AM";
	}
	else if(s2HH == 12) {
		s12_2 = "PM";
	}
	else {
		s12_2 = "PM";
		s2HH = s2HH-12;
	}

	char *output = (char *)malloc(20);
	snprintf(output, 20, "%02d:%02d %s - %02d:%02d %s", s1HH, s1MM, s12_1, s2HH, s2MM, s12_2);
  	//snprintf(output, 20, "%d,", s1HH);
  	return output;
 }

 char* convertToTimerFMT(int seconds) {
 	char *output = (char *)malloc(9);
 	int temp = seconds;
 	int hr = (int)temp/3600;
 	temp -= hr*3600;
 	int min = (int)temp/60;
 	temp -= min*60;
 	int sec = temp;
 	snprintf(output, 9, "%02d:%02d:%02d", hr, min, sec);
 	return output;
 }