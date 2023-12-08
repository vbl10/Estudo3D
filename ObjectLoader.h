#pragma once
#include <vector>
#include <fstream>
#include <string>

std::vector<float> ObjLoadFloats(const std::string& path, const std::string& id);
std::vector<int> ObjLoadInts(const std::string& path, const std::string& id);