#include <pebble.h>

int convertTime(char *time) {
	//hour
	char *hour = (char*) malloc(3);
	strncpy(hour, time, 2);
	//sec
	char *min = (char*) malloc(3);
	strncpy(min, time+2, 2);

	return atoi(hour)*60 + atoi(min);
}

char* concatStr(char *s1, char *s2)
{
	size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
	char *output = (char *)malloc(len1+len2+4);
	snprintf(output, len1+len2+4, "%s - %s", s1, s2);
  	return output;
 }