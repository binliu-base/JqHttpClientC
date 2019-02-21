#include <stdio.h>
#include <curl/curl.h>

typedef struct jq_client * jq_client_ptr;
typedef char * varchar; 

typedef struct jq_client
{
  CURL *curl;   
  CURLcode res;
  void (* jq_client_free)(jq_client_ptr  in);
  varchar (* get_token)(varchar url,  varchar mob, varchar pwd);
  char  m_token[40];
}jq_client;

jq_client *jq_client_new();
