
Serialization
===================

**Serialization** is the process of translating data structures or object state into a format that can be transmitted across the network and reconstructed later in the same or another computer environment.

----------


MessagePack
-------------

**MessagePack** is an efficient binary serialization format. It lets you exchange data among multiple languages like JSON. But it's faster and smaller. Small integers are encoded into a single byte, and typical short strings require only one extra byte in addition to the strings themselves.

####Serializing Objects in C++

In order to implement MessagePack serialization in C++ the msgpack-c [[1]] library was used. An object can be serialized by using the *MSGPACK_DEFINE* macro. All standard types are supported.

```c++
#include <vector>
#include <string>
#include <msgpack.hpp>

struct Entity {
	std::string id;
	std::string type;
	std::vector<std::string> services;
		
	MSGPACK_DEFINE(id, type, services);
};

int main(int argc, char* argv[]) {
	Entity entity {
	  "1", "camera", { "fps", "delay" }
	};
	std::stringstream body;
  msgpack::pack(body, entity);
}
```
The object above will be equivalent to the following JSON object.

```javascript
["1", "camera", ["fps", "delay"]] 
// Binary Form: 93 a1 31 a6 63 61 6d 65 72 61 92 a3 66 70 73 a5 64 65 6c 61 79 
```

[1]: https://github.com/msgpack/msgpack-c/
