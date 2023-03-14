#include "PythonScript.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>

static void validatePythonStatus(const PyStatus& status)
{
    if(PyStatus_Exception(status))
        Py_ExitStatusException(status);
}

static wchar_t* str2wstr(PyConfig* config, const std::string& str)
{
    wchar_t* wstr = nullptr;

    validatePythonStatus(PyConfig_SetBytesString(config, &wstr, str.c_str()));

    return wstr;
}

#define SET_PYTHON_CONFIG_STRING(CONFIG_FIELD, STRING)\
validatePythonStatus(PyConfig_SetBytesString(&config, &config.CONFIG_FIELD, STRING))

PythonScript::PythonScript(const std::string& scriptFilePath)
{
    std::filesystem::path path("scripts/" + scriptFilePath);

    scriptFileName = path.filename().string();

    errno_t error = fopen_s(&scriptFile, path.string().c_str(), "rb");

    if(error)
        throw std::runtime_error("Unable to open python script at \"" + path.string() + "\": " + std::to_string(error));

    PyConfig_InitIsolatedConfig(&config);

    static const std::string pythonDirectoryPath = "python";

#define STR_HELPER(X) #X
#define STR(X) STR_HELPER(X)

    static const std::string pythonPath = pythonDirectoryPath + "/python" STR(PY_MAJOR_VERSION) STR(PY_MINOR_VERSION);
    
    SET_PYTHON_CONFIG_STRING(exec_prefix, pythonDirectoryPath.c_str());

    SET_PYTHON_CONFIG_STRING(prefix, (pythonPath + "/site-packages").c_str());

    config.module_search_paths_set = true;

    addModuleSearchPath(pythonPath);
    addModuleSearchPath(scriptFileParentPath = path.parent_path().string());
}

PythonScript::~PythonScript()
{
    fclose(scriptFile);

    PyConfig_Clear(&config);
}

std::string PythonScript::getScriptFileName() const
{
    return scriptFileName;
}

void PythonScript::runScript(std::vector<std::string> args) const
{
    size_t argc = args.size() + 1;
    
    std::vector<char*> argv(argc);
    
    std::string scriptFileNameCopy = scriptFileName;

    argv[0] = scriptFileNameCopy.data();
    
    for(size_t i = 0; i < args.size(); i++)
        argv[i + 1] = args[i].data();

    validatePythonStatus(PyConfig_SetBytesArgv(&config, argv.size(), argv.data()));

    validatePythonStatus(Py_InitializeFromConfig(&config));
    
    static const std::string WALL(20, '*');
    
#define PRINT_PYTHON_OUTPUT_BOUNDARY(TEXT)\
    std::cout << WALL + " PYTHON OUTPUT "#TEXT" " + WALL << std::endl
    
    std::cout << std::endl;

    PRINT_PYTHON_OUTPUT_BOUNDARY(START);

    int returnValue = PyRun_SimpleFile(scriptFile, scriptFileName.c_str());
    
    PRINT_PYTHON_OUTPUT_BOUNDARY(END);

    std::cout << std::endl;

    if(returnValue)
        throw std::runtime_error("Error while running python script at \"" + scriptFilePath + "\" (returned " + std::to_string(returnValue) + ')');
}

void PythonScript::addModuleSearchPath(const std::string& path, bool relativePath)
{
    PyWideStringList_Append(&config.module_search_paths, str2wstr(&config, relativePath ? scriptFileParentPath + '/' + path : path));
}