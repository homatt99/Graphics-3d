#include <cstdlib>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <SDL2/SDL.h> //Using SDL2 for base window

GLuint program;
GLint attribute_coord2d;


bool init_resources(void){
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    

    //vertex shader
    const char* vs_source = "#version 120\n"
        "attribute vec2 coord2d;                  "
        "void main(void){                         "
        "   gl_Position = vec4(coord2d, 0.0,1.0); "
        "}";
    
    glShaderSource(vs, 1, &vs_source, NULL);   
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if(!compile_ok) {
        cerr << "Error in vertex shader" << endl;
        return false;
    }


    //Fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* fs_source = "#version 120\n"
        "void main(void){       "
        "   gl_FragColor[0] = gl_FragCoord.x/640.0; "
        "   gl_FragColor[1] = gl_FragCoord.y/480.0; "
        "   gl_FragColor[2] = 0.5; "
        "}";

    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if(!compile_ok){
        cerr <<"Error in fragment shader" << endl;
        return false;
    }

    //GLSL Program
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS,&link_ok);
    if(!link_ok){
        cerr << "Error in glLinkProgram" << endl;
        return false;
    }

    //Passing the triangle vertices to the vertex shader
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if(attribute_coord2d == -1){
        cerr << "Could not bind attribute " << attribute_name << endl;
        return false;
    }
    return true;
}

void render(SDL_Window* window) {
    //clear the background as white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);

    GLfloat triangle_vertices[] = {
        0.0, 0.8, 
        -0.8, -0.8, 
        0.8, -0.8,
    }; //coordinates of openGL by default range from [-1,+1]

    glVertexAttribPointer(
            attribute_coord2d,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            triangle_vertices
                              );
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(attribute_coord2d);

    SDL_GL_SwapWindow(window);

}

//reads shader from file, as opposed to from predefined c-string
char* file_read(const char* filename){
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if(rw == NULL) return NULL;

    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*)malloc(res_size+1);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;

    while(nb_read_total < res_size && nb_read != 0){
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }

    SDL_RWclose(rw);
    if(nb_read_total != res_size){
        free(res);
        return NULL;
    }

    res[nb_read_total] = '\0';
    return res;
}

void mainLoop(SDL_Window* window){
    while(true){
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            if (ev.type == SDL_QUIT) return;
        }
        render(window);
    }
}

void free_resources(){
    glDeleteProgram(program);
}

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Matthew's Triangle", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);

    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK){
        cerr << "Error: glewINIT: " << glewGetErrorString(glew_status) << endl;
        return EXIT_FAILURE;
    }

    if(!init_resources()){
        return EXIT_FAILURE;
    }

    mainLoop(window);

    
    free_resources();
    return 0;    
}
