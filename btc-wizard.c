#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

struct variables {
  CURL *curl;
  CURLcode res;
  FILE *fptr;
  FILE *output;
  char buffer[1024];
};

int get_value() {
  struct variables env;
  env.curl = curl_easy_init();
  if(env.curl) {
    env.fptr = fopen("data.txt", "w");
    curl_easy_setopt(env.curl, CURLOPT_URL, "https://www.google.com/finance/quote/BHP:ASX");
    curl_easy_setopt(env.curl, CURLOPT_WRITEDATA, env.fptr);
    env.res = curl_easy_perform(env.curl);
    fclose(env.fptr);
    curl_easy_cleanup(env.curl);
  }
}

int main() {
  struct variables env;
  get_value();
  env.output = popen("grep -Po '(?<=<div class=\"YMlKec fxKbKc\">)(.*?)(?=</div>)' data.txt", "r");
  while (fgets(env.buffer, sizeof(env.buffer), env.output) != NULL) {
    printf("%s", env.buffer);
  }
}
