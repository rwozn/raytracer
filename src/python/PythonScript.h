#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <Python.h>

class PythonScript
{
protected:
	FILE* scriptFile;

	mutable PyConfig config;

	std::string scriptFileName;
	std::string scriptFilePath;
	std::string scriptFileParentPath;

public:
	PythonScript(const std::string& scriptFilePath);
	~PythonScript();
	
	std::string getScriptFileName() const;

	void runScript(std::vector<std::string> args) const;

	void addModuleSearchPath(const std::string& path, bool relativePath = false);
};
