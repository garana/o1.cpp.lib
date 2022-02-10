/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Gonzalo Arana
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef O1CPPLIB_O1_PROPERTY_HH
#define O1CPPLIB_O1_PROPERTY_HH

#include <type_traits>

#define O1_SCALAR_PROPERTY_DEFINITION(type, name, initial_value)               \
    static_assert(                                                             \
	    std::is_scalar<type>::value,                                           \
		"O1_SCALAR_PROPERTY_DEFINITION: " #type "must be a scalar"             \
    );                                                                         \
    private: type _##name{initial_value};

#define O1_SCALAR_PROPERTY_GETTER(type, name)                                  \
    public: type name() const { return _##name; }

#define O1_SCALAR_PROPERTY_SETTER(type, name)                                  \
     public: void name(type new_value) { _##name = new_value; }


#define O1_READONLY_SCALAR_PROPERTY(type, name, initial_value)                 \
     O1_SCALAR_PROPERTY_DEFINITION(type, name, initial_value)                  \
     O1_SCALAR_PROPERTY_GETTER(type, name)

#define O1_READWRITE_SCALAR_PROPERTY(type, name, initial_value)                \
     O1_SCALAR_PROPERTY_DEFINITION(type, name, initial_value)                  \
     O1_SCALAR_PROPERTY_GETTER(type, name)                                     \
     O1_SCALAR_PROPERTY_SETTER(type, name)


#define O1_REFERENCE_PROPERTY_DEFINITION(type, name, initial_value)            \
    private: type _##name{initial_value};

#define O1_REFERENCE_PROPERTY_GETTER(type, name, readOnly)                     \
    public: type& name()       { return _##name; }                             \
    public: type& name() const { return _##name; }


#define O1_REFERENCE_PROPERTY_SETTER(type, name)                               \
     public: void name(type new_value) { _##name = new_value; }

#define O1_READONLY_REFERENCE_PROPERTY(type, name, initial_value)              \
     O1_REFERENCE_PROPERTY_DEFINITION(type, name, initial_value)               \
     O1_REFERENCE_PROPERTY_GETTER(const type, name, 1)                         \

#define O1_READWRITE_REFERENCE_PROPERTY(type, name, initial_value)             \
     O1_REFERENCE_PROPERTY_DEFINITION(type, name, initial_value)               \
     O1_REFERENCE_PROPERTY_GETTER(type, name, 0)                               \
     O1_REFERENCE_PROPERTY_SETTER(type, name)


#define O1_OWNING_PROPERTY_DEFINITION(type, name)                              \
    private: type _##name;

#define O1_OWNING_PROPERTY_GETTER(type, name)                                  \
    public: type& name()       { return _##name; }                             \
    public: type& name() const { return _##name; }

#define O1_OWNING_PROPERTY_SETTER(type, name)                                  \
     public: void name(const type& new_value) { _##name = new_value; }

#define O1_READONLY_OWNING_PROPERTY(type, name)                                \
     O1_OWNING_PROPERTY_DEFINITION(type, name)                                 \
     O1_OWNING_PROPERTY_GETTER(const type, name)

#define O1_READWRITE_OWNING_PROPERTY(type, name, initial_value)                \
     O1_OWNING_PROPERTY_DEFINITION(type, name, initial_value)                  \
     O1_OWNING_PROPERTY_GETTER(type, name)                                     \
     O1_OWNING_PROPERTY_SETTER(type, name)

#endif //O1CPPLIB_O1_PROPERTY_HH
