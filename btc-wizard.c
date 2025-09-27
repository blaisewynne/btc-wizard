#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdbool.h>
struct variables {
  CURL *curl;
  CURLcode res;
  FILE *fptr;
  FILE *output;
  char buffer[1024];
  char *end;
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
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.txt", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    remove_comma(env.buffer, ',');
    printf("%s", env.buffer);
  }
  }
}
