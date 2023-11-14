#include "ADTs/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void printQueue(const Queue *q, FILE *fp){
	if (q->isEmpty(q)) {
		fprintf(fp, "Empty\n");
	} else {
		const Iterator *it = q->itCreate(q);
		long v;
		int i = 0L;
		
		while(it->hasNext(it)) {
			it->next(it, (void **)&v);
			fprintf(fp, "%s%ld", (i == 0L) ? "" : " ", v);
			i++;
		}
		fprintf(fp, "\n");
		it->destroy(it);
	}
	
}

void exerciseQueue(FILE *fp, const Queue *q) {
	char buf[BUFSIZ];

	fgets(buf, BUFSIZ, fp);
	
	while (fgets(buf, BUFSIZ, fp) != NULL) {
		if (strcmp(buf, "dequeue\n") == 0) {
			long element;
			if (q->isEmpty(q)) {
				printf("QueueError\n");
			} else {
				q->dequeue(q, (void **)&element);
				printf("%ld\n", element);
			}
		} else if (strcmp(buf, "print\n") == 0) {
			printQueue(q, stdout);
		} else {
			long element;
			char *var = malloc(sizeof(char*));

			sscanf(buf+5, "%s %ld", var, &element);
			q->enqueue(q, (void *)element);
			free(var);
		}
	}
}

int main(int argc, char *argv[]) {
	int exitStatus = EXIT_FAILURE;

	if (argc != 2) {
		fprintf(stderr, "Unable to read more than one file\n");
		goto cleanup;
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open file\n");
		goto cleanup;
	}

	const Queue *q = Queue_create(doNothing);
	if (q == NULL) {
		fprintf(stderr, "Unable to create Queue ADT\n");
		fclose(fp);
		goto cleanup;
	}
	exerciseQueue(fp, q);
	exitStatus = EXIT_SUCCESS;
cleanup:
	if (q != NULL) {
		q->destroy(q);
	}
	if (fp != NULL) {
		fclose(fp);
	}
	return exitStatus;

}
