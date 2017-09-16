#include "kernel_gl.h"

void render(GLuint shader, GLuint vbo, GLuint ibo, GLuint length){
	glUseProgram(shader);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);		
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
	glDrawElements(GL_TRIANGLES,  length, GL_UNSIGNED_INT, NULL);
}

int init_shader(shader *shdr, source_t *shdr_source, shdr_attr *attr, int length){
    int IsCompiled_VS, IsCompiled_FS;
    int IsLinked;
    int maxLength;
    char *vertexInfoLog;
    char *fragmentInfoLog;
    char *shaderProgramInfoLog;
	GLchar *vertexsource, *fragmentsource;

    vertexsource = filetobuf(shdr_source->vertex_source);
    fragmentsource = filetobuf(shdr_source->fragmet_source);

    shdr->vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shdr->vertexshader, 1, (const GLchar**)&(vertexsource), 0);
    glCompileShader(shdr->vertexshader);
    glGetShaderiv(shdr->vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
    if(IsCompiled_VS == FALSE)
    {
       glGetShaderiv(shdr->vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
       vertexInfoLog = (char *)malloc(maxLength);
       glGetShaderInfoLog(shdr->vertexshader, maxLength, &maxLength, vertexInfoLog);	
	   printf("1 %s\n", vertexInfoLog);	   
       free(vertexInfoLog);	   
       return -1;
    }
    
    shdr->fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shdr->fragmentshader, 1, (const GLchar**)&(fragmentsource), 0);
    glCompileShader(shdr->fragmentshader);
    glGetShaderiv(shdr->fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == FALSE)
    {
       glGetShaderiv(shdr->fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
       fragmentInfoLog = (char *)malloc(maxLength);
       glGetShaderInfoLog(shdr->fragmentshader, maxLength, &maxLength, fragmentInfoLog);
	   printf("2 %s\n", fragmentInfoLog);	  
       free(fragmentInfoLog);
       return -1;
    }

    shdr->shaderprogram = glCreateProgram();
    glAttachShader(shdr->shaderprogram, shdr->vertexshader);
    glAttachShader(shdr->shaderprogram, shdr->fragmentshader);
	for(int i = 0; i < length; ++i){
		glBindAttribLocation(shdr->shaderprogram, attr[i].index, attr[i].name);
		//printf("%s %d\n", attr[i].name, attr[i].index);
	}
    glLinkProgram(shdr->shaderprogram);
    glGetProgramiv(shdr->shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
    if(IsLinked == FALSE)
    {
       glGetProgramiv(shdr->shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
       shaderProgramInfoLog = (char *)malloc(maxLength);
       glGetProgramInfoLog(shdr->shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);
       free(shaderProgramInfoLog);
	   printf("3\n");
       return -1;
    }
	free(vertexsource);
    free(fragmentsource);
    return 1;
}

void clear_shader(shader* shdr){
    glDetachShader(shdr->shaderprogram, shdr->vertexshader);
    glDetachShader(shdr->shaderprogram, shdr->fragmentshader);
    glDeleteProgram(shdr->shaderprogram);
    glDeleteShader(shdr->vertexshader);
    glDeleteShader(shdr->fragmentshader);
}
void init_attrib_array(int n){
    for(int i=0; i<n; ++i)
        glEnableVertexAttribArray(i);
}
void clear_attri_array(int n){
    for(int i=0; i<n; ++i)
        glDisableVertexAttribArray(i);
}
void init_water_buffer(GLuint *buffer, vertex *water){
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);    
    glBufferData(GL_ARRAY_BUFFER, N*N*sizeof(vertex), water, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(2*sizeof(float)));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(5*sizeof(float)));
}
void init_sky_buffer(GLuint *buffer, float sky[][3]){
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);    
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), sky, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
}
void init_index_water(GLuint *index_buffer, unsigned int* index_data){
    glGenBuffers(1, index_buffer);    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(6*(N-1)*(N-1)), index_data, GL_STATIC_DRAW);

}
void init_index_sky(GLuint *index_buffer, unsigned int *index_data){
    glGenBuffers(1, index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(36), index_data, GL_STATIC_DRAW);
}
void clear_buffer(GLuint* index_buffer){
    glDeleteBuffers(1, index_buffer);
}
void delete_texture(GLuint *texture, GLuint *cubemap){    
	glDeleteTextures(1, texture);
	glDeleteTextures(1, cubemap);
}
void initCubeMap(GLuint* texture) {
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void setupCubeMap(GLuint *texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg) {
	initCubeMap(texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0, xpos->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, xpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0, xneg->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, xneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0, ypos->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, ypos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0, yneg->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, yneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0, zpos->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, zpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0, zneg->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, zneg->pixels);
}
void initwatertexture(GLuint* texture) {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
}
void setupwatertexture(GLuint *texture, SDL_Surface *text) {
	initwatertexture(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->w,text->h, 0, GL_BGR, GL_UNSIGNED_BYTE, text->pixels);
}
int load_texture(GLuint *texture, GLuint *cubemap){
    SDL_Surface* surf = SDL_LoadBMP("media/2.bmp");
    if (surf == NULL)
    {
        printf("Error: \"%s\"\n",SDL_GetError());
        return -1;
    }
	SDL_Surface *xpos = SDL_LoadBMP("media/xpos.bmp");	SDL_Surface *xneg = SDL_LoadBMP("media/xneg.bmp");
	SDL_Surface *ypos = SDL_LoadBMP("media/ypos.bmp");	SDL_Surface *yneg = SDL_LoadBMP("media/yneg.bmp");
	SDL_Surface *zpos = SDL_LoadBMP("media/zpos.bmp");	SDL_Surface *zneg = SDL_LoadBMP("media/zneg.bmp");
    if ((xpos == NULL)||(ypos == NULL)||(zpos == NULL)||(xneg == NULL)||(yneg == NULL)||(zneg == NULL))
    {
        printf("Error: \"%s\"\n",SDL_GetError());
        return -1;
    }
    
    setupwatertexture(texture, surf);	
	setupCubeMap(cubemap, xpos, xneg, ypos, yneg, zpos, zneg);
    
    SDL_FreeSurface(xneg);	SDL_FreeSurface(xpos);
	SDL_FreeSurface(yneg);	SDL_FreeSurface(ypos);
	SDL_FreeSurface(zneg);	SDL_FreeSurface(zpos);
    SDL_FreeSurface(surf);
    return 1;
}

int init_water(vertex* water){
	for(int i = 0; i<N; i++)
        for(int j = 0; j<N; j++)
        {
			water[i*N + j].vx = 4.0*(float)i/(N-1);
			water[i*N + j].vy = 4.0*(float)j/(N-1);
			water[i*N + j].nx = 0;
			water[i*N + j].ny = 1;
            water[i*N + j].nz = 0;
			water[i*N + j].x = i;
			water[i*N + j].y = 0;
            water[i*N + j].z = j;			
        }
	return 1;
}

int init_index_w(unsigned int *indexwater){
	unsigned int pa = 1;
	unsigned int pb = N;
	int k = 0;
	for (int i = 0; i < N-1; ++i) {
	  for (int j = 0; j < N-1; ++j) {
		indexwater[k] = pa-1;
		k++;
		indexwater[k] = pa;
		k++;
		indexwater[k] = pb;
		k++;
		indexwater[k] = pa;
		k++;
		indexwater[k] = pb+1;
		k++;
		indexwater[k] = pb;
		k++;
		pa++;
		pb++;
	  }    
	  pa++;
	  pb++;
	}
	return 1;
}

int num_face(char* file_str){
	int lindex=0;
	FILE * file = fopen(file_str, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return -1;
	}
	
	char lineHeader[60];
	while( fgets(lineHeader, 60, file) != NULL ){
		if((lineHeader[0] == 'f')&&(lineHeader[1] == ' '))
			lindex++;
	}
	
	fclose(file);
	return lindex;	
}

int load_obj(char* file_name, vertex* model, unsigned int* index_model){
	int lvert=0, ltext=0, lnorm=0, lindex=0;

	FILE * file = fopen("1.obj", "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return -1;
	}
	
	char lineHeader[60];
	while( fgets(lineHeader, 60, file) != NULL ){
	if(lineHeader[0] == 'v'){
		if(lineHeader[1] == ' ')
			lvert++;
		if(lineHeader[1] == 't')
			ltext++;
		if(lineHeader[1] == 'n')
			lnorm++;
	}else
		if((lineHeader[0] == 'f')&&(lineHeader[1] == ' '))
			lindex++;
	}
	rewind(file);
	
	v3 *coord;
    v3 *norm;
    v2 *text;
	coord = malloc(lvert*sizeof(v3));
	if(!coord){
		printf("Error: mallock down\n");
		return -1;
	}
    norm = malloc(lnorm*sizeof(v3));
	if(!norm){
		printf("Error: mallock down\n");
        free(coord);
		return -1;
	}
    text = malloc(ltext*sizeof(v2));
	if(!text){
		printf("Error: mallock down\n");
        free(coord);
        free(norm);
		return -1;
	}
	int i=0, j=0, k=0, l=0, NN=3*lindex;
	int vertexIndex[3], uvIndex[3], normalIndex[3];
	
	while( fgets(lineHeader, 3, file) != NULL  ){
	if(lineHeader[0] == 'v'){
		if(lineHeader[1] == ' '){
			fscanf(file, "%f %f %f\n", &coord[i].x, &coord[i].y, &coord[i].z );
			i++;
		}
		if(lineHeader[1] == 't'){
			fscanf(file, "%f %f\n", &text[j].x, &text[j].y);
			j++;
		}
		if(lineHeader[1] == 'n'){
			fscanf(file, "%f %f %f\n", &norm[k].x, &norm[k].y, &norm[k].z );
			k++;
		}
	}else
		if((lineHeader[0] == 'f')&&(lineHeader[1] == ' ')){
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			
			for(int i = 0; i<3 ; ++i){
				vertexIndex[i]-=1;
				normalIndex[i]-=1;
				uvIndex[i]-=1;
				
				model[l].x = coord[vertexIndex[i]].x;
				model[l].y = coord[vertexIndex[i]].y;
				model[l].z = coord[vertexIndex[i]].z;
				model[l].nx = norm[normalIndex[i]].x;
				model[l].ny = norm[normalIndex[i]].y;
				model[l].nz = norm[normalIndex[i]].z;
				model[l].vx = text[uvIndex[i]].x;
				model[l].vy = text[uvIndex[i]].y;
				l++;
			};			
		}
	}
	for(int i = 0; i<NN ; i+=3){
		index_model[i] = i;
		index_model[i+1] = i+1;
		index_model[i+2] = i+2;
	}
	
	
	free(coord);
    free(norm);
    free(text);
	fclose(file);
	return 0;
}

void init_obj_buffer(GLuint *buffer, vertex *data, unsigned int length, GLuint atrp1, GLuint atrp2,GLuint atrp3){
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, length*sizeof(vertex), data, GL_STATIC_DRAW);
    glVertexAttribPointer(atrp3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(atrp2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(2*sizeof(float)));
	glVertexAttribPointer(atrp1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(5*sizeof(float)));
}
void init_obj_index(GLuint *index_buffer, unsigned int* index_data, unsigned int length){
    glGenBuffers(1, index_buffer);    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(length), index_data, GL_STATIC_DRAW);

}

