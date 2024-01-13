#include "email_sender.h"
#include <stdio.h>


int main() {
	 FILE *finalize = fopen("work.txt", "wb");
    	if (!finalize) {
        	fprintf(stderr, "Failed to open file for writing.\n");
        	return 1;
    }
    fprintf(finalize,"hello");
    fclose(finalize);
    
    const char *to = "rsaifur388@gmail.com";
    const char *file_path = "work.txt";
    

    int result = send_email_with_attachment(to, file_path);

    if (result == 0) {
        printf("Email sent successfully.\n");
    } else {
        printf("Failed to send email.\n");
    }

    return 0;
}
