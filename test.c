#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

int test() {
  struct variables env;
  get_value();
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.temp", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    strcpy(env.btc_value, env.buffer);
    remove_comma(env.btc_value, ',');
    double test = atof(env.btc_value);
  }
}

int test2() {
  struct variables env;
  get_value();
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.temp", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    //printf("%s", env.buffer);
    strcpy(env.btc_value, env.buffer);
    remove_comma(env.btc_value, ',');
    double test = atof(env.btc_value);
    //printf("%s", env.btc_value);
    //printf("%f", test);
    printf("Value: ");
    for (int i = 0.0; i < (179999 / 5000); i++) {
      printf("#");
    }
    printf("\n");
  }
}

int main() {
  printf("Test");
  test2();
}
