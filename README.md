# MeshSliceTricks

![](https://raw.githubusercontent.com/xiconxi/MeshSliceTricks/master/sliceVisual.gif)

External Libs: 

```
1. glm
2. glfw 
3. gl3w
4. imgui
```

## Usages:
Clone repos:

```git clone submodule --recurse https://github.com/xiconxi/MeshSliceTricks```

[Config Gl3w Lib](https://github.com/skaslev/gl3w):

```
cd External/gl3w
python gl3w_gen.py
```

cmake&build
```
mkdir build & cd build
cmake ../
make -j4
#cmake & make again !!(given to using gl3w incorrectly)
cmake ../
make -j4
```