void init2Dtexture(GLuint* texture, unsigned int text_modul) {
	glActiveTexture(GL_TEXTURE0 + text_modul);
	glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
}
void setup2Dtexture(GLuint *texture, SDL_Surface *text, unsigned int text_modul) {
	init2Dtexture(texture, text_modul);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->w,text->h, 0, GL_BGR, GL_UNSIGNED_BYTE, text->pixels);
}
int load_2Dtexture(GLuint *texture, unsigned int text_modul, char* path){

	SDL_Surface* surf = SDL_LoadBMP(path);
    if (surf == NULL)
    {
        printf("Error: \"%s\"\n",SDL_GetError());
        return -1;
    }	

    setup2Dtexture(texture, surf, text_modul);	
    
    SDL_FreeSurface(surf);
    return 1;
}

int inin_cells(v3* cells){
	
	int k = 0;
	for(int i = 0; i<11; i++){
		cells[k].x = i*(N/10);
		cells[k].y = 10;
		cells[k].z = 0*(N/10);		
		k++;
	}
	
	for(int i = 1; i<10; i++){
		cells[k].x = 0*(N/10);
		cells[k].y = 10;
		cells[k].z = i*(N/10);		
		k++;
		
		cells[k].x = 10*(N/10);
		cells[k].y = 10;
		cells[k].z = i*(N/10);	
		k++;
	}

	for(int i = 0; i<11; i++){
		cells[k].x = i*(N/10);
		cells[k].y = 10;
		cells[k].z = 10*(N/10);		
		k++;
	}
	return 1;
}
int init_index_cells(unsigned int *index){
	int k = 0;
	for(int i = 0; i<11; i++){
		index[k] = i;
		index[k+1] = i+29;
		k+=2;
	}
	index[k] = 0;
	k++;
	index[k] = 10;
	k++;
	for(int i = 11; i<29; i++){
		index[k] = i;		
		k++;
	}
	index[k] = 29;
	k++;
	index[k] = 39;
	
	return 1;
}

void load_cells(GLuint *buffer, v3 *data, unsigned int length, GLuint atrp1){
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, length*sizeof(v3), data, GL_STATIC_DRAW);
	glVertexAttribPointer(atrp1, 3, GL_FLOAT, GL_FALSE, sizeof(v3), 0);
}

void render2(GLuint shader, GLuint vbo, GLuint ibo, GLuint length){
	glLineWidth(5); 
	glUseProgram(shader);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);		
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	
	glDrawElements(GL_LINES,  length, GL_UNSIGNED_INT, NULL);
}