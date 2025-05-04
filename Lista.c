#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 1024
#define MAX_POINTS 100
#define MAX_LINES 1000  // Para armazenar até 1000 linhas de entrada

typedef struct {
    double x, y;
} Point;

double euclidean_distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

double distance_to_origin(Point p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

int compare_points(const void *a, const void *b) {
    Point *pa = (Point *)a;
    Point *pb = (Point *)b;
    double da = distance_to_origin(*pa);
    double db = distance_to_origin(*pb);
    if (da < db) return -1;
    else if (da > db) return 1;
    return 0;
}

int main() {
    FILE *input = fopen("L0Q1.in", "r");
    FILE *output = fopen("L0Q1.out", "w");

    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char lines[MAX_LINES][MAX_LINE];
    int total_lines = 0;

    while (fgets(lines[total_lines], sizeof(lines[0]), input)) {
        total_lines++;
    }

    for (int l = 0; l < total_lines; l++) {
        char *line = lines[l];
        Point original_points[MAX_POINTS];
        Point sorted_points[MAX_POINTS];
        int count = 0;

        char *token = strtok(line, "()");
        while (token != NULL) {
            double x, y;
            if (sscanf(token, "%lf,%lf", &x, &y) == 2) {
                original_points[count].x = x;
                original_points[count].y = y;
                sorted_points[count] = original_points[count];
                count++;
            }
            token = strtok(NULL, "()");
        }

        qsort(sorted_points, count, sizeof(Point), compare_points);

        fprintf(output, "points");
        for (int i = 0; i < count; i++) {
            fprintf(output, " (%.0f,%.0f)", sorted_points[i].x, sorted_points[i].y);
        }

        double distance = 0;
        for (int i = 0; i < count - 1; i++) {
            distance += euclidean_distance(original_points[i], original_points[i+1]);
        }

        double shortcut = euclidean_distance(original_points[0], original_points[count - 1]);

        fprintf(output, " distance %.2f shortcut %.2f", distance, shortcut);

        if (l < total_lines - 1) {
            fprintf(output, "\n");
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
