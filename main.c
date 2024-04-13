#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertTimingPoints(const char* inputFilePath) {
    char outputFilePath[256];
    strcpy(outputFilePath, inputFilePath);
    strcat(outputFilePath, ".osu"); 

    FILE *inputFile = fopen(inputFilePath, "r");
    FILE *outputFile = fopen(outputFilePath, "w");

    if (!inputFile || !outputFile) {
        printf("Error opening files!\n");
        return;
    }

    float startTime, endTime, startBar, endBar, timeSignature, barsCovered;
    float previousTime = 0;
    int isFirstPoint = 1;


    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        if (sscanf(line, "%f;%f;%f;%f", &endTime, &endBar, &timeSignature, &barsCovered) == 4) {
            if (!isFirstPoint) {
                float durationMs = (endTime - startTime) * 1000;
                float beatsCovered = (endBar - startBar) * timeSignature;
                float beatLength = durationMs / beatsCovered;
                fprintf(outputFile, "%d,%f,4,2,0,20,1,0\n", (int)(previousTime - 60), beatLength);
            } else {
                isFirstPoint = 0;
            }
            previousTime = endTime * 1000;
            startTime = endTime;
            startBar = endBar;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Conversion completed successfully. Output file: %s\n", outputFilePath);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file_path>\n", argv[0]);
        return 1;
    }

    convertTimingPoints(argv[1]);
    return 0;
}
