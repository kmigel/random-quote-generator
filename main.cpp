#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

size_t curl_callback(void *ptr, size_t size, size_t nmemb, string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if(curl) {
        string response_string;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: libcurl-agent/1.0");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Cache-Control: no-cache");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_URL, "https://quote-garden.onrender.com/api/v3/quotes/random");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
            curl_easy_cleanup(curl);
            return 1;
        }
        else {
            int ind = 0;
            while(response_string[ind - 2] != 't' || response_string[ind - 1] != '\"' || response_string[ind] != ':') ind++;
            ind++;
            while(response_string[ind - 1] != '\"' || response_string[ind] != ',') cout << response_string[ind++];
            cout << " - ";
            ind += 16;
            while(response_string[ind + 1] != '"') cout << response_string[ind++];
            cout << "\n";
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return 0;
}