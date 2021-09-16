#include "Window/Window.h"
#include "Visualization/ColorMidi.h"

int main() {
    Window mWindow("Midi Visualizer", 1280, 720);
    mWindow.loop();
    return 0;
}