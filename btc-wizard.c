#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <time.h>

struct variables {
  CURL *curl;
  CURLcode res;
  FILE *fptr;
  FILE *output;
  char buffer[1024];
  char *end;
  float btc_value;
  time_t current_time;
};

int get_value() {
  struct variables env;
  env.curl = curl_easy_init();
  if(env.curl) {
    env.fptr = fopen("data.txt", "w");
    curl_easy_setopt(env.curl, CURLOPT_URL, "https://www.google.com/finance/quote/BTC-AUD");
    curl_easy_setopt(env.curl, CURLOPT_WRITEDATA, env.fptr);
    env.res = curl_easy_perform(env.curl);
    fclose(env.fptr);
    curl_easy_cleanup(env.curl);
  }
}

char remove_comma(char * str, char c) {
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
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.txt", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    printf("\033[H");
    printf("\033[2J");
    env.btc_value = strtold(env.buffer, &env.end);
    printf("\x1b[0;33mCurrent Value: \x1b[1;32m%s\x1b[0;37m", env.buffer);
    printf("\x1b[0;34mCurrent Time: \x1b[4;36m%s\x1b[0;37m", ctime(&env.current_time));
    //printf("%.2f\n", env.btc_value);
  }
  }
}

