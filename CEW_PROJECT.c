#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include "cjson/cJSON.h"
#include <string.h>
#include "email_sender.h"
#include <time.h>


// Data structure to store daily averages
struct DailyAverages {
    double temperature;
    double humidity;
    double wind_kph;
    double wind_mph;
    double feels_like;
    int count;  // Number of data points for the day
};

// Function to initialize daily averages
void initDailyAverages(struct DailyAverages *daily) {
    daily->temperature = 0.0;
    daily->humidity = 0.0;
    daily->wind_kph = 0.0;
    daily->wind_mph = 0.0;
    daily->feels_like = 0.0;
    daily->count = 0;
}

// Function to update daily averages
void updateDailyAverages(struct DailyAverages *daily, double temp, double hum, double wind_kph, double wind_mph, double feels_like) {
    daily->temperature += temp;
    daily->humidity += hum;
    daily->wind_kph += wind_kph;
    daily->wind_mph += wind_mph;
    daily->feels_like += feels_like;
    daily->count++;

}

// Function to calculate and print daily averages
void printDailyAverages(const struct DailyAverages *daily, const char *date) {
    if (daily->count >0) {
        //printf("Date: %s\n", date);
        //printf("Average Temperature: %.2fC\n", daily->temperature / daily->count);
        //printf("Average Humidity: %.2f\n", daily->humidity / daily->count);
        //printf("Average Wind_kph: %.2f\n", daily->wind_kph / daily->count);
        //printf("Average Wind_mph: %.2f\n", daily->wind_mph / daily->count);
        //printf("Average Feels Like: %.2fC\n", daily->feels_like / daily->count);
        //printf("\n");
        FILE *finalize = fopen("ReportFile.txt", "ab");
    	if (!finalize) {
        	fprintf(stderr, "Failed to open file for writing.\n");
        	return;
    }
    	fprintf(finalize,"\n");
    	fprintf(finalize,"Date: %s\n", date);
        fprintf(finalize,"Average Temperature: %.2fC\n", daily->temperature / daily->count);
        fprintf(finalize,"Average Humidity: %.2f\n", daily->humidity / daily->count);
        fprintf(finalize,"Average Wind_kph: %.2f\n", daily->wind_kph / daily->count);
        fprintf(finalize,"Average Wind_mph: %.2f\n", daily->wind_mph / daily->count);
        fprintf(finalize,"Average Feels Like: %.2fC\n", daily->feels_like / daily->count);
        fprintf(finalize,"\n");
        fclose(finalize);
    
    }
}

void printTodaysAverage(const struct DailyAverages *daily, const char *date) {
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    char current_date[11];
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", tm_info);

    if (strcmp(date, current_date) == 0 && daily->count > 0) {
        //printf("\033[1mToday's Date:\033[0m %s\n", date);
        //printf("Current Time: %02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        printf("\n");
    	printf("Today's Weather Forecast:\n");
    	printf("----------------------------\n");
    	printf("\n");
        printf("Average Temperature: %.2fC\n", daily->temperature / daily->count);
        printf("Average Humidity: %.2f\n", daily->humidity / daily->count);
        printf("Average Wind_kph: %.2f\n", daily->wind_kph / daily->count);
        printf("Average Wind_mph: %.2f\n", daily->wind_mph / daily->count);
        printf("Average Feels Like: %.2fC\n", daily->feels_like / daily->count);
        printf("\n");
        printf("\n");
    	printf("Weather Analysis:\n");
    	printf("----------------------------\n");
    	printf("\n");

        // Check for hot or cold weather
        if ((daily->temperature / daily->count) > 35) {
            printf("It is hot today!\n");
        } else if ((daily->temperature / daily->count) < 15) {
            printf("It is cold today!\n");
        } else{
            printf("It is neither cold nor hot today!!");
        }

        printf("\n");
    
        
        FILE *f_anomaly = fopen("Anomaly.txt", "ab");
    	if (!f_anomaly) {
        	fprintf(stderr, "Failed to open file for reading.\n");
        	return ;
    	}
    	fprintf(f_anomaly,"\n"); 
    	//fprintf(f_anomaly,"Time: %02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    	//fprintf(f_anomaly, "Date: %s\n", date);
    	fprintf(f_anomaly,"\n");
    	fprintf(f_anomaly,"Today's Weather Forecast:\n");
    	fprintf(f_anomaly,"----------------------------\n");
    	fprintf(f_anomaly,"\n");
        fprintf(f_anomaly,"Average Temperature: %.2fC\n", daily->temperature / daily->count);
        fprintf(f_anomaly,"Average Humidity: %.2f\n", daily->humidity / daily->count);
        fprintf(f_anomaly,"Average Wind_kph: %.2f\n", daily->wind_kph / daily->count);
        fprintf(f_anomaly,"Average Wind_mph: %.2f\n", daily->wind_mph / daily->count);
        fprintf(f_anomaly,"Average Feels Like: %.2fC\n", daily->feels_like / daily->count);
        fprintf(f_anomaly,"\n");
        fprintf(f_anomaly,"Weather Analysis:\n");
    	fprintf(f_anomaly,"----------------------------\n");
    	fprintf(f_anomaly,"\n");
        if ((daily->temperature / daily->count) > 30) {
            fprintf(f_anomaly,"It is very hot today so take the following precautions for your safety: \n");
            fprintf(f_anomaly,"-->Drink plenty of water to stay hydrated\n-->Avoid excessive consumption of caffeinated or alcoholic beverages, as they can contribute to dehydration.\n");
            fprintf(f_anomaly,"-->Wear sunglasses and a wide-brimmed hat to shield the face and eyes from the sun. \n");
            fprintf(f_anomaly,"-->Apply sunscreen with a high SPF to protect the skin from harmful UV rays.\n");          
        } else if ((daily->temperature / daily->count) < 25) {
            fprintf(f_anomaly,"It is very cold today so take the following precautions for your safety: \n");
            fprintf(f_anomaly,"-->Wear multiple layers of clothing to trap and retain body heat.\n-->Use thermal or insulated clothing as a base layer for better insulation.\n");
            fprintf(f_anomaly,"-->Wear gloves or mittens to protect hands from cold exposure. \n-->Use thermal socks and insulated boots to keep feet warm.\n");
            fprintf(f_anomaly,"-->Perform simple exercises to maintain circulation and warmth.\n");          
        } else{
            fprintf(f_anomaly,"It is neither cold nor hot today!!");
        }
        fclose(f_anomaly);
    }
}



