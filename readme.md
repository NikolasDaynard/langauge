A lang written as an example for nixos

Run nix-shell to enter the dev enviroment then run

```
mkdir build
cd build
cmake ..
make
```

To compile, then run "clang output.ll -o output" to compile to an exe

lang examples. It's similar to lua sytatically, with optional semicolon terminators. It has the sytatic sugar of feeling like lua but allows for extreme low level programming. It is 0 indexed. Designed with string manipulation as a core feature

This language drops the then end syntax in favor of easier to write c style braces

```
std = include("std")

print("hellow world!")
```

```
foo(bar, name) {
    print(name .. "says" .. bar)
}

foo(3, "Joe")
```

variable types are optional but allowed, with memory denoted by {num}

```
int foo = 20
int{8} bar = 100
string type = "string"

int foo(){
    return 0
}
```

Attempting to reassign a typed variable results in an error, leaving expressive features, but also a way against them.

Variable are private by default

```
foo() {
    var = 2
}
fooPublic() {
    public var = 2
}

foo()
print(var)
fooPublic()
print(var)
```
output: nil, 2

loop keywords:
```
for(i = 0, i < 5, i++) {
    print(i)
}

cond = true
while(cond) {
    print(cond)
    cond = false
}

goto foo

foo: -- labels are functions so they can be optionally be called like foo()
print("went")
```

for loops can be optionally declared as, defauting to ++ and = 0

```
for(i, 5) {
    print(i) -- prints 0, 1, 2, 3, 4, 5
}
```

Much like lua, it has easy language integration, you can call c / cpp by using

```
foolib = extern("foo.c") -- or .cpp or .cc

print(foolib::some_c_function(3, 4))
```

Libraries can be included with
```
std = include("std")

std::test()
```
This is the recommeneded way, however libraries can be imported globally without a keyword with

```
include("std")
test()
```