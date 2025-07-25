#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_leap_year(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void get_calendar_matrix(int year, int month, char cal[7][7][4]) {
  // Leere alle Felder mit "" (wichtiger Fix!)
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      strcpy(cal[i][j], "");
    }
  }

  // Anzahl Tage im Monat
  int days_in_month[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int num_days = days_in_month[month - 1];

  // Zeller's Kongruenz – Monats- & Jahresanpassung
  int zeller_month = month;
  int zeller_year = year;
  if (zeller_month < 3) {
    zeller_month += 12;
    zeller_year -= 1;
  }

  // Zeller's Formel: 0=Sa, 1=So, ..., 6=Fr
  int h = (1 + (13 * (zeller_month + 1)) / 5 + zeller_year + zeller_year / 4 - zeller_year / 100 + zeller_year / 400) % 7;

  // Wochentag auf Montag=0, Sonntag=6 mappen
  int start_weekday = (h + 5) % 7;

  // Wochentag-Header (erste Zeile)
  const char* weekdays[7] = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"};
  for (int i = 0; i < 7; i++) {
    strncpy(cal[0][i], weekdays[i], 4);
  }

  // Tage einfüllen
  int current_day = 1;
  for (int row = 1, col = start_weekday; current_day <= num_days;) {
    for (; col < 7 && current_day <= num_days; col++) {
      snprintf(cal[row][col], 4, "%d", current_day++);
    }
    col = 0;
    row++;
  }

  // (optional) Leere restliche Felder – bereits durch Initialisierung oben abgedeckt
}


// Convert a date to Julian Day Number
long date_to_jdn(int year, int month, int day) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    return jdn;
}

// Parse a date string "YYYY-MM-DD" and convert to integers
void parse_date(const char *date_str, int *year, int *month, int *day) {
    char date_copy[11]; // Buffer to hold a copy of the date string
    strncpy(date_copy, date_str, 10); // Make a copy of the date string
    date_copy[10] = '\0'; // Ensure null-termination
    
    char *token = strtok(date_copy, "-"); // Split the string by '-'
    if (token) {
        *year = atoi(token);
        token = strtok(NULL, "-");
        if (token) {
            *month = atoi(token);
            token = strtok(NULL, "-");
            if (token) {
                *day = atoi(token);
            }
        }
    }
}

// Calculate the difference in days between two dates given in "YYYY-MM-DD" format
int days_between_dates(const char *start_date, const char *end_date) {
    int start_year, start_month, start_day;
    int end_year, end_month, end_day;
    
    // Parse start date
    parse_date(start_date, &start_year, &start_month, &start_day);
    
    // Parse end date
    parse_date(end_date, &end_year, &end_month, &end_day);
    
    long start_jdn = date_to_jdn(start_year, start_month, start_day);
    long end_jdn = date_to_jdn(end_year, end_month, end_day);
    return (int)(end_jdn - start_jdn);
}
