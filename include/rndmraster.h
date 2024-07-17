#ifndef RNDMRASTER_H
#define RNDMRASTER_H

#include <stdint.h>
#include <stdio.h>

#define CONTEXT_FIELDS	\
	int imagewidth, imageheight;	\
	uint32_t * buffer;		\
	float * zbuf;			\
	void (*draw)(struct context *);	\

struct context {
	CONTEXT_FIELDS
};

struct ppm_context {
	CONTEXT_FIELDS
	FILE * file;
};

struct vertexshader {
	float near;
};

enum colorization {
	INTERPOLATE,
	FILL
};

struct fragmentshader {
	enum colorization colorization;
	union {
		float colors[9];
		float color[3];
	} data;
};

struct ppm_context init_ppm(int width, int height, char * filename);

// Draws count triangles as specified in vertices and colors to the buffer of the given context
void drawtriangles(struct context * context, float * vertices, int count,
		struct vertexshader * vertex, struct fragmentshader * fragment);

// Renders the content of the buffer to the receiving system (e.g. an image file)
void display(struct context * context);

#endif
