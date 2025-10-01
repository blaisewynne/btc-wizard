#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
struct variables {
  CURL *curl;
  CURLcode res;
  FILE *fptr;
  FILE *output;
  char buffer[12];
  char *end;
  char btc_value[12];
  time_t current_time;
  char text[100];
};

void get_txt() {
  struct variables env;
  env.fptr = fopen("logo1.txt", "r");
  if (env.fptr == NULL) {
    perror("No text found");
  }

  while (fgets(env.text, sizeof(env.text), env.fptr) != NULL) {
    printf("\x1b[0;32m%s", env.text);
  }

  env.fptr = fopen("logo2.txt", "r");
  if (env.fptr == NULL) {
    perror("No text found");
  }

  while (fgets(env.text, sizeof(env.text), env.fptr) != NULL) {
    printf("\x1b[0;34m%s\x1b[0;32m", env.text);
  }

  fclose(env.fptr);

}

void get_value() {
  struct variables env;
  env.curl = curl_easy_init();
  if(env.curl) {
    env.fptr = fopen("data.temp", "w");
    curl_easy_setopt(env.curl, CURLOPT_URL, "https://www.google.com/finance/quote/BTC-AUD");
    curl_easy_setopt(env.curl, CURLOPT_WRITEDATA, env.fptr);
    env.res = curl_easy_perform(env.curl);
    fclose(env.fptr);
    curl_easy_cleanup(env.curl);
  }
}

char remove_comma(char *str, char c) {
  char *pr = str, *pw = str;
  while (*pr) {
    *pw = *pr++;
    pw += (*pw != c);
  }
  *pw = '\0';
}

int main() {
  struct variables env;
  while (true) {
  get_value();
  time(&env.current_time);
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.temp", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    printf("\033[H");
    printf("\033[2J");
    get_txt();
    printf("\x1b[0;33mCurrent Value (₿): \x1b[1;32m%s\x1b[0;37m", env.buffer);
    printf("\x1b[0;34mCurrent Time: \x1b[4;36m%s\x1b[0;37m", ctime(&env.current_time));
    env.fptr = fopen("history.temp", "a");
    fprintf(env.fptr, "%s, %s\n", env.buffer, ctime(&env.current_time));
    fclose(env.fptr);
    strcpy(env.btc_value, env.buffer);
    remove_comma(env.btc_value, ',');
    double value = atof(env.btc_value);
    printf("\x1b[0;35mValue: ");
    for (int i = 0; i < ((value / 200000) * 100 / 2 ); i++) {
      printf("\x1b[01;34m#");
    }
    printf("\n");
  }
  }
}

