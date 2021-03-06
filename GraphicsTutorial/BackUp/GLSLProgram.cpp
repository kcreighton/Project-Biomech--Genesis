#include "GLSLProgram.h"
#include "Errors.h"

#include <vector>

#include <fstream>

//The : _attributesNumber(0) ect. is an initialization list. It is a better way to initialize variables, since it avoids an extra copy. 
GLSLProgram::GLSLProgram() : _attributesNumber(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{

}

GLSLProgram::~GLSLProgram()
{

}

//Compiles the shaders into a form that your GPU can understand
void GLSLProgram::compileShaders(const std::string& VERTEX_SHADER_FILE_PATH, const std::string& FRAGMENT_SHADER_FILE_PATH)
{
    //Create program object
	_programID = glCreateProgram();
	
	//Create the vertex shader object, and store its ID
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		fatalError("vertex shader failed to be created!");
	}

    //Create the fragment shader object, and store its ID
    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		fatalError("fragment shader failed to be created!"); 
	}

    //Compile each shader
    compileShader(VERTEX_SHADER_FILE_PATH, _vertexShaderID);
    compileShader(FRAGMENT_SHADER_FILE_PATH, _fragmentShaderID);
}

//Adds an attribute to our shader. SHould be called between compiling and linking.
void GLSLProgram::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(_programID, _attributesNumber++, attributeName.c_str());
}

//links shaders together into a program.
void GLSLProgram::linkShaders()
{
    //Vertex and fragment shaders are successfully compiled.
    //Now time to link them together into a program.

    //Attach our shaders to our program
    glAttachShader(_programID, _vertexShaderID);
    glAttachShader(_programID, _fragmentShaderID);

    //Link our program
    glLinkProgram(_programID);

    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);

	//Error check
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);


		//Don't leak programs or shaders
        glDeleteProgram(_programID);
        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);

        //print the error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("shaders failed to link!");
    }

    //Always detach shaders after a successful link.
    glDetachShader(_programID, _vertexShaderID);
    glDetachShader(_programID, _fragmentShaderID);
    glDeleteShader(_vertexShaderID);
    glDeleteShader(_fragmentShaderID);
}

GLuint GLSLProgram::getUniformLocation(const std::string uniformName)
{
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		fatalError("Uniform" + uniformName + "not found in shader");
	}
	return location;
}

//enable the shader program, and all its attributes
void GLSLProgram::use()
{
    glUseProgram(_programID);
    //enable all the attributes we added with addAttribute
	for (int i = 0; i < _attributesNumber; i++)
	{
        glEnableVertexAttribArray(i);
    }
}

//disable the shader program
void GLSLProgram::unuse()
{
    glUseProgram(0);
	for (int i = 0; i < _attributesNumber; i++)
	{
        glDisableVertexAttribArray(i);
    }
}

//Compiles a single shader file
void GLSLProgram::compileShader(const std::string& FILE_PATH, GLuint shaderID)
{

    //Open the file
    std::ifstream shaderFile(FILE_PATH);
    if (shaderFile.fail())
	{
        perror(FILE_PATH.c_str());
        fatalError("failed to open " + FILE_PATH);
    }

    //File contents stores all the text in the file
    std::string fileContents = "";

    //line is used to grab each line of the file
    std::string line;

    //Get all the lines in the file and add it to the contents
    while (std::getline(shaderFile, line))
	{
        fileContents += line + "\n";
    }

	//Close the file
    shaderFile.close();

    //get a pointer to our file contents c string;
    const char* contentsPtr = fileContents.c_str();

    //tell opengl that we want to use fileContents as the contents of the shader file
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);

    //compile the shader
	glCompileShader(shaderID);

    //check for errors
    GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        //The maxLength includes the NULL character
        std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        //Provide the infolog in whatever manor you deem best.
        //Exit with failure.
		glDeleteShader(shaderID); //Don't leak the shader.

        //Print error log and quit
        std::printf("%s\n", &(errorLog[0]));
        fatalError("shader " + FILE_PATH + " failed to compile");
    }
}