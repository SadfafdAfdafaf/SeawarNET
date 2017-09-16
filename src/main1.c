#include "kernel_gl.h"
#include "subsys.h"
#include "game.h"
#include "server.h"


int main(int argc, char *argv[]){
	
	camera_t cam;
	keystate_t keys;
	vertex *water, *model_1;
	v3 *cells;
	GLuint vbo[4], indexvbo, indexsky, index_obj_model_1, index_cells;
	GLuint texture1, texture2, texture_model;
    shader water_shader, sky_shader, model_1_shader, cell_shader;
	source_t water_source, sky_source, model_1_source, cell_source;
	shdr_attr water_attr[3], sky_attr[1], model_1_attr[3], cell_attr[1];
	unsigned int *indexwater, *index_model_1, *index_cellsmass;
	int is_active = 1;
	float sky[8][3] = 
	{
		{-50.0f, -50.0f, -50.0f},
		{-50.0f, -50.0f, 50.0f},
		{-50.0f, 50.0f, -50.0f},
		{-50.0f, 50.0f, 50.0f},
		{50.0f, -50.0f, -50.0f},
		{50.0f, -50.0f, 50.0f},
		{50.0f, 50.0f, -50.0f},
		{50.0f, 50.0f, 50.0f}
	};
	unsigned int insky[36] = 
	{0, 2, 3,  0, 3, 1,  0, 1, 5,  0, 5, 4,  2, 6, 3,  3, 6, 7,  4, 5, 6,  5, 7, 6,  1, 3, 5,  3, 7, 5,   0, 6, 2,  0, 4, 6};   

	char msg[3];
	int mypole[10][10] =
	{
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
	};
	int pole[10][10] =
	{
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
		{0,0,0, 0,0,0, 0,0,0,0},
	};
	
	new_tcpsock = NULL;
	phase = 0;
	choose = 0;
	input_dest(msg);
	put_ship(msg, mypole);
	int dxship1, dyship1;
	int dxship2, dyship2;
	dxship1 = translatex(msg, N);
	dyship1 = translatey(msg, N);
	
	input_dest(msg);
	put_ship(msg, mypole);
	dxship2 = translatex(msg, N);
	dyship2 = translatey(msg, N);
	
	if ( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL");
        return 1;
    }
	init_soket(&ip, &tcpsock, &set);
	
	SDL_Window *win = SDL_CreateWindow("Hello World!", 50, 50, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (win == NULL){
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);
	
	init_subgl();
	camera_init(&cam, vec3(10.0, 10.0, 10.0), vec3(0.0, 0.0, -1.0), 
              vec3(0.0, 1.0, 0.0), CAMERA_FOV, CAMERA_ASPECT,
              CAMERA_ZNEAR, CAMERA_ZFAR);
	apply_camera(&cam);
	
	keyboard_init(&keys);
	
	cells = malloc(40*sizeof(v3));
	if(!cells){
		printf("Error: mallock down\n");
		return -1;
	}
	inin_cells(cells);
	init_attrib_array(8);
	load_cells(&vbo[3], cells, 40, 7);
	index_cellsmass = malloc(44*sizeof(int));
	if(!index_cellsmass){
		printf("Error: mallock down\n");
		return -1;
	}
	init_index_cells(index_cellsmass);
	init_obj_index(&index_cells, index_cellsmass, 44);
	free(index_cellsmass);	
	free(cells);	
	
	int num_f = num_face("1.obj");
	unsigned int length_obdg = 3*(unsigned int)num_f;
	
	model_1 = malloc(length_obdg*sizeof(vertex));
	if(!model_1){
		printf("Error: mallock down\n");
		return -1;
	}
	
	index_model_1 = malloc(length_obdg*sizeof(int));
	if(!index_model_1){
		printf("Error: mallock down\n");
		free(model_1);
		return -1;
	}	
		
	num_f = load_obj("1.obj", model_1, index_model_1);
	if(num_f < 0){
		free(index_model_1);
		free(model_1);
		return -1;
	}
	
	init_obj_buffer(&vbo[2], model_1, length_obdg, 4, 5, 6);
	init_obj_index(&index_obj_model_1, index_model_1, length_obdg);

	
	water = malloc(N*N*sizeof(vertex));
	if(!water){
		printf("Error: mallock down\n");
		free(index_model_1);
		free(model_1);
		return -1;
	}
    
	indexwater = malloc(6*(N-1)*(N-1)*sizeof(int));
	if(!indexwater){
		printf("Error: mallock down\n");
		free(water);
		free(index_model_1);
		free(model_1);
		return -1;
	}
	
    init_water(water);
    
	init_index_w(indexwater);

    sky_source.vertex_source = "shdr/sky.vert";
	sky_source.fragmet_source = "shdr/sky.frag";
	model_1_source.vertex_source = "shdr/model1.vert";
	model_1_source.fragmet_source = "shdr/model1.frag";
	cell_source.vertex_source = "shdr/cell.vert";
	cell_source.fragmet_source = "shdr/cell.frag";
	water_attr[0].name = "in_Position"; water_attr[0].index = 0;
	water_attr[1].name = "in_Normal"; water_attr[1].index = 1;
	water_attr[2].name = "in_Texture"; water_attr[2].index = 2;
	sky_attr[0].name = "in_Pos"; sky_attr[0].index = 3;
	model_1_attr[0].name = "in_Position"; model_1_attr[0].index = 4;
	model_1_attr[1].name = "in_Normal"; model_1_attr[1].index = 5;
	model_1_attr[2].name = "in_Texture"; model_1_attr[2].index = 6;
	cell_attr[0].name = "in_Position"; cell_attr[0].index = 7;
       
	
    if(load_texture(&texture1, &texture2) <0){
        printf("Error: Bad load texture\n");
		free(water);
		free(index_model_1);
		free(indexwater);
		free(model_1);
        return -1;
    }
	
	if(load_2Dtexture(&texture_model, 2, "media/model.bmp") <0){
        printf("Error: Bad load texture\n");
		free(water);
		free(index_model_1);
		free(indexwater);
		free(model_1);
        return -1;
    }

    init_water_buffer(&vbo[0], water);
    init_sky_buffer(&vbo[1], sky);
	init_index_water(&indexvbo, indexwater);
    init_index_sky(&indexsky, insky);
	
	free(water);
	free(index_model_1);
	free(indexwater);
	free(model_1);

	init_shader(&model_1_shader, &model_1_source, model_1_attr, 3);
	glUseProgram(model_1_shader.shaderprogram);
	GLint llll = glGetUniformLocation(model_1_shader.shaderprogram, "cube2");
	if (llll == -1)
	{
	   printf("03 erroe uniform\n");
	}
	glUniform1i(llll, 1);	
	GLint lllll = glGetUniformLocation(model_1_shader.shaderprogram, "Texture");
	if (lllll == -1)
	{
	   printf("04 erroe uniform\n");
	}
	glUniform1i(lllll, 2);
	GLint xship = glGetUniformLocation(model_1_shader.shaderprogram, "dx");
	if (xship == -1)
	{
	   printf("004 erroe uniform\n");
	}
	glUniform1i(xship, dxship1);
	GLint yship = glGetUniformLocation(model_1_shader.shaderprogram, "dy");
	if (yship == -1)
	{
	   printf("0004 erroe uniform\n");
	}
	glUniform1i(yship, dyship1);
	
	water_source.vertex_source = "shdr/water.vert";
    water_source.fragmet_source = "shdr/water.frag";
	init_shader(&water_shader, &water_source, water_attr, 3);
    glUseProgram(water_shader.shaderprogram);
    GLint loc = glGetUniformLocation(water_shader.shaderprogram, "timer");
	if (loc == -1)
	{
	   printf("1 erroe uniform\n");
	}
	glUniform1f(loc, 0);
	GLint www = glGetUniformLocation(water_shader.shaderprogram, "myTexture");	
	if (www == -1)
	{
	   printf("2 erroe uniform\n");
	}
	glUniform1i(www, 0);
	
	GLint lll = glGetUniformLocation(water_shader.shaderprogram, "cube1");
	if (lll == -1)
	{
	   printf("3 erroe uniform\n");
	}
	glUniform1i(lll, 1);

	init_shader(&sky_shader, &sky_source, sky_attr, 1);
	glUseProgram(sky_shader.shaderprogram);	
	GLint ll2 = glGetUniformLocation(sky_shader.shaderprogram, "cube");
	if (ll2 == -1)
	{
	   printf("4 erroe uniform\n");
	}
	glUniform1i(ll2, 1);
	
	init_shader(&cell_shader, &cell_source, cell_attr, 1);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	SDL_GL_SetSwapInterval(1);	
    
	glClearColor(0.5, 0.5, 0.5, 1.0);
	SDL_Event ev;
	float utime=0.0;
	while(is_active){
		while (SDL_PollEvent(&ev))
			switch (ev.type) {
			  case SDL_QUIT:
				is_active = 0;
				break;
			  case SDL_KEYDOWN:
				keyboard_set_key(&keys, ev.key.keysym.sym);
				break;
			  case SDL_KEYUP:
				keyboard_clear_key(&keys, ev.key.keysym.sym);
			}
		if (keys)
			process_keys(keys, &cam);
				
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(water_shader.shaderprogram);
		glUniform1f(loc, (utime += 0.1));
		glDisable(GL_DEPTH_TEST);		
		glDepthMask(GL_FALSE);
		render(sky_shader.shaderprogram, vbo[1], indexsky, 36);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		render(water_shader.shaderprogram, vbo[0], indexvbo, 6*(N-1)*(N-1));		
		render(model_1_shader.shaderprogram, vbo[2], index_obj_model_1, length_obdg); 
		glUniform1i(xship, dxship2);
		glUniform1i(yship, dyship2);
		render(model_1_shader.shaderprogram, vbo[2], index_obj_model_1, length_obdg); 
		glUniform1i(xship, dxship1);
		glUniform1i(yship, dyship1);
		render2(cell_shader.shaderprogram, vbo[3], index_cells, 44);
		
		SDL_GL_SwapWindow(win);
		
		if((game_loop(msg, mypole, pole)) < 0){
			is_active = 0;
		}
	}

    clear_shader(&water_shader);
	clear_shader(&sky_shader);
	clear_shader(&model_1_shader);
	clear_shader(&cell_shader);
    
    clear_attri_array(8);
    clear_buffer(&vbo[0]);
    clear_buffer(&vbo[1]);
    clear_buffer(&vbo[2]);
    clear_buffer(&vbo[3]);
    clear_buffer(&indexvbo);
    clear_buffer(&indexsky);
    delete_texture(&texture1, &texture2);

	exit_socket(&new_tcpsock);
	exit_socket(&tcpsock);
	net_quit();
	SDL_GL_DeleteContext(glcontext);	
	SDL_DestroyWindow(win);
	SDL_Quit();	
	return 0;
}