#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	int tile_idx;
	int edges[4];
}
tile_t;

int random_edge(int);
int erand(int);

int main(void) {
	int cols_n, rows_n, border_edges_n, inner_edges_n, rotate, tiles_n, *used, i;
	tile_t *tiles;
	if (scanf("%d%d%d%d%d", &cols_n, &rows_n, &border_edges_n, &inner_edges_n, &rotate) != 5 || cols_n < 2 || rows_n < 2 || border_edges_n < 1 || inner_edges_n < 1) {
		fprintf(stderr, "Invalid puzzle settings\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	tiles_n = cols_n*rows_n;
	tiles = malloc(sizeof(tile_t)*(size_t)tiles_n);
	if (!tiles) {
		fprintf(stderr, "Could not allocate memory for tiles\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	used = calloc((size_t)tiles_n, sizeof(int));
	if (!used) {
		fprintf(stderr, "Could not allocate memory for used\n");
		fflush(stderr);
		free(tiles);
		return EXIT_FAILURE;
	}
	srand((unsigned)time(NULL));
	for (i = 0; i < tiles_n; i++) {
		int row_idx = i/cols_n, col_idx = i%cols_n;
		do {
			tiles[i].tile_idx = erand(tiles_n);
		}
		while (used[tiles[i].tile_idx]);
		used[tiles[i].tile_idx] = 1;
		if (row_idx == 0) {
			tiles[i].edges[0] = 0;
		}
		else {
			tiles[i].edges[0] = -tiles[i-cols_n].edges[2];
		}
		if (col_idx == cols_n-1) {
			tiles[i].edges[1] = 0;
		}
		else {
			if (row_idx == 0 || row_idx == rows_n-1) {
				tiles[i].edges[1] = random_edge(border_edges_n);
			}
			else {
				tiles[i].edges[1] = random_edge(inner_edges_n);
			}
		}
		if (row_idx == rows_n-1) {
			tiles[i].edges[2] = 0;
		}
		else {
			if (col_idx == 0 || col_idx == cols_n-1) {
				tiles[i].edges[2] = random_edge(border_edges_n);
			}
			else {
				tiles[i].edges[2] = random_edge(inner_edges_n);
			}
		}
		if (col_idx == 0) {
			tiles[i].edges[3] = 0;
		}
		else {
			tiles[i].edges[3] = -tiles[i-1].edges[1];
		}
	}
	if (rotate) {
		int j;
		for (j = 0; j < tiles_n; j++) {
			int r = erand(4), edges[4], k;
			for (k = 0; k < 4; k++) {
				edges[k] = tiles[j].edges[(k+r)%4];
			}
			for (k = 0; k < 4; k++) {
				tiles[j].edges[k] = edges[k];
			}
		}
	}
	printf("%d, %d, %d\n", cols_n, rows_n, rotate);
	for (i = 0; i < tiles_n; i++) {
		int tile_idx;
		do {
			tile_idx = erand(tiles_n);
		}
		while (used[tile_idx] == 2);
		used[tile_idx] = 2;
		printf("%d: %d, %d, %d, %d\n", tiles[tile_idx].tile_idx, tiles[tile_idx].edges[0], tiles[tile_idx].edges[1], tiles[tile_idx].edges[2], tiles[tile_idx].edges[3]);
	}
	free(used);
	free(tiles);
	return EXIT_SUCCESS;
}

int random_edge(int edges_n) {
	int edge = erand(edges_n)+1;
	if (erand(2) == 0) {
		return edge;
	}
	return -edge;
}

int erand(int max) {
	return (int)(rand()/(RAND_MAX+1.0)*max);
}
