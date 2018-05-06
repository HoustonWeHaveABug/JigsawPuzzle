#include <stdio.h>
#include <stdlib.h>

typedef struct option_s option_t;

struct option_s {
	int tile_idx;
	int idx;
	int edge_u;
	int edge_r;
	int edge_d;
	int edge_l;
	option_t *same;
	option_t *last;
	option_t *next;
};

void update_min_max(int, int *, int *);
void set_option(option_t *, int, int, int, int, int, int);
void chain_option(option_t *, option_t *, option_t *);
void choose_option(int);
void print_option(option_t *);
int set_edge_lu_key(int);
int set_edge_rd_key(int);
int set_header_key(int, int, int, int);

int cols_n, rows_n, rotate, tiles_n, width, options_n, *used, edges_n, cell_max, solutions_n;
option_t *options, *headers, **cells;

int main(void) {
	int edge_min, edge_max, headers_n, i;
	if (scanf("%d, %d, %d", &cols_n, &rows_n, &rotate) != 3 || cols_n < 2 || rows_n < 2) {
		fprintf(stderr, "Invalid puzzle settings\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	tiles_n = cols_n*rows_n;
	for (i = tiles_n-1, width = 1; i > 9; i /= 10, width++);
	if (rotate) {
		options_n = tiles_n*4;
	}
	else {
		options_n = tiles_n;
	}
	options = malloc(sizeof(option_t)*(size_t)options_n);
	if (!options) {
		fprintf(stderr, "Could not allocate memory for options\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	used = calloc((size_t)tiles_n, sizeof(int));
	if (!used) {
		fprintf(stderr, "Could not allocate memory for used\n");
		fflush(stderr);
		free(options);
		return EXIT_FAILURE;
	}
	edge_min = 0;
	edge_max = 0;
	for (i = 0; i < tiles_n; i++) {
		int tile_idx, edge_u, edge_r, edge_d, edge_l;
		if (scanf("%d: %d, %d, %d, %d", &tile_idx, &edge_u, &edge_r, &edge_d, &edge_l) != 5 || tile_idx < 0 || tile_idx >= tiles_n || used[tile_idx]) {
			fprintf(stderr, "Invalid tile\n");
			fflush(stderr);
			free(used);
			free(options);
			return EXIT_FAILURE;
		}
		update_min_max(edge_u, &edge_min, &edge_max);
		update_min_max(edge_r, &edge_min, &edge_max);
		update_min_max(edge_d, &edge_min, &edge_max);
		update_min_max(edge_l, &edge_min, &edge_max);
		if (rotate) {
			set_option(options+i*4, tile_idx, 0, edge_u, edge_r, edge_d, edge_l);
			set_option(options+i*4+1, tile_idx, 1, edge_l, edge_u, edge_r, edge_d);
			set_option(options+i*4+2, tile_idx, 2, edge_d, edge_l, edge_u, edge_r);
			set_option(options+i*4+3, tile_idx, 3, edge_r, edge_d, edge_l, edge_u);
		}
		else {
			set_option(options+i, tile_idx, 0, edge_u, edge_r, edge_d, edge_l);
		}
		used[tile_idx] = 1;
	}
	if (edge_min+edge_max) {
		fprintf(stderr, "Invalid puzzle data\n");
		fflush(stderr);
		free(used);
		free(options);
		return EXIT_FAILURE;
	}
	edges_n = edge_max*2+1;
	headers_n = edges_n*edges_n*4;
	headers = malloc(sizeof(option_t)*(size_t)headers_n);
	if (!headers) {
		fprintf(stderr, "Could not allocate memory for headers\n");
		fflush(stderr);
		free(used);
		free(options);
		return EXIT_FAILURE;
	}
	for (i = 0; i < headers_n; i++) {
		headers[i].last = headers+i;
		headers[i].next = headers+i;
	}
	for (i = 0; i < options_n; i++) {
		int header_key = set_header_key(set_edge_lu_key(options[i].edge_l), set_edge_lu_key(options[i].edge_u), set_edge_rd_key(options[i].edge_r), set_edge_rd_key(options[i].edge_d));
		option_t *option;
		for (option = headers[header_key].last; option != headers+header_key && (option->edge_r != options[i].edge_r || option->edge_d != options[i].edge_d); option = option->last);
		if (option == headers+header_key) {
			options[i].same = NULL;
		}
		else {
			options[i].same = option;
		}
		chain_option(options+i, headers[header_key].last, headers+header_key);
	}
	cells = malloc(sizeof(option_t *)*(size_t)tiles_n);
	if (!cells) {
		fprintf(stderr, "Could not allocate memory for cells\n");
		fflush(stderr);
		free(headers);
		free(used);
		free(options);
		return EXIT_FAILURE;
	}
	cell_max = 0;
	solutions_n = 0;
	if (headers[3].next != headers+3) {
		used[headers[3].next->tile_idx] = 2;
		cells[0] = headers[3].next;
		choose_option(1);
		used[headers[3].next->tile_idx] = 1;
	}
	printf("Solutions %d\n", solutions_n);
	fflush(stdout);
	free(cells);
	free(headers);
	free(used);
	free(options);
	return EXIT_SUCCESS;
}

void update_min_max(int edge, int *edge_min, int *edge_max) {
	if (edge < *edge_min) {
		*edge_min = edge;
	}
	else if (edge > *edge_max) {
		*edge_max = edge;
	}
}

void set_option(option_t *option, int tile_idx, int idx, int edge_u, int edge_r, int edge_d, int edge_l) {
	option->tile_idx = tile_idx;
	option->idx = idx;
	option->edge_u = edge_u;
	option->edge_r = edge_r;
	option->edge_d = edge_d;
	option->edge_l = edge_l;
}

void chain_option(option_t *option, option_t *last, option_t *next) {
	option->last = last;
	last->next = option;
	option->next = next;
	next->last = option;
}

void choose_option(int cell_idx) {
	int col_idx, row_idx, edge_l_key, edge_u_key, edge_r_key, edge_d_key, header_key;
	option_t *option;
	if (cell_idx > cell_max) {
		cell_max = cell_idx;
		printf("%d\n", cell_max);
	}
	if (cell_idx == tiles_n) {
		solutions_n++;
		if (solutions_n == 1) {
			int j;
			for (j = 0; j < rows_n; j++) {
				int k;
				print_option(cells[j*cols_n]);
				for (k = 1; k < cols_n; k++) {
					putchar(' ');
					print_option(cells[j*cols_n+k]);
				}
				puts("");
			}
			fflush(stdout);
		}
		return;
	}
	col_idx = cell_idx%cols_n;
	row_idx = cell_idx/cols_n;
	if (col_idx == 0) {
		edge_l_key = 0;
	}
	else {
		edge_l_key = set_edge_lu_key(-cells[cell_idx-1]->edge_r);
	}
	if (row_idx == 0) {
		edge_u_key = 0;
	}
	else {
		edge_u_key = set_edge_lu_key(-cells[cell_idx-cols_n]->edge_d);
	}
	if (col_idx == cols_n-1) {
		edge_r_key = 0;
	}
	else {
		edge_r_key = 1;
	}
	if (row_idx == rows_n-1) {
		edge_d_key = 0;
	}
	else {
		edge_d_key = 1;
	}
	header_key = set_header_key(edge_l_key, edge_u_key, edge_r_key, edge_d_key);
	for (option = headers[header_key].next; option != headers+header_key; option = option->next) {
		if (used[option->tile_idx] < 2 && (!option->same || used[option->same->tile_idx] == 2)) {
			used[option->tile_idx] = 2;
			cells[cell_idx] = option;
			choose_option(cell_idx+1);
			used[option->tile_idx] = 1;
		}
	}
}

void print_option(option_t *option) {
	printf("%*d", width, option->tile_idx);
	if (rotate) {
		printf("-%d", option->idx);
	}
}

int set_edge_lu_key(int edge) {
	if (edge < 0) {
		return edges_n+edge;
	}
	return edge;
}

int set_edge_rd_key(int edge) {
	if (edge != 0) {
		return 1;
	}
	return edge;
}

int set_header_key(int edge_l_key, int edge_u_key, int edge_r_key, int edge_d_key) {
	return (edge_l_key*edges_n+edge_u_key)*4+edge_r_key*2+edge_d_key;
}
