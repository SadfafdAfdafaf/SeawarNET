#ifndef __KERNEL_GL_H__
#define __KERNEL_GL_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "subsys.h"
#include "fileext.h"

typedef struct vertex{
    float vx, vy;
	float nx, ny, nz;
	float x, y, z;
}vertex;
typedef struct v3{
    float x, y, z;
}v3;
typedef struct v2{
    float x, y;
}v2;
typedef struct shader{
    GLuint vertexshader, fragmentshader, shaderprogram;
}shader;

typedef struct source_t{
	char* vertex_source;
    char* fragmet_source;  
}source_t;

typedef struct shdr_attr{
	char* name;
	GLuint index; 
}shdr_attr;

void render(GLuint shader, GLuint vbo, GLuint ibo, GLuint length);
void render2(GLuint shader, GLuint vbo, GLuint ibo, GLuint length);

int init_shader(shader *shdr, source_t *shdr_source, shdr_attr *attr, int length);
void clear_shader(shader* shdr);

void init_attrib_array(int n);
void clear_attri_array(int n);

void init_water_buffer(GLuint *buffer, vertex *water);
void init_sky_buffer(GLuint *buffer, float sky[][3]);
void init_index_water(GLuint *index_buffer, unsigned int* index_data);
void init_index_sky(GLuint *index_buffer, unsigned int *index_data);
void clear_buffer(GLuint* index_buffer);

void delete_texture(GLuint *texture, GLuint *cubemap);
int load_texture(GLuint *texture, GLuint *cubemap);
void setupwatertexture(GLuint *texture, SDL_Surface *text);
void initwatertexture(GLuint* texture);
void initCubeMap(GLuint* texture);
void setupCubeMap(GLuint *texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg);

int init_water(vertex* water);
int init_index_w(unsigned int *indexwater);

int num_face(char* file_str);
int load_obj(char* file_name, vertex* model, unsigned int* index_model);
void init_obj_buffer(GLuint *buffer, vertex *data, unsigned int length, GLuint atrp1, GLuint atrp2,GLuint atrp3);
void init_obj_index(GLuint *index_buffer, unsigned int* index_data, unsigned int length);

void init2Dtexture(GLuint* texture, unsigned int text_modul);
void setup2Dtexture(GLuint *texture, SDL_Surface *text, unsigned int text_modul);
int load_2Dtexture(GLuint *texture, unsigned int text_modul, char* path);

int inin_cells(v3* cells);
int init_index_cells(unsigned int *index);
void load_cells(GLuint *buffer, v3 *data, unsigned int length, GLuint atrp1);
void draw_cells(GLuint vbo);
#endif