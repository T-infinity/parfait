// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once

#include <stdexcept>
#include <sstream>
#define THROW(message) throw std::logic_error(std::string(message) + std::string(__FILE__) + std::string(__func__) + std::to_string(__LINE__));

inline void MyThrow(const std::string message,
        const char* file,
        const std::size_t line,
        const char* func) {
    std::ostringstream stream;
    stream << "EXCEPTION: " << message << ", file " << file << " func " << func << " line " << line;
    throw std::logic_error(stream.str());
}
