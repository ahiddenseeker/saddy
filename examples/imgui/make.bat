g++ game.cpp example.cpp sadimgui.cpp -o../../bin/example-imgui-debug.exe ../../3rdparty/imgui/imgui.cpp  ../../3rdparty/imgui/imgui_demo.cpp ../../3rdparty/imgui/imgui_draw.cpp -I../../3rdparty/imgui/ -I../../include/ -L../../lib/ -lsaddy-debug -lglu32 -lgdi32 -lm -lopengl32 -ladvapi32