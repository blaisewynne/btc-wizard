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
    env.btc_value = strtold(env.buffer, &env.end);
    printf("\rCurrent Value: %s", env.buffer);
    printf("\rCurrent Time: %s", ctime(&env.current_time));
    //printf("%.2f\n", env.btc_value);
  }
  }
}

