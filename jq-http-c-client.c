#include "jq-http-c-client.h"
#include <stdlib.h>  //malloc
#include <string.h>
#include "cJSON.h"


jq_client_ptr client;

varchar get_token(varchar url,  varchar mob, varchar pwd){

  if(client->curl) {

    cJSON *jsonstr = cJSON_CreateObject();
    cJSON_AddStringToObject(jsonstr, "method", "get_token");    
    cJSON_AddStringToObject(jsonstr, "mob", mob);        
    cJSON_AddStringToObject(jsonstr, "pwd", pwd);        


    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_easy_setopt(client->curl, CURLOPT_HTTPHEADER, headers); 

    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    /* Now specify the POST data */
    curl_easy_setopt(client->curl, CURLOPT_POSTFIELDS,cJSON_Print(jsonstr));

    /* Perform the request, res will get the return code */
    client->res = curl_easy_perform(client->curl);
    /* Check for errors */


    if(client->res != CURLE_OK){
	    fprintf(stderr, "curl_easy_perform() failed: s\n",
              curl_easy_strerror(client->res));
          /* always cleanup */
	    curl_easy_cleanup(client->curl);
		curl_global_cleanup();
	    return curl_easy_strerror(client->res);
    }

    /* always cleanup */
    curl_easy_cleanup(client->curl);
  }

  curl_global_cleanup();

  varchar result = curl_easy_strerror(client->res);
  strncpy(client->m_token, result,0);
  return client->m_token;

}


jq_client_ptr jq_client_new(void){

  jq_client_ptr out = malloc(sizeof(jq_client));

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  *out = (jq_client) {.curl = curl_easy_init(), .get_token = get_token};
  // *out = (jq_client) {.curl = curl_easy_init()};
  
  if (out->curl){     
     return out;
  }

}



int main(void){

  client  = jq_client_new(); 

  varchar url = "https://dataapi.joinquant.com/apis";
  varchar mob = "19925473023";
  varchar pwd = "KoKo150118";

  varchar token =  client->get_token(url, mob, pwd);
  // varchar token =  get_token(url, mob, pwd);  

  // char result[40] ;
  // strncpy(result, token,1);

  printf("%s\n", token);
}