size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void printCurrentWeather(cJSON *root) {
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    char current_date[11];
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", tm_info);

    cJSON *location = cJSON_GetObjectItem(root, "location");
    cJSON *current = cJSON_GetObjectItem(root, "current");

    if (location && current) {
        const char *city = cJSON_GetObjectItem(location, "name")->valuestring;
        const char *country = cJSON_GetObjectItem(location, "country")->valuestring;
        double temp_c = cJSON_GetObjectItem(current, "temp_c")->valuedouble;
        double humidity = cJSON_GetObjectItem(current, "humidity")->valuedouble;
        double wind_mph = cJSON_GetObjectItem(current, "wind_mph")->valuedouble;
        double wind_kph = cJSON_GetObjectItem(current, "wind_kph")->valuedouble;
        double feels_like_c = cJSON_GetObjectItem(current, "feelslike_c")->valuedouble;
        
        printf("\033[1mCurrent Weather in %s, %s:\033[0m\n", city, country);
        printf("Date/Time: %s / %02d:%02d:%02d\n", current_date, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
        printf("Temperature: %.2fC\n", temp_c);
        printf("Humidity: %.2f\n", humidity);
        printf("Wind Speed: %.2f mph / %.2f kph\n", wind_mph, wind_kph);
        printf("Feels Like: %.2fC\n", feels_like_c);
        printf("\n");
        FILE *f_anomaly = fopen("Anomaly.txt", "wb");
    	if (!f_anomaly) {
        	fprintf(stderr, "Failed to open file for reading.\n");
        	return ;
    	}
    	fprintf(f_anomaly,"\n");
    	fprintf(f_anomaly, "Current Weather in %s, %s:\n", city, country);
    	fprintf(f_anomaly,"Date/Time: %s / %02d:%02d:%02d\n", current_date, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    	//fprintf(f_anomaly, "Date: %s\n", current_date);
        fprintf(f_anomaly,"Temperature: %.2fC\n", temp_c);
        fprintf(f_anomaly,"Humidity: %.2f\n", humidity);
        fprintf(f_anomaly,"Wind Speed: %.2f mph / %.2f kph\n", wind_mph, wind_kph);
        fprintf(f_anomaly,"Feels Like: %.2fC\n", feels_like_c);
        fprintf(f_anomaly,"\n");
        fclose(f_anomaly);
    } 
}

//Write Forecast Data
void parse_hourly_data(cJSON *hourArray, const char *city_name) {
    
    cJSON *hour = NULL;
    struct DailyAverages daily;
    char current_date[11] = "";  // Format: YYYY-MM-DD

    initDailyAverages(&daily);

    cJSON_ArrayForEach(hour, hourArray) {
        const char *time = cJSON_GetObjectItem(hour, "time")->valuestring;
        const char *date = time ? time : "N/A";
        if (strncmp(date, current_date, 10) != 0) {
            // Print previous day's averages
            printDailyAverages(&daily, current_date);
            
            // Start calculating averages for the new day
            initDailyAverages(&daily);
            strncpy(current_date, date, 10);
        }

        double temp_c = cJSON_GetObjectItem(hour, "temp_c")->valuedouble;
        double humidity = cJSON_GetObjectItem(hour, "humidity")->valuedouble;
        double wind_mph = cJSON_GetObjectItem(hour, "wind_mph")->valuedouble;
        double wind_kph = cJSON_GetObjectItem(hour, "wind_kph")->valuedouble;
        double feelslike_c = cJSON_GetObjectItem(hour, "feelslike_c")->valuedouble;

        // Update daily averages
        updateDailyAverages(&daily, temp_c, humidity, wind_kph, wind_mph, feelslike_c);

        // Save formatted data into a file based on the city name
        char file_name[256];
    	snprintf(file_name, sizeof(file_name), "%s_data.txt", city_name);
    	FILE *forg = fopen(file_name, "ab");
    	if (!forg) {
        	fprintf(stderr, "Failed to open file for writing.\n");
        	return;
    }

        fprintf(forg, "Time: %s\n Temperature (C): %.2fC\n Humidity: %.2f\n Wind_mph: %.3f\n Wind_kph: %.3f\n Feels Like: %.2fC\n\n", time, temp_c, humidity, wind_mph, wind_kph, feelslike_c);
        fclose(forg);
    }

      // Print averages for the last day  
    printDailyAverages(&daily, current_date);
    
     // Print today's averages
    printTodaysAverage(&daily, current_date);


   
}

void parse_forecast_data(cJSON *forecast, const char *city_name) {
    cJSON *forecastday = cJSON_GetObjectItem(forecast, "forecastday");
    cJSON *day = NULL;
    cJSON_ArrayForEach(day, forecastday) {
        cJSON *hourArray = cJSON_GetObjectItem(day, "hour");
        if (hourArray) {
            // Parse and print hourly data for each day
            parse_hourly_data(hourArray, city_name);
        }
    }
}

int main() {
    char *city_name = "karachi";
    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://api.weatherapi.com/v1/forecast.json?key=e6ff049c207841f883e205341232412&q=karachi&days=3&aqi=no&alerts=no");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Key: e6ff049c207841f883e205341232412");
    headers = curl_slist_append(headers, "X-RapidAPI-Host: api.weatherapi.com");

    FILE *fp = fopen("api_response.txt", "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing.\n");
        return 1;
    }
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, fp);

    CURLcode ret = curl_easy_perform(hnd);

    // Clean up resources
    curl_slist_free_all(headers);
    curl_easy_cleanup(hnd);
    fclose(fp);

    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        return 1;
    }

    // Parse JSON response
    fp = fopen("api_response.txt", "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for reading.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    fread(json_data, 1, file_size, fp);
    json_data[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }

        // Clean up cJSON and close the file
        cJSON_Delete(root);
        fclose(fp);
        free(json_data);

        return 1;
    }

    cJSON *forecast = cJSON_GetObjectItem(root, "forecast");
    if (!forecast) {
        fprintf(stderr, "Failed to get 'forecast' object.\n");

        // Clean up cJSON and close the file
        cJSON_Delete(root);
        fclose(fp);
        free(json_data);

        return 1;
    }
    
     // After parsing the JSON response
    printCurrentWeather(root);

    // Parse and print the complete forecast data
     char file_name[256];
    	snprintf(file_name, sizeof(file_name), "%s_data.txt", city_name);
    	FILE *forg = fopen(file_name, "wb");
    	if (!forg) {
        	fprintf(stderr, "Failed to open file for writing.\n");
        	return 1;
    }
    parse_forecast_data(forecast, city_name);

    // Clean up cJSON and close the file
    cJSON_Delete(root);
    fclose(fp);
    free(json_data);
    
    

    // Call the header file
      const char *to = "hafizabdulbaseer565@gmail.com";
    const char *file_path = "Anomaly.txt";
    

    int result = send_email_with_attachment(to, file_path);

  

    if (result == 0) {
        printf("Email sent successfully!\n");
    } else {
        printf("Failed to send email. Error code: %d\n", result);
    }


    return 0;
}
